#include <iostream>

#include "src/colorconsole.hpp"

using namespace qyvlik;


int main()
{
    ColorConsole log;

    log.print("[here is red]", ColorConsole::Red);
    log.print("[here is white]");

    return 0;
}


