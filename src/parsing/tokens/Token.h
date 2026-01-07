#pragma once
#include <string>

namespace Parsing::Tokens
{
    struct Token// : public Traits::Stringify
    {
        unsigned int start, end;
        std::string name;
        std::string value;
        bool ignore = false;

        // Token(unsigned int start, unsigned int end, bool ignore)
        //     : start(start), end(end), ignore(ignore)
        // {}

        // [[nodiscard]] const std::string toString() const override
        // {
        //     return "TOKENT";   
        // }
    };

}