/*****************************************************************************
 * Disclaimer:THE SAMPLE CODE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED *
 * WARRANTIES,  INCLUDING  THE  IMPLIED  WARRANTIES OF  MERCHANTABILITY  AND *
 * FITNESS FOR A  PARTICULAR PURPOSE  ARE DISCLAIMED. IN  NO EVENT SHALL THE *
 * AUTHORS  BE  LIABLE  FOR  ANY  DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,*
 * EXEMPLARY,  OR  CONSEQUENTIAL   DAMAGES  (INCLUDING,  BUT  NOT LIMITED TO,*
 * PROCUREMENT  OF  SUBSTITUTE   GOODS  OR SERVICES;  LOSS  OF USE, DATA, OR *
 * PROFITS;  OR BUSINESS  INTERRUPTION)  SUSTAINED  BY YOU OR A  THIRD PARTY,*
 * HOWEVER CAUSED AND ON ANY  THEORY OF  LIABILITY,  WHETHER   IN   CONTRACT,*
 * STRICT LIABILITY,OR TORT ARISING IN ANY WAY OUT OF THE USE OF THIS SAMPLE *
 * CODE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                  *
 ****************************************************************************/

//
// Created by Asanka Wickramasinghe on 4/8/18.
// Copyright (c) 2018 Asanka & Ortho. All rights reserved.
//

#include <cmath>
#include <iostream>
#include <chrono>

#include "EdgeSequesnce.h"
namespace  ortho
{
    edge_sequence::edge_sequence(long long clause_index1, long long clause_index2, long long literal1, long long literal2,
                                     int clause1_index, int clause2_index, ordered_clauses ord_clauses, literals_map lit_map)
            :_ordered_clauses(ord_clauses), _literals_map(lit_map),
             _index_in_clause1(clause1_index), _index_in_clause2(clause2_index),
            _clause_idx1(clause_index1), _clause_idx2(clause_index2),
             _num_zeros(0), _num_ones(0), _zero_cell_found(false)
    {

        auto started = std::chrono::high_resolution_clock::now();
        //check if two literals are negations of each other. That case edge is invalid
        if (literal1 == -literal2)
        {
            _valid = false;
            return;
        }
        _literal_idx1 = _literals_map->at(literal1);
        _literal_idx2 = _literals_map->at(literal2);

        //create bit set
        _sequence = std::make_shared<std::vector<std::bitset<3>>>();
        _sequence->resize(_ordered_clauses->size());

        
        //fill everything up with '1's
        std::for_each(_sequence->begin(), _sequence->end(), [](std::bitset<3> &bs){bs.set();});

        //setup edge clauses
        _sequence->at(clause_index1).reset();
        _sequence->at(clause_index1).set(clause1_index);

        _sequence->at(clause_index2).reset();
        _sequence->at(clause_index2).set(clause2_index);

        //flipping bits
        std::list<std::pair<long long, int>> positions;
        if (_ordered_clauses->at(clause_index1)._original_data.at(clause1_index) > 0)
        {
            //positive literal, need to flip negative positions
            positions.insert(positions.end(), _literal_idx1._negatives.begin(), _literal_idx1._negatives.end());
        }
        else
        {
            //negative literal, need to flip positive positions
            positions.insert(positions.end(), _literal_idx1._positives.begin(), _literal_idx1._positives.end());
        }

        if (_ordered_clauses->at(clause_index2)._original_data.at(clause2_index) > 0)
        {
            //positive literal, need to flip negative positions
            positions.insert(positions.end(), _literal_idx2._negatives.begin(), _literal_idx2._negatives.end());
        }
        else
        {
            //negative literal, need to flip positive positions
            positions.insert(positions.end(), _literal_idx2._positives.begin(), _literal_idx2._positives.end());
        }

        for (const auto& pos : positions)
        {
            _sequence->at(pos.first).set(pos.second, 0);
        }

        //set placeholder bits to zero
        for (size_t i=0; i<_ordered_clauses->size(); i++)
        {
            if (_ordered_clauses->at(i)._elements_count != 3)
            {
                //there should be only 2 literal clauses as 1 literal clauses were removed in
                //earlier stage
                _sequence->at(i).set(2, false);

            }
        }
        auto done = std::chrono::high_resolution_clock::now();
        _num_zeros = 3 * _sequence->size();
        //std::cout << "edge " << std::chrono::duration_cast<std::chrono::microseconds>(done-started).count() << std::endl << std::flush;



    }

