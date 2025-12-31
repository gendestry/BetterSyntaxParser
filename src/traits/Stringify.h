#pragma once
#include <string>

namespace Traits
{
    struct Stringify
    {
        [[nodiscard]] virtual const std::string toString() const = 0;
        virtual void print() const {};
    };
}