#include <DragonRecipes/Token.h>
#include <DragonRecipes/Lexer.h>
#include <map>
#include <string>
#include <regex>
#include <sstream>

#define GRP_NUM 2
#define GRP_WS 3
#define GRP_OP 4
#define GRP_ID 5

namespace dragon {

int groupCount(const std::string& str, int &count) {
    count = 0;
    int depth = 0;
    bool escape = false;
    bool charSet = false;

    for (char c : str) {
        if (escape) {
            escape = false;
            continue;
        } else if (c == '\\') {
            escape = true;
            continue;
        }

        if (charSet) {
            if (c == ']')
                charSet = false;
            continue;
        } else if (c == '[') {
            charSet = true;
            continue;
        }

        if (c == '(') {
            ++count;
            ++depth;
        } else if (c == ')') {
            --depth;
        }
    }

    return depth;
}

class LexerPrivate {
public:

    virtual ~LexerPrivate();

    // numbers, spaces, operators, parens
    LexerPrivate () :
        pos(0),
        index(0) {
    }

    int setSource (const std::string &str) {
        text = str;
        return reset();
    }

    int reset () {
        remaining = text;
        pos = 0;
        index = 0;
        return 0;
    }

    int addTerminal(const std::string &symbol, int id, const std::string &pattern) {
        regexCompiled = false;
        regexVec.push_back(pattern);
        int groupNum = currentGroupCount + 1;
        groupNumToId[groupNum] = id;
        idToSymbol[id] = symbol;
        int count = 0;
        int depth = groupCount(pattern, count);
        currentGroupCount += count + 1;
        return depth;
    }

    void clear() {
        regexVec.clear();
        groupNumToId.clear();
        currentGroupCount = 1;
    }

    int compileRegex() {
        std::stringstream ss;
        ss << "^(";

        bool first = true;
        for (auto r : regexVec) {
            if (first)
                first = false;
            else
                ss << "|";

            ss << "(" << r << ")";
        }

        ss << ")";

        compositeRegex = ss.str();
        re = std::regex(compositeRegex);

        regexCompiled = true;
        return 0;
    }

    std::string text;
    std::string remaining;
    size_t pos;
    std::regex re;
    int index;
    TokenPtr token;

    std::vector<std::string> regexVec;
    std::map<int,int> groupNumToId;
    std::map<int,std::string> idToSymbol;

    int currentGroupCount{1};
    std::string compositeRegex;
    bool regexCompiled{false};
};

LexerPrivate::~LexerPrivate() {

}

Lexer::~Lexer() {
    data.reset(nullptr);
}

Lexer::Lexer () :
    data(std::make_unique<LexerPrivate>()) {
    }

    TokenPtr Lexer::token() {
        return data->token;
    }

    bool Lexer::next() {

        if (!data->regexCompiled)
            compileRegex();

        data->token = newToken("$");
        std::smatch m;
        bool found = false;
        bool done = false;
        while(!done) {
            found = false;
            if (std::regex_search(data->remaining, m, data->re)) {
                //    if(m.prefix.)
                int i = 0;
                for(auto item :m) {
                    if ((i > 1) && item.matched) {
                        unsigned long len = static_cast<unsigned long>(item.length());
                        data->pos += len;
                        std::string lexeme = data->remaining.substr(0, len);
                        data->remaining = data->remaining.substr(len);
                        int id = data->groupNumToId[i];
                        std::string symbol = data->idToSymbol[id];
                        if (symbol.empty())
                            symbol = lexeme;
                        data->token = newToken(symbol, id, lexeme, 0, 0);
                        found = true;
                        // If whitespace, keep looking
                        if (data->token->id() != 2) {
                            done = true;
                        }
                    }
                    ++i;
                }
            }

            if (!found)
                done = true;
        }

        return found;
    }

    void Lexer::reset() {
        data->reset();
    }

    int Lexer::addTerminal(const std::string &symbol, int id, const std::string &pattern) {
        return data->addTerminal(symbol, id, pattern);
    }

    int Lexer::setSource(const std::string &str) {
        return data->setSource(str);
    }

    int Lexer::compileRegex() {
        return data->compileRegex();
    }

    void Lexer::clear() {
        data->clear();
    }

    std::string Lexer::compositeRegex() {
        if (!data->regexCompiled)
            compileRegex();
        return data->compositeRegex;
    }


    }

