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
#include "Utilities.h"


bool ortho::string_starts_with(const std::string &str, const std::string &pattern)
{
    return (str.find(pattern) == 0);
}

std::vector<std::string> ortho::tokenize(const std::string &_line, char delimiter)
{
    std::string line = _line + "\n";
    std::vector<std::string> tokens;
    if (_line == "\n")
    {
        return tokens;
    }
    int pos=0;
    int prev=0;
    bool found = false;
    while (pos < line.size())
    {
        int val = line.at(pos);
        if (line.at(pos) > 32 && line.at(pos) < 127 )
        {

            if (!found)
            {
                found = true;
                prev = pos;
            }
        }
        else if (line.at(pos) <= 32 || line.at(pos) >= 127 )
        {
            if (found)
            {
                tokens.push_back(line.substr(prev, (pos- prev)));
                found = false;
            }
        }
        pos++;
    }
    /*std::string token;
    std::istringstream tokenStream(line);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }*/
    return tokens;
}

std::string ortho::trim(const std::string &str)
{
    //find first and last alphanumeric char
    int first=0, last=str.size()-1;
    for (int i=0; i<str.size(); i++)
    {
        if (str.at(i) > 32 && str.at(i) < 127 )
        {
            first = i;
            break;
        }
    }
    for (int i=str.size()-1; i>=0; i--)
    {
        if (str.at(i) > 32 && str.at(i) < 127 )
        {
            last = i;
            break;
        }
    }

    return str.substr(first, (last - first) +1);
}