    edge_sequence::edge_sequence(bit_sequence bitSequence, literals_map literalsMap, ordered_clauses ord_clauses)
            : _literals_map(literalsMap),
              _sequence(bitSequence), _ordered_clauses(ord_clauses)
    {
        _num_zeros = 3 * _sequence->size();
    }

    edge_sequence::edge_sequence(ordered_clauses ord_clauses, literals_map literalsMap): _literals_map(literalsMap),
                                                                                         _ordered_clauses(ord_clauses)
    {
        _sequence = std::make_shared<std::vector<std::bitset<3>>>();

        //resizing _sequence to number of clauses
        _sequence->resize(ord_clauses->size());

        //fill everything up with '0's
        std::for_each(_sequence->begin(), _sequence->end(), [](std::bitset<3> &bs){bs.reset();});
        _num_zeros = 3 * _sequence->size();
    }

    void edge_sequence::set_bit(long long clause, size_t pos)
    {
        _sequence->at(clause).set(pos);
    }

    bool edge_sequence::is_valid()
    {

        auto started = std::chrono::high_resolution_clock::now();
        if (!_valid)
        {
            return false;
        }
        //std::cout << "checking validity of edge" << std::endl;
        _positions_updated = false;
        _updated_positions.clear();
        while (apply_lcr_and_k_rule())
        {}
        auto done = std::chrono::high_resolution_clock::now();

        //std::cout << "valid " << std::chrono::duration_cast<std::chrono::microseconds>(done-started).count() << std::endl << std::flush;

        //check if zero value cells found
        if (_zero_cell_found)
        {
            _valid = false;
            return false;
        }
        else
        {
            return true;
        }

    }

    //will return true if bit change has occured
    bool edge_sequence::apply_lcr_and_k_rule()
    {
        //std::cout << "applying LCR and K rule" << std::endl;
        bool ret_val = false;
        std::vector<long long> check_positions;

        if (_positions_updated)
        {
            for( const auto& pos : _updated_positions)
            {
                check_positions.push_back(pos);
            }
            _updated_positions.clear();
            _positions_updated = false;
        }
        else
        {
            for (size_t i=0; i<_sequence->size(); i++)
            {
                check_positions.push_back(i);
            }
        }
        for (const auto &i : check_positions)
        {

            std::bitset<3> &cell = _sequence->at(i);
            unsigned long value = cell.to_ulong();
            // std::cout << value << std::endl;
            int pos = -1;
            switch (value)
            {
                case 0: //000
                {
                    _zero_cell_found = true;
                    return false;
                    break;
                }
                case 1: //001
                {
                    pos = 0;
                    break;
                }
                case 2: //010
                {
                    pos = 1;
                    break;
                }
                case 4: //100
                {
                    pos = 2;
                    break;
                }
                default:
                {
                    break;
                }
            }
            if (pos != -1)
            {
                //std::cout << "loner cell found. Pos: " << pos << ". clause: " << i << std::endl;
                //cell is a loner. find which literals we need to flip
                long long lit_val = _ordered_clauses->at(i)._original_data.at(pos);
                std::list<std::pair<long long, int>> positions;
                if (lit_val > 0)
                {
                    //need to flip negative literal
                    positions = _literals_map->find(lit_val)->second._negatives;
                }
                else
                {
                    //need to flip positive literal
                    positions = _literals_map->find(-lit_val)->second._positives;
                }

                for( const auto& pos : positions)
                {
                    if (_sequence->at(pos.first).test(pos.second))
                    {
                        _sequence->at(pos.first).set(pos.second, 0);
                        /*if (_sequence->at(pos.first).to_ulong() == 0)
                        {
                            _zero_cell_found = true;
                            return false;
                        }*/
                        //_updated_positions.insert(pos.first);
                        //_positions_updated = true;
                        ret_val = true;
                    }
                }
            }
        }
        return ret_val;
    }

    std::ostream& operator<<(std::ostream& os, const edge_sequence& edge)
    {

        for (const auto &cell : *edge._sequence)
        {
            os << "(" << cell << ")";
        }
        return os;
    }

