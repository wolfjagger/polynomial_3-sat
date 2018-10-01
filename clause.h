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
// Created by Asanka Wickramasinghe on 4/21/18.
// Copyright (c) 2018 Asanka & Ortho. All rights reserved.
//

#ifndef OPENMPI_TEST_CLAUSE_H
#define OPENMPI_TEST_CLAUSE_H

#include <array>
#include <vector>
#include <algorithm>
#include <memory>

namespace ortho {
    struct clause {
        clause(long long val1, long long val2, long long val3)
        {
            _data[0] = val1;
            _data[1] = val2;
            _data[2] = val3;
            _original_data[0] = val1;
            _original_data[1] = val2;
            _original_data[2] = val3;
            _elements_count = 3 - std::count(_original_data.begin(), _original_data.end(), 0);
            std::sort(_data.begin(), _data.end());
        }

        bool operator<(const clause &cl) const
        {
            std::array<long long, 3> tmp = _data;
            std::sort(tmp.begin(), tmp.end());
            if (_data[0] < cl._data[0])
                return true;
            if (_data[0] == cl._data[0] && _data[1] < cl._data[1])
                return true;
            return (_data[0] == cl._data[0] && _data[1] == cl._data[1] && _data[2] < cl._data[2]);
        }
        long long first(){ return _data[0]; }
        long long second(){ return _data[1]; }
        long long third(){ return _data[2]; }

        bool is_quantum() const
        {
            if (_data[1] == -_data[0] || _data[2] == -_data[0])
            {
                return true;
            }
            else if (_data[2] == -_data[1])
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        std::array<long long, 3> _data, _original_data;
        int _elements_count;
        friend std::ostream& operator<<(std::ostream& os, const clause& cl)
        {
            std::string out = "( ";
            for (int i=0; i< cl._elements_count; i++)
            {
                out = out + std::to_string(cl._original_data[i]) +" ";
            }
            out = out + ")";
            os << out;
            return os;
        };

    };

    using ordered_clauses = std::shared_ptr<std::vector<clause>>;
};

#endif //OPENMPI_TEST_CLAUSE_H
