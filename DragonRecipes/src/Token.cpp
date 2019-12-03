/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <memory>
#include <DragonRecipes/Token.h>
#include <DragonRecipes/StringTools.h>
#include <PrivateHeaders/SymbolPrivate.h>

#define PTR TokenPrivate *ptr = dynamic_cast<TokenPrivate*>(data.get());
#define CPTR const TokenPrivate *ptr = dynamic_cast<const TokenPrivate*>(data.get());

namespace dragon
    {
    class TokenPrivate : public SymbolPrivate
    {
    public:
        TokenPrivate(const std::string &_name = "", int _id = 0, int line = 0, int col = 0) :
            SymbolPrivate(_name, _id, Symbol::term), col(col), error(0), line(line), start(0) {}

        int col;
        int error;
        std::string errorString;
        std::string lexeme;
        int line;
        int start;
    };
    
    Token::Token(const std::string &name, int id, int line, int col) :
        Symbol(new TokenPrivate(name, id, line, col)) {
    }

    Token::~Token() {
    }
      
    void Token::print(std::ostream &os) const
    {
        os << "class: Token\n";
        os << "name: " << data->name << "\n";
        os << "id: " << data->id << "\n";
        os << "type: " << typeStr() << "\n";
    }
    
    std::string Token::toString() const {
        std::stringstream ss;
        print(ss);
        return ss.str();
    }
    
    int Token::col() const {
        CPTR
        return ptr->col;
    }

    int Token::error() const {
        CPTR
        return ptr->error;
    }

    std::string Token::errorString() const {
        CPTR
        return ptr->errorString;
    }

    int Token::line() const {
        CPTR
        return ptr->line;
    }

    int Token::size() const {
        CPTR
        return static_cast<int>(ptr->lexeme.size());
    }

    int Token::start() const {
        CPTR
        return ptr->start;
    }

    void Token::setCol(int col) {
        PTR
        ptr->col = col;
    }

    void Token::setError(int error) {
        PTR
        ptr->error = error;
    }

    void Token::setErrorString(const std::string &errorString) {
        PTR
        ptr->errorString = errorString;
    }

    void Token::setLexeme(const std::string &lexeme) {
        PTR
        ptr->lexeme = lexeme;
    }

    void Token::setLine(int line) {
        PTR
        ptr->line = line;
    }

    void Token::setStart(int start) {
        PTR
        ptr->start = start;
    }

    }
