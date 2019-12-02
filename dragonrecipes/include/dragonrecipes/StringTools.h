/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/

#ifndef STRING_TOOLS_h
#define STRING_TOOLS_h

#include <string>
#include <vector>
#include <sstream>

#include <DragonRecipes/Constants.h>

namespace dragon {

inline std::string trim(const std::string &str) {
    if (!str.size())
        return str;
    
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
        return "";
    
    size_t stop = str.find_last_not_of(" \t\r\n");
    size_t len = stop - start + 1;
    
    if (len == str.size())
        return str;
    
    return str.substr(start, len);
}

inline void split(const std::string &str,
                  const std::string &delim,
                  std::vector<std::string> &vec) {
    
    vec.clear();
    
    if (!str.size())
        return;

    size_t tokenStart = 0;
    size_t delimStart = 0;

    while(1) {
        tokenStart = str.find_first_not_of(delim, delimStart);

        if (tokenStart == std::string::npos)
            break;
        
        delimStart = str.find_first_of(delim, tokenStart);
        if (delimStart == std::string::npos) {
            vec.push_back(str.substr(tokenStart));
            break;
        }
        
        size_t len = delimStart - tokenStart;
        vec.push_back(str.substr(tokenStart, len));
    }
}

inline std::string join(const std::vector<std::string> &vec,
                        const std::string &delim) {
    std::stringstream ss;
    for(size_t i = 0; i < vec.size(); ++i) {
        if (i > 0)
            ss << delim;
        ss << vec[i];
    }
    
    return ss.str();
}

}

#endif
