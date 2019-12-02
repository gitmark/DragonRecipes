/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/

#include <iostream>
#include <DragonRecipes.h>

using namespace dragon;

int main(int argc, const char *argv[]) {
    Production p("a", "b");
    p.print(std::cout);
    std::cout << "\n";
	std::cout << "hello\n";
	return 0;
}


