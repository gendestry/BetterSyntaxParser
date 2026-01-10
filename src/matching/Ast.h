#pragma once
#include "parsing/tokens/Token.h"
#include "traits/Stringify.h"
#include "utils/stringutils.h"
#include <vector>
#include <cstdint>

namespace Matching
{

    struct Ast : public Traits::Stringify
    {
        std::string name;
        std::vector<Ast*> nodes;
        uint32_t tokenIndex = 0;
        uint32_t numTokens = 0;
        bool custom_token = false;

        Ast(){}

        Ast(std::string name)
            : name(name)
        {}



        ~Ast()
        {
            for(Ast* node : nodes)
            {
                delete node;
            }
        }

        virtual const std::string paddedToString(uint8_t padding) const
        {
            Utils::Stream stream;
            std::string pad;
            {
                Utils::Stream paddingS;
                for(uint8_t i = 0; i < padding; i++)
                {
                    paddingS << "  ";
                }

                pad = paddingS.end();
            }
            stream << pad << name << ": \n";
            std::string newline = (nodes.size() > 1) ? "\n" : "";
            for(Ast* node : nodes)
            {
                stream << node->paddedToString(padding + 1) << newline;
            }

            return stream.end();
        }

        const std::string toString() const override
        {
            return paddedToString(0);
        }
    };

    struct AstToken : public Ast
    {
        AstToken(const Parsing::Tokens::Token& token)
            : token(token)
        {
            name = token.name;
        }
        Parsing::Tokens::Token token;

        virtual const std::string paddedToString(uint8_t padding) const override
        {
            Utils::Stream stream;
            std::string pad;
            {
                Utils::Stream paddingS;
                for(uint8_t i = 0; i < padding; i++)
                {
                    paddingS << "  ";
                }

                pad = paddingS.end();
            }
            stream << pad << name << ": '" << token.value << "'";
            return stream.end();
        }
    };

}