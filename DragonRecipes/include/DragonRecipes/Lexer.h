#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <memory>
#include <DragonRecipes/Token.h>
#include <DragonRecipes/Constants.h>

namespace dragon {

DRAGON_EXPORT int groupCount(const std::string& str, int &count);

class LexerPrivate;

class DRAGON_EXPORT Lexer {
public:
    Lexer ();
    virtual ~Lexer();
    TokenPtr token();
    bool next();
    void reset();
    int addTerminal(const std::string &symbol, int id, const std::string &pattern);
    int setSource(const std::string &src);
    int compileRegex();
    void clear();
    std::string compositeRegex();

private:
    std::unique_ptr<LexerPrivate> data;
};

using LexerPtr = std::shared_ptr<Lexer>;

inline LexerPtr newLexer() {
    return std::make_shared<Lexer>();
}

}

#endif
