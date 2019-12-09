/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef ERROR_H
#define ERROR_H

#include <vector>
#include <string>
#include <memory>
#include <DragonRecipes/Constants.h>

namespace dragon {

class ErrorPrivate;
constexpr int E_SUCCESS = 0;
constexpr int E_TERM_OUT_OF_RANGE = 1;
constexpr int E_NONTERM_OUT_OF_RANGE = 2;
constexpr int E_UNKNOWN_Error = 3;

class DRAGON_EXPORT Error {
  public:
    Error(int num, const std::string &msg);
    Error(std::unique_ptr<ErrorPrivate> &&ptr);
    Error(const Error &Error);
    Error(Error &&Error) noexcept;
    Error &operator=(const Error &Error);
    Error &operator=(Error &&Error) noexcept;

    virtual ~Error();

    int num();
    static std::string name(int num);
    std::string name();
    std::string msg();

    void print(std::ostream &os);
    std::string toString();

  private:
    std::unique_ptr<ErrorPrivate> data;
};

using ErrorPtr = std::shared_ptr<Error>;

inline ErrorPtr newError(int num, const std::string &msg) {
    return std::make_shared<Error>(num, msg);
}

DRAGON_EXPORT std::string errorString(int num);
}


#endif