    bit_sequence edge_sequence::get_bits()
    {
        return _sequence;
    }

    edge_sequence edge_sequence::operator&(edge_sequence &other)
    {
        bit_sequence sequence = std::make_shared<std::vector<std::bitset<3>>>();
        sequence->resize(_ordered_clauses->size());

        //fill everything up with '0's
        std::for_each(sequence->begin(), sequence->end(), [](std::bitset<3> &bs){bs.reset();});
        size_t ss = _sequence->size();
        //check if we can omit intersections
        long long edge2_clause1_index = other._clause_idx1;
        long long edge2_clause2_index = other._clause_idx2;

        if ((_sequence->at(edge2_clause1_index) & other._sequence->at(edge2_clause1_index)).to_ulong() == 0)
        {
            return edge_sequence(sequence, _literals_map, _ordered_clauses);
        }
        if ((_sequence->at(edge2_clause2_index) & other._sequence->at(edge2_clause2_index)).to_ulong() == 0)
        {
            return edge_sequence(sequence, _literals_map, _ordered_clauses);
        }
        if ((_sequence->at(_clause_idx1) & other._sequence->at(_clause_idx1)).to_ulong() == 0)
        {
            return edge_sequence(sequence, _literals_map, _ordered_clauses);
        }
        if ((_sequence->at(_clause_idx2) & other._sequence->at(_clause_idx2)).to_ulong() == 0)
        {
            return edge_sequence(sequence, _literals_map, _ordered_clauses);
        }
        for (int i=0; i<_sequence->size(); i++)
        //for (int i=0; i<3; i++)
        {
            sequence->at(i) = _sequence->at(i) & other._sequence->at(i);
            /*if (sequence->at(i).to_ulong() == 0)
            {
                //std::cout << "zero found" << std::endl;
                return edge_sequence(sequence, _literals_map, _ordered_clauses);
            }*/
        }

        return edge_sequence(sequence, _literals_map, _ordered_clauses);
    }

    edge_sequence edge_sequence::operator|(edge_sequence &other)
    {

        bit_sequence sequence = std::make_shared<std::vector<std::bitset<3>>>();
        sequence->resize(_ordered_clauses->size());


        for (int i=0; i<_sequence->size(); i++)
        {
            sequence->at(i) = _sequence->at(i) | other._sequence->at(i);
        }

        return edge_sequence(sequence, _literals_map, _ordered_clauses);
    }

    EdgeHeader edge_sequence::get_header() const
    {
        long long lit_val1 = _ordered_clauses->at(_clause_idx1)._original_data.at(_index_in_clause1);
        long long lit_val2 = _ordered_clauses->at(_clause_idx2)._original_data.at(_index_in_clause2);
        return EdgeHeader(_clause_idx1, _clause_idx2, _index_in_clause1, _index_in_clause2,
        lit_val1, lit_val2);
    }

    void edge_sequence::set_sequence(const edge_sequence &other)
    {
        for (int i=0; i<_sequence->size(); i++)
        {
            _sequence->at(i) = other._sequence->at(i);
        }
    }

    void edge_sequence::reset_bit(long long clause, size_t pos)
    {
        _sequence->at(clause).set(pos, false);

    }

    long long edge_sequence::get_num_of_zeros() const
    {
        long long ret_val = 0;
        for (int i=0; i<_sequence->size(); i++)
        {
            int ones_count = _sequence->at(i).count();
            int ele_cout = _ordered_clauses->at(i)._elements_count;
            ret_val += (ele_cout - ones_count);
        }

        return ret_val;
    }

    bool edge_sequence::operator<(const edge_sequence &other) const
    {
        return (other.get_num_of_zeros() < this->get_num_of_zeros());
    }
    bool edge_sequence::operator>(const edge_sequence &other) const
    {
        return (other.get_num_of_zeros() > this->get_num_of_zeros());
    }

    long long edge_sequence::get_num_of_ones() const
    {
        long long ret_val = 0;
        for (int i=0; i<_sequence->size(); i++)
        {
            ret_val += _sequence->at(i).count();
        }

        return ret_val;
    }

    std::bitset<3> &edge_sequence::at(size_t pos) const
    {
        return _sequence->at(pos);
    }


}


