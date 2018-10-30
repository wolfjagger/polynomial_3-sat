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

#ifndef OPENMPI_TEST_PREPROCESSOR_H
#define OPENMPI_TEST_PREPROCESSOR_H

#include <list>
#include <map>

#include "DimacsReader.h"
#include "LiteralsMap.h"

namespace ortho {
    class Preprocessor {


    public:
        Preprocessor(const std::vector<clause> &clauses, int num_vars);

        void process(bool puresFound = true);
        void print_stats();
        std::vector<clause> get_processed_clauses();
        std::map<long long int, literal> get_literals_map();



    private:
        std::vector<clause> _clauses;
        int _num_literals;
        std::map<long long, literal> _literals;
        std::vector<long long> _pures, _all_pures;
        bool _is_init = false;
    };
}


#endif //OPENMPI_TEST_PREPROCESSOR_H
