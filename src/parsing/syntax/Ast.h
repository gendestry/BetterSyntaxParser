#pragma once
#include <vector>
#include <string>
#include "traits/Stringify.h"
#include "utils/stringutils.h"

namespace Parsing::Syntax
{
    struct Ast : public Traits::Stringify
    {
        std::string name;
        std::size_t index;
        std::vector<Ast*> nodes;

        Ast() = default;
        Ast(const std::string& name)
            : name(name)
        {}

        // Ast(Ast&& other)
        // {
        //     name = "expr";
        //     index = other.index;
        //     nodes = other.nodes;
        // };

        [[nodiscard]] const std::string toString() const override
        {
            Utils::Stream stream;
            stream.add(name, "\n  children:\n");
            for(auto* node : nodes)
            {
                stream << "  " << node->toString() << '\n';
            }

            return stream.end();
        }

        
    };

}