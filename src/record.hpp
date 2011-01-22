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

#ifndef _RECORD_HEADER_
#define _RECORD_HEADER_

#include <string>

class Record {
    private:
        std::string username;
        std::time_t time;
        float distance;
    public:
        Record();
        bool submit(float dist, bool internal = false);
        std::string getRecord();
        float getRecordDist() {return distance;}
        std::string getRecordName() {return username;}
        std::string getRecordDate(std::string format = "%c");
};
extern Record record;

#endif // _MAIN_HEADER_
