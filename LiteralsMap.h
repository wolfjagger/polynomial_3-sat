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

#ifndef OPENMPI_TEST_LITERALSMAP_H
#define OPENMPI_TEST_LITERALSMAP_H

#include <map>
#include <list>
#include <memory>

namespace ortho {

    struct literal
    {
        //default constructor
        literal(){}
        literal(long long lit):_literal(lit){}

        std::list<std::pair<long long, int>> _positives, _negatives;
        long long _literal;
        long long get_pure() const
        {
            if (_literal == 0)
            {
                return 0;
            }
            if (_positives.empty() && _negatives.empty())
            {
                return 0;
            }
            else
            {
                if (_positives.empty())
                    return -_literal;
                else if (_negatives.empty())
                    return _literal;
                else
                    return 0;
            }
        }
    };

    using literals_map = std::shared_ptr<std::map<long long, literal>>;
}
#endif //OPENMPI_TEST_LITERALSMAP_H
