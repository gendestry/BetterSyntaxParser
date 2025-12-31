#include "matching/Matcher.h"
#include <print>
int main()
{
    Matching::Matcher matcher("tokens.txt", "syntax.txt", "test.txt");
    if(matcher.match())
    {
        std::println("Success");
    }
    else
    {
        std::println("Succ");
    }

    return 0;
}