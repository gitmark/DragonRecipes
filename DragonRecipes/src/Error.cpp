/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <DragonRecipes/StringTools.h>
#include <DragonRecipes/Error.h>

#define ERROR_NAME_COUNT 4

namespace dragon
    {

    class ErrorPrivate
    {
    public:
        ErrorPrivate(int num, const std::string &msg) :
        num(num), msg(msg) {}
        
        int num;
        std::string msg;
        static const char *errorNames[ERROR_NAME_COUNT];
    };
    
    const char *ErrorPrivate::errorNames[ERROR_NAME_COUNT] = {
    "SUCCESS",
    "TERM_OUT_OF_RANGE",
    "NONTERM_OUT_OF_RANGE",
    "UNKNOWN_SYMBOL"
    };

    Error::Error(int num, const std::string &msg) :
    data(new ErrorPrivate(num, msg)) {}
    
    Error::~Error() {}

    int Error::num() {
        return data->num;
    }

    std::string Error::name(int num) {
        return ErrorPrivate::errorNames[num];
    }

    std::string Error::name() {
        return Error::name(data->num);
    }

    std::string Error::msg() {
        return data->msg;
    }

    void Error::print(std::ostream &os)
    {
        os << name() << " " << data->msg << "\n";
    }
    
    std::string Error::toString() {
        std::stringstream ss;
        print(ss);
        return ss.str();
    }
        
    }
