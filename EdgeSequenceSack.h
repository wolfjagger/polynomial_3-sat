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

#ifndef OPENMPI_TEST_EDGESEQUENCESACK_H
#define OPENMPI_TEST_EDGESEQUENCESACK_H

#include "clause.h"
#include "LiteralsMap.h"
#include "EdgeSequesnce.h"
#include "VertexSequence.h"
#include "unordered_map"

namespace ortho
{
    struct SSet
    {
        long long clause1;
        long long clause2;
        friend std::ostream& operator<<(std::ostream& os, const SSet& sset)
        {
            os << "S" << sset.clause1 << ","
               << sset.clause2 ;
            return os;
        }
        std::vector<edge_sequence> edges;
    };


    class SequenceSack
    {
    public:
        SequenceSack(literals_map lit_map, ordered_clauses ord_clauses);
        void print_stats();
        bool start_comparing();
        bool is_satisfiable();

    private:
        bool compare(SSet &s1, SSet &s2);
        bool process_zero_edge(EdgeHeader zeroHeader);
        void remove_edge(EdgeHeader header);
        void remove_vertex(VertexHeader header);

        bool process_vertices(EdgeHeader header);

        void create_edges();
        void create_vertices();

    private:
        literals_map _literals_map;
        ordered_clauses  _ordered_clauses;
        std::unordered_map<EdgeHeader, edge_sequence, KeyHasher> _edges;
        std::unordered_map<VertexHeader, vertex_sequence, VertexKeyHasher> _vertices;
        std::vector<SSet> _s_sets;
        bool _bits_changed;

    };
}



#endif //OPENMPI_TEST_EDGESEQUENCESACK_H
