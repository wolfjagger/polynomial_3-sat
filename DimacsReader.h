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
// Created by Asanka Wickramasinghe on 3/4/18.
// Copyright (c) 2018 Asanka & Ortho. All rights reserved.
//
#ifndef OPENMPI_TEST_DIMACSREADER_H
#define OPENMPI_TEST_DIMACSREADER_H

#include <algorithm>
#include <string>
#include <array>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <list>

#include "Utilities.h"
#include "clause.h"

namespace ortho {


    class DimacsReader {
    public:

        DimacsReader(const std::string &file_name);
        std::vector<clause> get_clauses();
        std::vector<long long> get_single_clause_literals();
        long long get_clause_count();
        long long get_var_count();

    private:
        long long _num_clauses;
        long long _num_variables;
        std::set<clause> _clauses;
        std::vector<clause> _quantum_clauses;
        std::vector<long long> _single_literals_;
    };
}

#endif //OPENMPI_TEST_DIMACSREADER_H
