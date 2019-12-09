/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <DragonRecipes/StringTools.h>
#include <DragonRecipes/Error.h>

namespace dragon {

class Error;

class ErrorPrivate {
  public:
      ErrorPrivate(int num = 0, std::string msg = std::string()) :
          num(num), msg(std::move(msg)) {}

    ErrorPrivate(const ErrorPrivate &errorPrivate) = default;
    ErrorPrivate(ErrorPrivate &&errorPrivate) noexcept = default;
    ErrorPrivate &operator=(const ErrorPrivate &errorPrivate) = default;
    ErrorPrivate &operator=(ErrorPrivate &&errorPrivate) noexcept = default;
    virtual ~ErrorPrivate() = default;

private:
    int num;
    std::string msg;
    static std::vector<std::string> errorNames;

    friend Error;
    friend std::string errorString(int num);
};

std::vector<std::string> ErrorPrivate::errorNames = {
    "SUCCESS",
    "TERM_OUT_OF_RANGE",
    "NONTERM_OUT_OF_RANGE",
    "UNKNOWN_ERROR"
};

Error::Error(int num, const std::string &msg) :
    data(new ErrorPrivate(num, msg)) {}

Error::Error(std::unique_ptr<ErrorPrivate> &&ptr) :
    data(std::move(ptr)) {}

Error::Error(const Error &Error) :
    data(new ErrorPrivate()) {
    *data = *Error.data;
}

Error::Error(Error &&Error) noexcept :
    data(new ErrorPrivate()) {
    std::swap(data, Error.data);
}

Error &Error::operator=(const Error &Error) {
    *data = *Error.data;
    return *this;
}

Error &Error::operator=(Error &&Error) noexcept {
    std::swap(data, Error.data);
    return *this;
}

Error::~Error() {
    data.reset(nullptr);
}

int Error::num() {
    return data->num;
}

std::string Error::name(int num) {
    return ErrorPrivate::errorNames[static_cast<size_t>(num)];
}

std::string Error::name() {
    return Error::name(data->num);
}

std::string Error::msg() {
    return data->msg;
}

void Error::print(std::ostream &os) {
    os << name() << " " << data->msg << "\n";
}

std::string Error::toString() {
    std::stringstream ss;
    print(ss);
    return ss.str();
}

std::string errorString(int num) {
    if (num < 0) {
        return "unknown error number: " + std::to_string(num) + "\n";
    }

    if (static_cast<size_t>(num) >= ErrorPrivate::errorNames.size()) {
        return "unknown error number: " + std::to_string(num) + "\n";
    }

    return ErrorPrivate::errorNames[static_cast<size_t>(num)];
}

}
