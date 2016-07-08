#include <iostream>

#include "src/colorconsole.hpp"

using namespace qyvlik;


int main()
{
    ColorConsole log;

    log.setFontColor(ColorConsole::Red);
    log.print("[here is red]");
    log.print("[here is white]");

    return 0;
}


