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
// Created by Asanka Wickramasinghe on 6/18/18.
// Copyright (c) 2018 Asanka & Ortho. All rights reserved.
//
#include <chrono>
#include <ctime>

#include "EdgeSequenceSack.h"

namespace  ortho
{
    SequenceSack::SequenceSack(literals_map lit_map, ordered_clauses ord_clauses):
            _ordered_clauses(ord_clauses), _literals_map(lit_map)
    {
        create_vertices();
        create_edges();

    }

    bool SequenceSack::start_comparing()
    {
        long long count = 0;
        _bits_changed = true;

        //compare until no bit change is done
        while (_bits_changed)
        {
            _bits_changed = false;
            for (size_t i=0; i<_s_sets.size()-1; i++)
            {
#ifdef DEBUG_PRINT
                std::cout << std::endl;
                std::cout << "Comparing S-Set " << _s_sets.at(i).clause1 << " and S-Set " <<  _s_sets.at(i).clause2 << std::endl;
                std::cout << _edges.size() << " Edges remaining in the system " << std::endl;
                std::cout << _vertices.size() << " Vertices remaining in the system " << std::endl;
#endif

                for (size_t j=i+1; j<_s_sets.size(); j++)
                {
                    count++;
                    if (!compare(_s_sets.at(i), _s_sets.at(j)))
                    {
                        return false;
                    }
                }
            }

        }
#ifdef DEBUG_PRINT
        std::cout << "total compares: " << count << std::endl << std::flush;
#endif

        return true;
    }

    void SequenceSack::print_stats_final()
    {
        std::cout << std::endl;
        std::cout << "Number of Edge Sequences remaining: " << _edges.size() << std::endl;
        std::cout << "Number of Vertex Sequences remaining: " <<_vertices.size() << std::endl;

        std::cout << std::endl;
    }
    void SequenceSack::print_stats()
    {
        std::cout << std::endl;
        std::cout << "Number of Edge Sequences: " << _edges.size() << std::endl;
        std::cout << "Number of Vertex Sequences: " <<_vertices.size() << std::endl;
        std::cout << "Number of S-Sets: " << _s_sets.size() << std::endl;
        std::cout << std::endl;

#ifdef DEBUG_PRINT
        std::cout << "Remaining edges: " << std::endl;
        for (const auto&[ eg_header, edge ] : _edges)
        {
            std::cout << eg_header << std::endl;
        }
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "Remaining edges: " << std::endl;
        for (const auto&[ vt_header, vertex ] : _vertices)
        {
            std::cout << vt_header << std::endl;
        }
        std::cout << std::endl;
#endif

    }

