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

#include <list>

#include "DimacsReader.h"
namespace ortho {
    DimacsReader::DimacsReader(const std::string &file_name)
    {
        std::ifstream input_stream(file_name);
        std::string line, trimmed_line;
        if (input_stream.is_open())
        {
            while (std::getline(input_stream, line))
            {
                trimmed_line = ortho::trim(line);
                if (ortho::string_starts_with(trimmed_line, "c"))
                {
                    continue;
                }
                std::vector<std::string> tokens = ortho::tokenize(trimmed_line, ' ');
                if (ortho::string_starts_with(trimmed_line, "p"))
                {
                    if (tokens.size() == 4)
                    {
                        _num_clauses = std::stoi(tokens.at(3));
                        _num_variables = std::stoi(tokens.at(2));
                    }
                    continue;
                }
                int val1, val2, val3;
                bool placeholder_inserted = false;
                if (tokens.size() == 4)
                {
                    val1 = std::stoi(tokens.at(0));
                    val2 = std::stoi(tokens.at(1));
                    val3 = std::stoi(tokens.at(2));
                }
                else if (tokens.size() == 3)
                {
                    val1 = std::stoi(tokens.at(0));
                    val2 = std::stoi(tokens.at(1));
                    val3 = 0;
                    placeholder_inserted = true;
                }
                else if (tokens.size() == 2)
                {
                    //_single_literals_.push_back(std::stoi(tokens.at(0)));
                    //continue;
                    val1 = std::stoi(tokens.at(0));
                    val2 = 0;
                    val3 = 0;
                    placeholder_inserted = true;
                }
                else
                {
                    continue;
                }

                int prevSize = _clauses.size();
                //check for quantums, ignore clauses with placeholders

                if (!placeholder_inserted)
                {
                    if (val2 == -val1 || val3 == -val1)
                    {
                        _quantum_clauses.emplace_back(val1, val2, val3);
                        continue;
                    }
                    if (val3 == -val2)
                    {
                        _quantum_clauses.emplace_back(val1, val2, val3);
                        continue;
                    }
                }
                else //process two variabl clauses
                {
                    if (val2 != val3)
                    {
                        if (val1 == -val2)
                        {
                            _quantum_clauses.emplace_back(val1, val2, val3);
                            continue;
                        }
                    }
                }
                _clauses.emplace(val1, val2, val3); //only use non quantum clauses


            }
            //remove quantums
            std::list<int> quantum_indices;

            _num_clauses = _clauses.size();
        }
    }

    long long DimacsReader::get_clause_count()
    {
        return _clauses.size();
    }

    long long DimacsReader::get_var_count()
    {
        return _num_variables;
    }

    std::vector<clause> DimacsReader::get_clauses()
    {
        return std::vector<clause>(_clauses.begin(), _clauses.end());
    }

    std::vector<long long> DimacsReader::get_single_clause_literals()
    {
        return _single_literals_;
    }


}
