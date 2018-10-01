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

#ifndef OPENMPI_TEST_VERTEXSEQUENCE_H
#define OPENMPI_TEST_VERTEXSEQUENCE_H


#include <utility>
#include <vector>
#include <bitset>
#include <iostream>
#include <set>

#include "clause.h"
#include "LiteralsMap.h"

namespace  ortho {

    using vertex_bit_sequence = std::shared_ptr<std::vector<std::bitset<3>>>;

    struct VertexHeader
    {
        long long clause_idx1;
        int element_idx1;
        long long literal_val1;
        VertexHeader(long long cl_idx1,  int ele_idx1, long long lit_val1):
                clause_idx1(cl_idx1), element_idx1(ele_idx1),
                literal_val1(lit_val1){}
        bool operator==(const VertexHeader &other) const
        { return (clause_idx1 == other.clause_idx1
                  && element_idx1 == other.element_idx1
                  && literal_val1 == other.literal_val1);
        }
        friend std::ostream& operator<<(std::ostream& os, const VertexHeader& header)
        {
            os << "V" << header.clause_idx1 << "_"
               << header.element_idx1 << ","
               << header.literal_val1 ;
            return os;
        }
    };

    struct VertexKeyHasher
    {
        std::size_t operator()(const VertexHeader& other) const
        {
            using std::size_t;
            using std::hash;
            using std::string;

            std::string str_key = std::to_string(other.clause_idx1) +
                                  "_" +
                                  std::to_string(other.element_idx1);
            size_t res = 17;
            res = res * 31 + hash<string>()( str_key);
            return res;
        }
    };
    class vertex_sequence {
    public:
        vertex_sequence(long long clause_index, long long literal, int pos_in_clause,
                        ordered_clauses ord_clauses, literals_map lit_map);
        vertex_sequence(vertex_bit_sequence bitSequence, literals_map literalsMap, ordered_clauses ord_clauses);
        vertex_sequence(ordered_clauses ord_clauses, literals_map literalsMap);
        vertex_bit_sequence  get_bits();
        void set_bit(long long clause, size_t pos);
        void reset_bit(long long clause, size_t pos);
        bool is_valid();


        void set_sequence(const vertex_sequence &other);

        VertexHeader get_header();

        friend std::ostream& operator<<(std::ostream& os, const vertex_sequence& edge);


    private:
        bool apply_lcr_and_k_rule();


    private:
        long long _clause_idx;
        int _index_in_clause;
        literals_map _literals_map;
        long long _literal_val;
        ordered_clauses _ordered_clauses;
        vertex_bit_sequence _sequence;
        int _num_zeros;
        bool _zero_cell_found = false;
        bool _valid = true;
        std::set<long long> _updated_positions;
        bool _positions_updated = false;
    };


}

#endif //OPENMPI_TEST_VERTEXSEQUENCE_H
