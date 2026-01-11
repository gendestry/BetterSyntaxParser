#pragma once
#include "parsing/tokens/Token.h"
#include "traits/Stringify.h"
#include "utils/stringutils.h"
#include <vector>
#include <cstdint>
#include <memory>
#include "utils/logger.h"

namespace Matching
{

    struct Ast : public Traits::Stringify
    {
        mutable Utils::Logger logger;
        std::string name;
        std::shared_ptr<Ast> parent;
        std::vector<std::shared_ptr<Ast>> nodes;
        uint32_t tokenIndex = 0;
        uint32_t numTokens = 0;
        bool custom_token = false;

        Ast()
            : logger("MATCHER")
        {}

        Ast(std::string name)
            : name(name), logger("MATCHER")
        {}

        virtual const std::string paddedToString(uint8_t padding) const
        {
            Utils::Stream stream;
            std::string pad = Utils::Stream::pad(padding, "  ");
    
            if(parent)
            {
                stream << logger.scopeStr() << pad << name << "(" << parent->name << "): \n";
            }
            else
            {
                stream << logger.scopeStr() << pad << name << ": \n";
            }

            std::string newline = (nodes.size() > 1) ? "\n" : "";
            for(uint32_t i = 0; i < nodes.size(); i++)
            {
                std::shared_ptr<Ast> node = nodes[i];
                stream << node->paddedToString(padding + 1);

                if(i < nodes.size() - 1)
                {
                    stream << newline;
                }
            }

            return stream.end();
        }

        const std::string toString() const override
        {
            return paddedToString(0) + "\n";
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
            std::string pad = Utils::Stream::pad(padding, "  ");

            if(parent)
            {
                stream << logger.scopeStr() << pad << name << "(" << parent->name << "): ";
            }
            else
            {
                stream << logger.scopeStr() << pad << name << ": ";
            }
            
            stream << Utils::Font::fmagenta << "'" << token.value << "'" << Utils::Font::reset;
            return stream.end();
        }
    };

}