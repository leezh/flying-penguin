//  The Flying Penguin
//  Copyright (C) 2010-2011 Lee Zher Huei <lee.zh.92@gmail.com>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//  MA 02110-1301, USA.
//

#include "util.hpp"
using namespace std;

namespace util {
    sf::Color to_colour(std::string text) {
        unsigned int r = 255, g = 255, b = 255, a = 255;
        
        if (text.size() == 6 || text.size() == 8) {
            std::stringstream ss;
            
            ss << std::hex << text.substr(0, 2);
            ss >> r;
            ss.clear();
            
            ss << std::hex << text.substr(2, 2);
            ss >> g;
            ss.clear();
            
            ss << std::hex << text.substr(4, 2);
            ss >> b;
            ss.clear();
            
            if (text.size() == 8) {
                ss << std::hex << text.substr(6, 2);
                ss >> a;
            }
        }
        return sf::Color(r, g, b, a);
    }

    void tokenise(const std::string &input, std::vector<std::string> &tokens, std::string seperators) {
        string start_quotes = "\"\'";
        string end_quotes = "\"\'";
        string escapes = "\\";
        char quote_used = 0;
        char quote_end = 0;
        int level = 0;
        
        string::size_type pos, sstart;

        pos = input.find_first_not_of (seperators);
        sstart = pos;
        while (pos != string::npos && pos < input.size()) {
            pos = input.find_first_of(start_quotes + end_quotes + escapes + seperators, pos);
            if (pos == string::npos) {
                break;
            }

            if (escapes.find_first_of(input.at(pos)) != string::npos) {
                pos += 2;
                continue;
            }

            if (level != 0) {
                if (input.at(pos) == quote_end) {
                    level--;
                    pos++;
                    continue;
                }
                
                if (input.at(pos) == quote_used) {
                    level++;
                    pos++;
                    continue;
                }
            } else {
                if (start_quotes.find_first_of(input.at(pos)) != string::npos) {
                    quote_used = input.at(pos);
                    quote_end = end_quotes.at(start_quotes.find_first_of(quote_used));
                    level++;
                    pos++;
                    continue;
                }

                if (seperators.find_first_of(input.at(pos)) != string::npos) {
                    tokens.push_back(input.substr(sstart, pos - sstart));
                    pos = input.find_first_not_of(seperators, pos);
                    sstart = pos;
                    continue;
                }
            }
            pos++;
        }
        if (sstart != string::npos) {
            tokens.push_back(input.substr(sstart));
        }
    }
}
