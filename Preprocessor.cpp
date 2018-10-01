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

#include <functional>
#include "Preprocessor.h"
namespace ortho
{
    Preprocessor::Preprocessor(const std::vector<clause> &clauses, int num_vars):_clauses(clauses), _num_literals(num_vars)
    {
    }

    void Preprocessor::process()
    {
        std::set<long long> clausesToBeRemovedSet;
        std::set<long long> lonerLiteralsSet;
        if (_is_init)
        {
            if (_pures.empty() && lonerLiteralsSet.empty())
                return;
        }
        _is_init = true;

        for (const auto& _val : _pures)
        {
            long long val;
            if (_val > 0)
            {
                val = _val;
            }
            else
            {
                val = -_val;
            }
            literal lit = _literals.at(val);
            std::list<std::pair<long long, int>> positions = (lit._positives.size() > 0) ? lit._positives : lit._negatives;
            for (const auto& positives : positions)
            {
                clausesToBeRemovedSet.insert(positives.first);
            }
        }
        //sort clauses to be removed
        std::vector<long long> clausesToBeRemoved(clausesToBeRemovedSet.begin(), clausesToBeRemovedSet.end());
        std::sort(clausesToBeRemoved.begin(), clausesToBeRemoved.end(), std::greater<long long>());
        //remove
        for (const auto& idx : clausesToBeRemoved)
        {
            _clauses[idx] = _clauses.back();
            _clauses.pop_back();
        }
        _literals.clear();
        for (size_t i=0; i<_clauses.size(); ++i)
        {
            const clause &clause = _clauses.at(i);
            for (int j=0; j<clause._elements_count; j++)
            {
                long long val = clause._original_data.at(j);
                long long abs_val = std::abs(val);
                if (_literals.find(abs_val) == _literals.end())
                {
                    _literals.emplace(abs_val, literal(abs_val));
                }
                (val < 0) ? _literals.at(abs_val)._negatives.emplace_back(i,j) : _literals.at(abs_val)._positives.emplace_back(i,j);
            }
        }

        //check for pures
        _pures.clear();
        for (const auto &pair : _literals)
        {
            long long pure = pair.second.get_pure();
            if (pure != 0)
            {
                std::cout << "Pure " << pure << std::endl;
                _pures.push_back(pair.second._literal);
            }
        }
        //check for loners
        lonerLiteralsSet.clear();
        for (int i=0; i<_clauses.size(); i++)
        {
            auto cl = _clauses.at(i);
            if (cl._elements_count == 1)
            {
                //this is a loner. we need to remove negative litarals from other clauses
                lonerLiteralsSet.insert(cl._original_data.at(0));
                clausesToBeRemovedSet.insert(i);
            }
        }
        for (const auto &_loner : lonerLiteralsSet)
        {
            long long loner;
            if (_loner > 0)
            {
                loner = _loner;
            }
            else
            {
                loner = -_loner;
            }
            literal lit = _literals.at(loner);
            std::list<std::pair<long long, int>> positions = (_loner > 0) ? lit._negatives : lit._positives;
            for ( const auto &pos : positions)
            {
                clause &cl = _clauses.at(pos.first);
                cl._elements_count--;
                //cl._original_data.at(pos.second) = 0;
                //rearrange
                if (pos.second != 2)
                {
                    if (cl._elements_count != 0)
                    {
                        if (pos.second == 0)
                        {
                            if (cl._original_data.at(2) != 0)
                            {
                                cl._original_data.at(0) = cl._original_data.at(2);
                                cl._original_data.at(2) = 0;
                            }
                            else if (cl._original_data.at(1) != 0)
                            {
                                cl._original_data.at(0) = cl._original_data.at(1);
                                cl._original_data.at(1) = 0;
                            }
                            else
                            {
                                cl._original_data.at(0) = 0;
                            }
                        }
                    }
                }
                else
                {
                    cl._original_data.at(pos.second) = 0;
                }

            }
        }

        process();
    }

    void Preprocessor::print_stats()
    {
        //clauses
        std::cout << std::endl;
        std::cout << "Number of clauses - " << _clauses.size() << std::endl;

        //variables
        std::cout << "Number of variables - " << _num_literals << std::endl;
        //pure variables
        std::cout << "Number of pure variables - " << _pures.size() << std::endl;
        if (_pures.size() > 0)
        {
            std::cout << "Pure variables -" << std::endl;
            for (auto const& pure : _pures)
            {
                std::cout << pure << std::endl;
            }
        }

        //placeholders

    }

    std::vector<clause> Preprocessor::get_processed_clauses()
    {
        return _clauses;
    }

    std::map<long long int, literal> Preprocessor::get_literals_map()
    {
        return _literals;
    }


}
