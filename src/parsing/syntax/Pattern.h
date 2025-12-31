#pragma once
#include <vector>
#include <string>
#include "traits/Stringify.h"

namespace Parsing::Syntax
{
    class Pattern : public Traits::Stringify
    {
        
        void parseTokens(const std::string& pattern);
    public:
        struct Token : public Traits::Stringify
        {
            std::string token;
            bool modifier;

            Token(std::string name, bool mod)
                : token(name), modifier(mod){}

            [[nodiscard]] const std::string toString() const override
            {
                return token;
            }

        };
        
        Pattern(const std::string& pattern);
        
        [[nodiscard]] const std::vector<Token>& tokens() const {return m_tokens;};
        [[nodiscard]] const std::string toString() const override;
    private:
        std::vector<Token> m_tokens;
    };
}