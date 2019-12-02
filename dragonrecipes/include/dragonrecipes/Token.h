/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <memory>
#include <DragonRecipes/Constants.h>
#include <DragonRecipes/Symbol.h>

namespace dragon
{

class TokenPrivate;

    class DRAGON_EXPORT Token : public Symbol
    {
    public:
        Token(const std::string &name = "", int id = 0, int line = 0, int col = 0);
        virtual ~Token();
        
        int col() const;
        int error() const;
        std::string errorString() const;
        std::string lexeme() const;
        int line() const;
        int size() const;
        int start() const;

        void setCol(int col);
        void setError(int error);
        void setErrorString(const std::string &errorStringl);
        void setLexeme(const std::string &lexeme);
        void setLine(int line);
        void setStart(int start);

        void print(std::ostream &os) const;
        std::string toString() const;
 
    };

    typedef std::shared_ptr<Token> TokenPtr;

    inline TokenPtr newToken(const std::string &name = "", int id = 0, int line = 0, int col = 0) {
        return std::make_shared<Token>(name, id, line, col);
    }
}

#endif
