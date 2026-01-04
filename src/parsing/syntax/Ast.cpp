#include "Ast.h"
#include "utils/stringutils.h"
#include "utils/font.h"
#include <print>

namespace Parsing::Syntax
{
 
    std::vector<std::string> Ast::m_visited;

    bool Ast::hasVisited(const std::string& name) const
    {
        for(auto& s : m_visited)
        {
            if(s == name)
            {
                return true;
            }
        }

        return false;
    }

    bool Ast::terminates() const
    {
        return nodes.size() == 0;
    }

    Ast::Ast(const std::string& name)
        : name(name)
    {}

    const std::string Ast::paddedString(uint16_t currentPadding) const
    {
        Utils::Stream stream;
        
        auto getPadding = [](uint16_t p) -> std::string
        {
            if(p == 0)
            {
                return "";
            }

            Utils::Stream padding;
            for(uint8_t i = 0; i < p; i++)
            {
                padding << " ";
            }

            // padding << "-";
            return padding.end();
        };

        std::string myref;

        {
            Utils::Stream ref;
            ref << "references: ";
    
            for(auto* r : references)
            {
                ref << r->name << "  ";
            }
            myref = ref.end();
        }

        if(terminates())
        {
            stream << Utils::Font::byColorCode(160,160,160) << Utils::Font::italic
                << getPadding(currentPadding) << "'" <<  name  << "'" 
                << Utils::Font::reset << ", " << myref << "\n";
            
            return stream.end();
        }
        else
        {
            stream << Utils::Font::fmagenta
                << getPadding(currentPadding) << "'" <<  name  << "'" 
                << Utils::Font::reset << ", " << myref;
        }

        stream <<  ":\n";
        for(auto* node : nodes)
        {
            if(node->name == "temp")
            {
                stream << node->paddedString(currentPadding + 2);
            }
            else if(!hasVisited(node->name))
            {
                m_visited.push_back(node->name);
                stream << node->paddedString(currentPadding + 2);
            }
            else
            {
                stream << getPadding(currentPadding + 2) << "'" << node->name << "'" << "\n";
            }
        }

        return stream.end();
    }

    bool Ast::match(std::vector<Ast*>& tokens, std::size_t& currentIndex)
    {
        std::size_t maxMatch = 0;
        // m_visited.clear();
        for(auto* node : nodes)
        {
            std::println("Index: {}, checking node: {}", currentIndex, node->name);
            for(auto* t : tokens)
            {
                std::print("{}  ", t->name);
            }
            std::println("");
            if(node->name == tokens[currentIndex]->name)
            {
                tokens[currentIndex] = this;
                std::println("Found match: {}", node->name);
                maxMatch = currentIndex + 1;
                // currentIndex++;
                // m_parsedAst[m_currentToken] = m_syntaxParser.getNode(root->name);
                // if(node->terminated)
                // {
                    // return true;
                // }
                // return true;
            }
            else if(node->terminated)
            {
                continue;
            }
            else
            {
                std::println("Penetrating: {}", node->name);
                node->match(tokens, currentIndex);
                // for(auto* n1 : node->nodes)
                // {
                    
                // }
            }
        }
        // for(auto)
    }


    [[nodiscard]] const std::string Ast::toString() const
    {
        m_visited.push_back(name);
        std::string ret = paddedString(0);
        m_visited.clear();
        return ret;
    }
}