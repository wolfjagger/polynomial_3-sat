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

#ifndef OPENMPI_TEST_EDGESEQUESNCE_H
#define OPENMPI_TEST_EDGESEQUESNCE_H

#include <utility>
#include <vector>
#include <bitset>
#include <iostream>
#include <set>

#include "clause.h"
#include "LiteralsMap.h"
#include "boost/dynamic_bitset.hpp"

namespace  ortho {

    using bit_sequence = std::shared_ptr<std::vector<std::bitset<3>>>;


    struct EdgeHeader
    {
        long long clause_idx1;
        long long clause_idx2;
        int element_idx1;
        int element_idx2;
        long long literal_val1, literal_val2;
        EdgeHeader(long long cl_idx1, long long cl_idx2, int ele_idx1, int ele_idx2,
         long long lit_val1, long long lit_val2):
                clause_idx1(cl_idx1), clause_idx2(cl_idx2), element_idx1(ele_idx1), element_idx2(ele_idx2),
        literal_val1(lit_val1), literal_val2(lit_val2){}
        bool operator==(const EdgeHeader &other) const
        { return (clause_idx1 == other.clause_idx1
                  && element_idx1 == other.element_idx1
                  && clause_idx2 == other.clause_idx2
                  && element_idx2 == other.element_idx2);
        }



        friend std::ostream& operator<<(std::ostream& os, const EdgeHeader& header)
        {
            os << "I" << header.clause_idx1 << "_"
                                               << header.element_idx1 << ","
                                                                         << header.clause_idx2 << "_" << header.element_idx2 ;
            return os;
        }
    };

    struct KeyHasher
    {
        std::size_t operator()(const EdgeHeader& other) const
        {
            using std::size_t;
            using std::hash;
            using std::string;

            std::string str_key = std::to_string(other.clause_idx1) +
                                  "_" +
                                  std::to_string(other.element_idx1) +
                                  "_" +
                                  std::to_string(other.clause_idx2) +
                                  "_" +
                                  std::to_string(other.element_idx2);
            size_t res = 17;
            res = res * 31 + hash<string>()( str_key);
            return res;
        }
    };
    class edge_sequence {
    public:
        edge_sequence(long long clause_index1, long long clause_index2, long long literal1, long long literal2,
                              int clause1_index, int clause2_index, ordered_clauses ord_clauses, literals_map lit_map);
        edge_sequence(bit_sequence bitSequence, literals_map literalsMap, ordered_clauses ord_clauses);
        edge_sequence(ordered_clauses ord_clauses, literals_map literalsMap);
        bit_sequence  get_bits();
        void set_bit(long long clause, size_t pos);
        void reset_bit(long long clause, size_t pos);
        bool is_valid();
        long long get_num_of_zeros() const;
        long long get_num_of_ones() const;
        bool operator<( const edge_sequence& other ) const ;
        bool operator>( const edge_sequence& other ) const ;

        std::bitset<3>& at(size_t pos) const;



        edge_sequence operator&(edge_sequence &other);
        edge_sequence operator|(edge_sequence &other);
        void set_sequence(const edge_sequence &other);

        EdgeHeader get_header() const;

        friend std::ostream& operator<<(std::ostream& os, const edge_sequence& edge);


    private:
        bool apply_lcr_and_k_rule();


    private:
        long long _clause_idx1, _clause_idx2;
        int _index_in_clause1, _index_in_clause2;
        literals_map _literals_map;
        literal _literal_idx1, _literal_idx2;
        ordered_clauses _ordered_clauses;
        bit_sequence _sequence;
        int _num_zeros;
        long long _num_ones;
        bool _zero_cell_found = false;
        bool _valid = true;
        std::set<long long> _updated_positions;
        bool _positions_updated = false;
    };


}



#endif //OPENMPI_TEST_EDGESEQUESNCE_H