    bool SequenceSack::compare(SSet &s1, SSet &s2)
    {

        //std::cout << "S" <<  s1.clause1 << "," << s1.clause2 << ", S" << s2.clause1 << ","
                                                                                       //<< s2.clause2 << std::endl;


        std::vector<edge_sequence> set1_edges, set2_edges;

        //set1  edges
        int s1_cl1_element_count = _ordered_clauses->at(s1.clause1)._elements_count;
        int s1_cl2_element_count = _ordered_clauses->at(s1.clause2)._elements_count;

        for (int i=0; i<s1_cl1_element_count; i++)
        {
            for (int j=0; j<s1_cl2_element_count; j++)
            {
                EdgeHeader header(s1.clause1, s1.clause2, i, j, _ordered_clauses->at(s1.clause1)._original_data.at(i),
                                  _ordered_clauses->at(s1.clause2)._original_data.at(j));
                if (_edges.count(header) > 0)
                {
                    set1_edges.push_back(_edges.at(header));
                }

            }
        }

        //set2  edges
        int s2_cl1_element_count = _ordered_clauses->at(s2.clause1)._elements_count;
        int s2_cl2_element_count = _ordered_clauses->at(s2.clause2)._elements_count;


        for (int i=0; i<s2_cl1_element_count; i++)
        {
            for (int j=0; j<s2_cl2_element_count; j++)
            {
                EdgeHeader header(s2.clause1, s2.clause2, i, j,_ordered_clauses->at(s2.clause1)._original_data.at(i),
                                  _ordered_clauses->at(s2.clause1)._original_data.at(j));
                if (_edges.count(header) > 0)
                {
                    set2_edges.push_back(_edges.at(header));
                }

            }
        }
        if (set1_edges.empty())
        {
            std::cout << "Empty SSet " << s1 << ". Unsatisfiable 3-sat" << std::endl;
            return false;
        }
        if (set2_edges.empty())
        {
            std::cout << "Empty SSet " << s2 << ". Unsatisfiable 3-sat" << std::endl;
            return false;
        }
        std::list<long long> to_be_removed;



        //start left to right
        bool bits_changed = false;
        auto started = std::chrono::high_resolution_clock::now();


        for (size_t i=0; i<set2_edges.size(); i++)
        {
            long long num_ones = set2_edges.at(i).get_num_of_ones();
            std::vector<edge_sequence> intersection_results;
            for (size_t j=0; j<set1_edges.size(); j++)
            {

                edge_sequence intersection_result = set2_edges.at(i) & set1_edges.at(j);
                /*std::cout << "E1: " << set1_edges.at(j) << std::endl << std::flush;
                std::cout << "E2: " << set2_edges.at(i) << std::endl << std::flush;
                std::cout << "Int: " << intersection_result << std::endl << std::flush;*/
                //check if we have any bits changed
                if (intersection_result.get_num_of_ones() < num_ones)
                {
                    if (intersection_result.is_valid())
                    {
                        intersection_results.push_back(intersection_result);
                    }
                }
                else
                {
                    //std::cout << "saved" << std::endl << std::flush;
                    intersection_results.push_back(intersection_result);
                }

            }
            if (intersection_results.size() == 0)
            {
                //invalid edge. remove from edges
                _bits_changed = true;
                EdgeHeader header_rm = set2_edges.at(i).get_header();
                remove_edge(header_rm);
                if (!process_zero_edge(header_rm))
                {
                    return false; //empty clause found
                }
                if (!process_vertices(header_rm))
                {
                    return false;
                }
                set2_edges.erase(set2_edges.begin() + i);
                if (set2_edges.size() == 0)
                {
                    std::cout << "empty cl" << std::endl;
                }
                i--; //since we removed an element elements have jumped one
            }
            else
            {
                edge_sequence union_result = intersection_results.at(0);
                for (int k=1; k<intersection_results.size(); k++)
                {
                    union_result = union_result | intersection_results.at(k);
                }
                //find which bit positions have updated
                std::vector<std::pair<long long, int>> bit_changed_positions;
                //std::cout << set2_edges.at(i) << std::endl;
                //std::cout << union_result << std::endl;
                //std::cout << union_result << std::endl;

                for (int cls=0; cls<_ordered_clauses->size(); cls++)
                {
                    unsigned long changes = (union_result.at(cls) ^ set2_edges.at(i).at(cls)).to_ulong();
                    if (changes != 0)
                    {
                        _bits_changed = true;
                    }

                    switch (changes)
                    {
                        case 1: //001
                        {
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 0));
                            break;
                        }
                        case 2: //010
                        {
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 1));
                            break;
                        }
                        case 3: //011
                        {
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 0));
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 1));
                            break;
                        }
                        case 4: //100
                        {
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 2));
                            break;
                        }
                        case 5: //101
                        {
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 2));
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 0));
                            break;
                        }
                        case 6: //110
                        {
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 2));
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 1));
                            break;
                        }
                        default:
                        {
                            break;
                        }


                    }

                }
                for ( std::pair<long long, int> bit_pos : bit_changed_positions)
                {
                    long long lit_val = _ordered_clauses->at(bit_pos.first)._original_data.at(bit_pos.second);
                    std::list<std::pair<long long, int>> positions;
                    if (lit_val > 0)
                    {

                        positions = _literals_map->find(lit_val)->second._positives;
                    }
                    else
                    {

                        positions = _literals_map->find(-lit_val)->second._negatives;
                    }

                    for( const auto& pos : positions)
                    {
                        union_result.at(pos.first).reset(pos.second);

                    }
                }
                //std::cout << union_result << std::endl;
                //std::cout << "===================" << std::endl;
                if (!union_result.is_valid())
                {
                    _bits_changed = true;
                    EdgeHeader header_rm = set2_edges.at(i).get_header();
                    remove_edge(header_rm);
                    if (!process_zero_edge(header_rm))
                    {
                        return false;
                    }
                    if (!process_vertices(header_rm))
                    {
                        return false;
                    }
                    set2_edges.erase(set2_edges.begin() + i);
                    if (set2_edges.size() == 0)
                    {
                        std::cout << "empty cl" << std::endl;
                    }
                    i--; //since we removed an element elements have jumped one
                }
                else
                {
                    set2_edges.at(i).set_sequence(union_result);
                }

            }


        }

        //start right to left
        for (size_t i=0; i<set1_edges.size(); i++)
        {
            long long num_ones= set1_edges.at(i).get_num_of_ones();
            std::vector<edge_sequence> intersection_results;
            for (size_t j=0; j<set2_edges.size(); j++)
            {

                edge_sequence intersection_result = set1_edges.at(i) & set2_edges.at(j);
                if (intersection_result.get_num_of_ones() < num_ones)
                {
                    if (intersection_result.is_valid())
                    {
                        intersection_results.push_back(intersection_result);
                    }
                }
                else
                {
                    //std::cout << "saved" << std::endl << std::flush;
                    intersection_results.push_back(intersection_result);
                }

            }

            edge_sequence union_result(_ordered_clauses, _literals_map);
            if (intersection_results.size() == 0)
            {
                //remove edge
                _bits_changed = true;
                EdgeHeader header_rm = set1_edges.at(i).get_header();
                remove_edge(header_rm);

                if (!process_zero_edge(header_rm))
                {
                    return false;
                }

                if (!process_vertices(header_rm))
                {
                    return false;
                }
                set1_edges.erase(set1_edges.begin() + i);
                i--;
            }
            else
            {
                for (int k=0; k<intersection_results.size(); k++)
                {
                    union_result = union_result | intersection_results.at(k);
                }
                //find which bit positions have updated
                std::vector<std::pair<long long, int>> bit_changed_positions;

                for (int cls=0; cls<_ordered_clauses->size(); cls++)
                {
                    unsigned long changes = (union_result.at(cls) ^ set1_edges.at(i).at(cls)).to_ulong();
                    if (changes != 0)
                    {
                        _bits_changed = true;
                    }

                    switch (changes)
                    {
                        case 1: //001
                        {
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 0));
                            break;
                        }
                        case 2: //010
                        {
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 1));
                            break;
                        }
                        case 3: //011
                        {
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 0));
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 1));
                            break;
                        }
                        case 4: //100
                        {
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 2));
                            break;
                        }
                        case 5: //101
                        {
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 2));
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 0));
                            break;
                        }
                        case 6: //110
                        {
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 2));
                            bit_changed_positions.push_back(std::make_pair<long, int>(cls, 1));
                            break;
                        }
                        default:
                        {
                            break;
                        }


                    }
                    for ( std::pair<long long, int> bit_pos : bit_changed_positions)
                    {
                        long long lit_val = _ordered_clauses->at(bit_pos.first)._original_data.at(bit_pos.second);
                        std::list<std::pair<long long, int>> positions;
                        if (lit_val > 0)
                        {
                            //need to flip negative literal
                            positions = _literals_map->find(lit_val)->second._positives;
                        }
                        else
                        {
                            //need to flip positive literal
                            positions = _literals_map->find(-lit_val)->second._negatives;
                        }

                        for( const auto& pos : positions)
                        {
                            union_result.at(pos.first).set(pos.second, 0);

                        }
                    }
                }
                if (!union_result.is_valid())
                {
                    _bits_changed = true;
                    EdgeHeader header_rm = set1_edges.at(i).get_header();
                    remove_edge(header_rm);
                    if (!process_zero_edge(header_rm))
                    {
                        return false;
                    }
                    if (!process_vertices(header_rm))
                    {
                        return false;
                    }
                    set1_edges.erase(set1_edges.begin() + i);
                    i--; //since we removed an element elements have jumped one
                }
                else
                {
                    set1_edges.at(i).set_sequence(union_result);
                }

            }

        }

        return true;
    }

    bool SequenceSack::process_zero_edge(EdgeHeader zeroHeader)
    {
        bool ret_val = true;
        long long lit_val1 = zeroHeader.literal_val1;
        long long lit_val2 = zeroHeader.literal_val2;
        std::list<std::pair<long long, int>> positions_of_x;
        if (lit_val1 > 0)
        {
            //need to flip negative literal
            positions_of_x = _literals_map->find(lit_val1)->second._positives;
        }
        else
        {
            //need to flip positive literal
            positions_of_x = _literals_map->find(-lit_val1)->second._negatives;
        }


        std::list<std::pair<long long, int>> positions_of_y;
        if (lit_val2 > 0)
        {
            //need to flip negative literal
            positions_of_y = _literals_map->find(lit_val2)->second._positives;
        }
        else
        {
            //need to flip positive literal
            positions_of_y = _literals_map->find(-lit_val2)->second._negatives;
        }
        std::list<std::pair<long long, int>> positions_of_others;
        long long lit1_lookup_val = (lit_val1 > 0) ? lit_val1 : -lit_val1;
        long long lit2_lookup_val = (lit_val2 > 0) ? lit_val2 : -lit_val2;
        for (const auto &literal : *_literals_map.get())
        {
            if ((literal.first != lit1_lookup_val) && (literal.first != lit2_lookup_val))
            {
                positions_of_others.insert(positions_of_others.end(), literal.second._negatives.begin(),
                                           literal.second._negatives.end());
                positions_of_others.insert(positions_of_others.end(), literal.second._positives.begin(),
                                           literal.second._positives.end());
            }
        }
        //need to add -x and -y locations
        if (lit_val1 > 0)
        {
            positions_of_others.insert(positions_of_others.end(), _literals_map->at(lit1_lookup_val)._negatives.begin(),
                                       _literals_map->at(lit1_lookup_val)._negatives.end());
        }
        else
        {
            positions_of_others.insert(positions_of_others.end(), _literals_map->at(lit1_lookup_val)._positives.begin(),
                                       _literals_map->at(lit1_lookup_val)._positives.end());
        }
        if (lit_val2 > 0)
        {
            positions_of_others.insert(positions_of_others.end(), _literals_map->at(lit2_lookup_val)._negatives.begin(),
                                       _literals_map->at(lit2_lookup_val)._negatives.end());
        }
        else
        {
            positions_of_others.insert(positions_of_others.end(), _literals_map->at(lit2_lookup_val)._positives.begin(),
                                       _literals_map->at(lit2_lookup_val)._positives.end());
        }

        //remove Ix,y (part of refinement rules)
        //find all Ix,y
        for (const auto& x_pos: positions_of_x)
        {
            for (const auto &y_pos : positions_of_y)
            {
                EdgeHeader header(x_pos.first, y_pos.first, x_pos.second, y_pos.second, 0, 0);
                {
                    if (_edges.count(header) > 0)
                    {
                        remove_edge(header);
                    }
                }
                EdgeHeader header2(y_pos.first, x_pos.first, y_pos.second, x_pos.second, 0, 0);
                {
                    if (_edges.count(header2) > 0)
                    {
                        remove_edge(header2);
                    }
                }
            }
        }
        std::list<EdgeHeader> edges_to_be_modified;
        for (const auto &y_pos : positions_of_y)
        {
            for (const auto &other: positions_of_others)
            {
                EdgeHeader header(y_pos.first, other.first, y_pos.second, other.second, 0, 0);
                if (_edges.count(header) > 0)
                {
                    edges_to_be_modified.push_back(header);
                }
                EdgeHeader header2(other.first, y_pos.first, other.second, y_pos.second, 0, 0);
                if (_edges.count(header2) > 0)
                {
                    edges_to_be_modified.push_back(header2);
                }
            }
        }
        for (const auto &x_pos : positions_of_x)
        {
            for (const auto &other: positions_of_others)
            {
                EdgeHeader header(x_pos.first, other.first, x_pos.second, other.second, 0, 0);
                if (_edges.count(header) > 0)
                {
                    edges_to_be_modified.push_back(header);
                }
                EdgeHeader header2(other.first, x_pos.first, other.second, x_pos.second, 0, 0);
                if (_edges.count(header2) > 0)
                {
                    edges_to_be_modified.push_back(header2);
                }
            }
        }

        //we still have to look for Ix,x and Iy,y
        for (int i=0; i<positions_of_x.size()-1; i++)
        {
            for (int j=i+1; j<positions_of_x.size(); j++)
            {
                EdgeHeader header((*std::next(positions_of_x.begin(), i)).first, (*std::next(positions_of_x.begin(), j)).first,
                                  (*std::next(positions_of_x.begin(), i)).second, (*std::next(positions_of_x.begin(), j)).second, 0, 0);
                if (_edges.count(header) > 0)
                {
                    edges_to_be_modified.push_back(header);
                }
            }
        }

        for (int i=0; i<positions_of_y.size()-1; i++)
        {
            for (int j=i+1; j<positions_of_y.size(); j++)
            {
                EdgeHeader header((*std::next(positions_of_y.begin(), i)).first, (*std::next(positions_of_y.begin(), j)).first,
                                  (*std::next(positions_of_y.begin(), i)).second, (*std::next(positions_of_y.begin(), j)).second, 0, 0);
                if (_edges.count(header) > 0)
                {
                    edges_to_be_modified.push_back(header);
                }
            }
        }


        size_t to_be_mod_count = edges_to_be_modified.size();
        std::list<EdgeHeader> to_be_removed;
        for (const auto &e_header : edges_to_be_modified)
        {

            if (e_header.literal_val2 == lit_val2 ||
                    e_header.literal_val1 == lit_val2  )
            {
                edge_sequence seq = _edges.at(e_header);
                for (const auto &pos : positions_of_x)
                {
                    seq.reset_bit(pos.first, pos.second);
                }
                if (!seq.is_valid())
                {
                    to_be_removed.push_back(seq.get_header());
                }
            }
                //process second literal
            else if(e_header.literal_val1 == lit_val1 ||
                    e_header.literal_val2 == lit_val1 )
            {
                edge_sequence seq = _edges.at(e_header);
                for (const auto &pos : positions_of_y)
                {
                    seq.reset_bit(pos.first, pos.second);
                }
                if (!seq.is_valid())
                {
                    to_be_removed.push_back(seq.get_header());
                }
            }
        }
        for (const auto& item : to_be_removed)
        {
            remove_edge(item);
            if (!process_vertices(item))
            {
                ret_val = false;
            }
        }
        return ret_val;
    }

    bool SequenceSack::process_vertices(EdgeHeader header)
    {
        bool ret_val = true;
        //resetting y in all Vx and x in all Vy
        long long literal_x = header.literal_val1;
        long long literal_y = header.literal_val2;
        std::list<std::pair<long long, int>> x_positions, y_positions;
        if (literal_x > 0)
        {
           x_positions =  _literals_map->find(literal_x)->second._positives;
        }
        else
        {
            x_positions =  _literals_map->find(-literal_x)->second._negatives;
        }

        if (literal_y > 0)
        {
            y_positions =  _literals_map->find(literal_y)->second._positives;
        }
        else
        {
            y_positions =  _literals_map->find(-literal_y)->second._negatives;
        }

        std::vector<VertexHeader> to_be_removed;

        for (auto & [ vt_header, vertex ]: _vertices)
        {
            if (vt_header.literal_val1 == literal_x)
            {
                for (const auto &position : y_positions)
                {

                    vertex.reset_bit(position.first, position.second);

                }
                if (!vertex.is_valid())
                {
                    if (vt_header.literal_val1 == -20)
                    {
                        int asa = 0;
                    }
                    to_be_removed.push_back(vt_header);
                }
            }
            else if (vt_header.literal_val1 == literal_y)
            {
                for (const auto &position : x_positions)
                {

                    vertex.reset_bit(position.first, position.second);

                }
                if (!vertex.is_valid())
                {

                    to_be_removed.push_back(vt_header);
                }
            }
        }

        //if Vxi is removed we remove all Vx
        std::vector<VertexHeader> all_Vx;
        for (const auto &tbr_header : to_be_removed)
        {
            long long literal_xi = tbr_header.literal_val1;
            for (const auto & [ other_header, vertex ]: _vertices)
            {
                if (other_header.literal_val1 == literal_xi)
                {

                    all_Vx.push_back(other_header);
                }
            }

        }
        to_be_removed.insert(to_be_removed.end(), all_Vx.begin(), all_Vx.end());
        //now we have to sets of  vertices, to_be_removed and all_Vx we need to remove; If Vx is removed,
        //then we need to remove all Ix,#. Below is a brute force method which is not optimized for the
        //task. We need better indexing and grouping for edges and vertices to be able to do this efficiently.

        //finding Ix,# and remove them
        std::vector<EdgeHeader> edges_to_be_removed;
        for (const auto &v_header : to_be_removed)
        {
            for (const auto & [ e_header, edge ]: _edges)
            {
                if (e_header.literal_val1 == v_header.literal_val1 ||
                        e_header.literal_val2 == v_header.literal_val1)
                {
                    edges_to_be_removed.push_back(e_header);
                }
            }
            remove_vertex(v_header);
        }

        for (const auto &e_header : edges_to_be_removed)
        {
            remove_edge(e_header);
        }
        //check if we created empty clause
        for (int i=0; i<_ordered_clauses->size(); i++)
        {
            int vertex_count = 0;
            int clause_size = _ordered_clauses->at(i)._elements_count;
            for (int j=0; j<clause_size; j++)
            {
                VertexHeader hdr(i, j, _ordered_clauses->at(i)._original_data.at(j));
                if (_vertices.count(hdr) > 0)
                {
                    vertex_count++;
                }

            }
            if (vertex_count == 0) //zero clause
            {
                std::cout << "The vertex-sequences associated with clause " << i << " are zero." << std::endl;
                return false;
            }
        }
        return ret_val;
    }

    void SequenceSack::remove_edge(EdgeHeader header)
    {
#ifdef DEBUG_PRINT
        std::cout << "Edge Removed: " << header << std::endl;
#endif
        _edges.erase(header);
    }

    void SequenceSack::create_edges()
    {
#ifdef DEBUG_PRINT
        std::cout << "creating edges" << std::endl;
#endif
        for (size_t i=0; i<_ordered_clauses->size()-1; i++)
        {
            for (size_t j=i+1; j<_ordered_clauses->size(); j++)
            {
                SSet sset({(long long)i, (long long)j});

                for (int k=0; k<_ordered_clauses->at(i)._elements_count; k++)
                {
                    for (int l=0; l<_ordered_clauses->at(j)._elements_count; l++)
                    {

                        edge_sequence edge = edge_sequence(i, j, std::abs(_ordered_clauses->at(i)._original_data.at(k)),
                                                           std::abs(_ordered_clauses->at(j)._original_data.at(l)), k, l,
                                                           _ordered_clauses, _literals_map);

                        if (edge.is_valid())
                        {
                            EdgeHeader head(i, j, k, l, _ordered_clauses->at(i)._original_data.at(k),
                                            _ordered_clauses->at(j)._original_data.at(l));
                            _edges.insert(std::make_pair(head, edge));
                            sset.edges.push_back(edge);

                        }
                        auto done = std::chrono::high_resolution_clock::now();
                    }
                }
                _s_sets.push_back(sset);
            }
        }
    }

    void SequenceSack::create_vertices()
    {
        //std::cout << _edges.size() << " valid edgers created" << std::endl;
        //std::cout << "creating vertex sequences" << std::endl;

        for (size_t i =0; i<_ordered_clauses->size(); i++)
        {
            clause cl = _ordered_clauses->at(i);
            for (int j=0; j<cl._elements_count; j++)
            {
                long long literal = _ordered_clauses->at(i)._original_data.at(j);
                vertex_sequence vertex(i, literal, j, _ordered_clauses, _literals_map);
                if (vertex.is_valid())
                {
                    VertexHeader header(i, j, literal);
                    _vertices.insert(std::make_pair(header, vertex));
                }
            }
        }
        std::cout << _vertices.size() << " valid vertices created" << std::endl;
    }

    void SequenceSack::remove_vertex(VertexHeader header)
    {
        _vertices.erase(header);
    }



}

