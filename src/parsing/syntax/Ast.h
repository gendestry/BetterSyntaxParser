#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include "traits/Stringify.h"
#include <stack>

namespace Parsing::Syntax
{
    class Ast : public Traits::Stringify
    {
        const std::string paddedString(uint16_t currentPadding = 0) const;
        static std::vector<std::string> m_visited;

        static std::stack<Ast*> m_tokens;

        bool hasVisited(const std::string& name) const;
        bool terminates() const;
        bool matchNode(Ast* node, std::vector<Ast*>& tokens, std::size_t& currentIndex);

    public:
        std::string name;
        std::size_t index;
        std::vector<Ast*> nodes;
        std::vector<Ast*> references;

        bool terminated = false;
        void clearVisisted() { m_visited.clear(); }
        Ast() = default;
        Ast(const std::string& name);

        bool match(std::vector<Ast*>& tokens, std::size_t& currentIndex);
        [[nodiscard]] const std::string toString() const override;
    };
}