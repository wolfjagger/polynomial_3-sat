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

#include <iostream>
#include <mpi.h>
#include <bitset>
#include <vector>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <fstream>

#include "DimacsReader.h"
#include "Preprocessor.h"
#include "EdgeSequenceSack.h"

int main(int argc, char *argv[]) {

    if (argc != 2)
    {
        std::cout << "Invalid use of application. Please specify a DIMAC file" << std::endl;
        std::cout << std::endl;
        std::cout << "Usage " << std::endl;
        std::cout << argv[0] << "  <DIMAC file>" << std::endl;
        std::cout << std::endl;

        return -1;
    }
    {
        std::ifstream f(argv[1]);
        if (!f.good())
        {
            std::cout << "File: " << argv[1] << " does not exist. Exiting..." << std::endl;
            return -1;
        }
    }
    //first argument is the dimac file
    time_t tt;

    tt = std::chrono::system_clock::to_time_t ( std::chrono::system_clock::now() );
    std::cout << "start: " << ctime(&tt);
    ortho::DimacsReader reader(argv[1]);

    std::vector<ortho::clause> clauses = reader.get_clauses();

    ortho::Preprocessor preprocessor(reader.get_clauses(), reader.get_var_count());
    preprocessor.process();
    preprocessor.print_stats();



    long long count = reader.get_clause_count();

    std::vector<ortho::clause> processed_clauses = preprocessor.get_processed_clauses();
    std::map<long long, ortho::literal> literals = preprocessor.get_literals_map();

    //check clause count
    if (processed_clauses.size() == 0)
    {
        std::cout << std::endl;
        std::cout << "No clauses left after pre processing. Program won't go to comparing" << std::endl;
        return -1;
    }
    //check if we have zero element clauses
    for ( const auto& cl : processed_clauses)
    {
        if (cl._elements_count == 0)
        {
            std::cout << std::endl;
            std::cout << "A clause with zero elements found after preprocessing. No Solution exist" << std::endl;
            return -1;
        }
    }

    ortho::literals_map lit_map = std::make_shared<std::map<long long, ortho::literal>>(literals);
    ortho::ordered_clauses  ord_clauses = std::make_shared<std::vector<ortho::clause>>(processed_clauses);

    ortho::SequenceSack edges_sack(lit_map, ord_clauses);
    edges_sack.print_stats();


    //first round of sset comparing
    std::cout << std::endl;
    std::cout << "Starting S-Set comparing" << std::endl;
    if (!edges_sack.start_comparing())
    {
        std::cout << std::endl;
        std::cout << "Unsatisfiable SSet" << std::endl;
        tt = std::chrono::system_clock::to_time_t ( std::chrono::system_clock::now() );
        std::cout << "end : " << ctime(&tt);
    }
    else
    {
        std::cout << std::endl;
        std::cout << "Round 1 finished. Solution exists for this 3-Sat" << std::endl;
        std::cout << "status after first round" << std::endl;

        edges_sack.print_stats();

        tt = std::chrono::system_clock::to_time_t ( std::chrono::system_clock::now() );
        std::cout << "end : " << ctime(&tt);
    }




}