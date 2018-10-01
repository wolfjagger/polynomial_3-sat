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
// Created by Asanka Wickramasinghe on 7/1/18.
// Copyright (c) 2018 Asanka & Ortho. All rights reserved.
//

#include <cmath>
#include <iostream>
#include "VertexSequence.h"


namespace  ortho
{
    vertex_sequence::vertex_sequence(long long clause_index, long long literal, int pos_in_clause,
                                     ordered_clauses ord_clauses, literals_map lit_map)
            :_ordered_clauses(ord_clauses), _literals_map(lit_map), _literal_val(literal),
             _index_in_clause(pos_in_clause),
             _clause_idx(clause_index),
             _num_zeros(0), _zero_cell_found(false)
    {
        //create bit set
        _sequence = std::make_shared<std::vector<std::bitset<3>>>();
        _sequence->resize(_ordered_clauses->size());


        //fill everything up with '1's
        std::for_each(_sequence->begin(), _sequence->end(), [](std::bitset<3> &bs){bs.set();});

        //process own clause
        //int val = std::pow(2, pos_in_clause);
        _sequence->at(clause_index).reset();
        _sequence->at(clause_index).set(pos_in_clause);

        //flipping bits
        std::list<std::pair<long long, int>> positions;
        if (_ordered_clauses->at(clause_index)._original_data.at(pos_in_clause) > 0)
        {
            //positive literal, need to flip negative positions
            positions.insert(positions.end(), _literals_map->at(literal)._negatives.begin(),
                             _literals_map->at(literal)._negatives.end());
        }
        else
        {
            //negative literal, need to flip positive positions
            positions.insert(positions.end(), _literals_map->at(std::abs(literal))._positives.begin(),
                             _literals_map->at(std::abs(literal))._positives.end());
        }

        for (const auto &position : positions)
        {
            _sequence->at(position.first).set(position.second, false);
        }

    }

    vertex_sequence::vertex_sequence(vertex_bit_sequence bitSequence, literals_map literalsMap, ordered_clauses ord_clauses)
            : _literals_map(literalsMap),
              _sequence(bitSequence), _ordered_clauses(ord_clauses)
    {
    }

    vertex_sequence::vertex_sequence(ordered_clauses ord_clauses, literals_map literalsMap): _literals_map(literalsMap),
                                                                                         _ordered_clauses(ord_clauses)
    {
        _sequence = std::make_shared<std::vector<std::bitset<3>>>();

        //resizing _sequence to number of clauses
        _sequence->resize(ord_clauses->size());

        //fill everything up with '0's
        std::for_each(_sequence->begin(), _sequence->end(), [](std::bitset<3> &bs){bs.reset();});
    }

    void vertex_sequence::set_bit(long long clause, size_t pos)
    {
        _sequence->at(clause).set(pos);
    }

    bool vertex_sequence::is_valid()
    {
        if (!_valid)
        {
            return false;
        }
        //std::cout << "checking validity of edge" << std::endl;
        _positions_updated = false;
        _updated_positions.clear();
        while (apply_lcr_and_k_rule())
        {}
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
    bool vertex_sequence::apply_lcr_and_k_rule()
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
            //skip end points (not needed)
            /*if (i == _clause_idx1 || i == _clause_idx2)
            {
                //continue;
            }*/
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
                        if (_sequence->at(pos.first).to_ulong() == 0)
                        {
                            _zero_cell_found = true;
                            return false;
                        }
                        //_updated_positions.insert(pos.first);
                        //_positions_updated = true;
                        ret_val = true;
                    }
                }
            }
        }
        return ret_val;
    }

    std::ostream& operator<<(std::ostream& os, const vertex_sequence& edge)
    {

        for (const auto &cell : *edge._sequence)
        {
            os << "(" << cell << ")";
        }
        return os;
    }

    vertex_bit_sequence vertex_sequence::get_bits()
    {
        return _sequence;
    }


    VertexHeader vertex_sequence::get_header()
    {
        return VertexHeader(_clause_idx, _index_in_clause, _literal_val);
    }

    void vertex_sequence::set_sequence(const vertex_sequence &other)
    {
        for (int i=0; i<_sequence->size(); i++)
        {
            _sequence->at(i) = other._sequence->at(i);
        }
    }

    void vertex_sequence::reset_bit(long long clause, size_t pos)
    {
        _sequence->at(clause).set(pos, false);

    }


}