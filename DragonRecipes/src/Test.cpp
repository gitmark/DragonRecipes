#include <DragonRecipes/Test.h>
#include <DragonRecipes/Production.h>
#include <DragonRecipes/Grammar.h>

namespace dragon {

int test(std::ostream &os) {

    ProdPtr prod = newProduction("a", "b c");
    std::vector<TokenPtr> terminals;
    GrammarPtr grammar = newGrammar();

    std::vector<std::string> nonterms = grammar->nontermVec();
    std::vector<std::string> terms = grammar->termVec();

    return 0;
}

}
