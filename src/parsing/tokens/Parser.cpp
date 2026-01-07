#include "Parser.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "utils/font.h"
#include "utils/stringutils.h"
#include "utils/utils.h"

using Utils::Font;

namespace Parsing::Tokens
{
    Parser::Parser(const std::string &file_path) : m_filePath(file_path)
    {
        // try{}
        parseDefinedTokens(file_path);
    }

    // FileParser::FileParser(const FileParser &other)
    //     : m_FilePath(other.m_FilePath),
    //       m_FileContent(other.m_FileContent)
    // {
    // }

    // Parser::Parser(Parser &&other) noexcept
    //     // : m_FilePath(std::move(other.m_FilePath)),
    //     //   m_FileContent(std::move(other.m_FileContent))
    // {
    //     m_filePath = std::move(other.m_filePath);
    // }

    bool Parser::parseDefinedTokens(const std::string& file_path)
    {
        std::ifstream file(file_path);

        if (!file.is_open())
        {
            // throw std::runtime_error("Error: file '" + file_path + "' not found");
            return false;
        }

        std::string line;

        while (std::getline(file, line))
        {
            bool ignore = false;

            if (line[0] == '#' || line.empty())
            {
                continue;
            }

            if (line[0] == '!')
            {
                ignore = true;
                line = line.substr(1);
            }

            size_t namePos = line.find_first_of("= ");
            std::string name = line.substr(0, namePos);

            size_t patternPos = line.find_first_of("= ", namePos + 1);
            while (line[patternPos] == ' ' || line[patternPos] == '=')
            {
                patternPos++;
            }

            std::string pattern = line.substr(patternPos);
            m_definedTokens.push_back({name, Regex::Regex(pattern), ignore});
            m_definedTokensMap[name] = m_definedTokens.size() - 1;
        }

        return true;
    }

    bool Parser::parseTokens(const std::string& file_path)
    {
        std::string input;
        bool debug = true;
        Utils::Stream outStr;
        
        try
        {
            input = Utils::getFileString(file_path);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }

        m_lineCounter.count(input);

        std::cout << std::endl
                  << Font::fgreen << " ==== INPUT ==== \n"
                  << Font::reset
                  << Font::forange << "'" << input << "'" << Font::reset << "\n"
                  << std::endl;


        std::string intermediate = input;
        unsigned int pos = 0;

        while (true)
        {
            if (pos >= input.size())
                break;

            outStr << " === POS[" << pos << "] === \n";
            intermediate = input.substr(pos);

            bool matched = false;
            for (auto &token : m_definedTokens)
            {
                std::string printableIntermediate = Utils::sanitizeString(intermediate);
                outStr << " - Testing token: "
                          << Font::fyellow << "'" << token.tokenName << "'" << Font::reset
                          << " on: " << Font::fcyan << "'" << printableIntermediate << "' " << Font::reset;

                if (token.regex.match(intermediate))
                {
                    outStr << Font::fgreen << "ok" << Font::reset << '\n';

                    unsigned int _maxMatch = token.regex.getMaxMatch();
                    unsigned int _numLinesBetween = m_lineCounter.numLinesInBetween(pos, pos + _maxMatch);
                    unsigned int _lineOffset = m_lineCounter.getXOffset(pos);
                    unsigned int _start = pos - _lineOffset;
                    unsigned int _end = pos + _maxMatch - _lineOffset;

                    unsigned int _line = m_lineCounter.accumulate(pos);

                    if (_numLinesBetween == 0)
                    {
                        outStr << Font::fgreen << "Matched token[" << _line + 1 << "][";
                    }
                    else
                    {
                        outStr << Font::fgreen << "Matched token[" << _line + 1 << "-" << _line + _numLinesBetween << "][";
                    }

                    outStr << _start << ", " << _end << "]: "
                              << Font::fyellow << "'" << token.tokenName << "'" << Font::reset << ": "
                              << Font::fcyan << "'" << Utils::sanitizeString(token.regex.getMatch()) << "'" << Font::reset << '\n';

                    if(!token.ignore)
                    {
                        m_tokens.push_back({pos, pos + _maxMatch, token.tokenName, Utils::sanitizeString(token.regex.getMatch()), token.ignore});
                    }
                    m_tokensFull.push_back({pos, pos + _maxMatch, token.tokenName, "" ,token.ignore});

                    pos += _maxMatch;
                    matched = true;
                    break;
                }
                else
                {
                    outStr << Font::fred << "err" << Font::reset << '\n';
                }
            }

            if (!matched)
            {
                outStr << Font::bold << Font::fred << "NO MATCH FOUND FOR: '" << intermediate << "'" << Font::reset << '\n';
                return false;
            }
        }

        outStr << Font::bold << Font::fgreen << "SUCCESS" << Font::reset << '\n';

        if(debug)
        {
            std::cout <<outStr.end();
        }
        return true;
    }

    bool Parser::isTokenDefined(const std::string& token) const
    {
        return m_definedTokensMap.contains(token);
    }

    const std::vector<Token>& Parser::getTokens() const
    {
        return m_tokens;
    }

    std::optional<std::reference_wrapper<Token>> Parser::operator[](std::size_t index)
    {
        if(index < m_tokens.size())
        {
            return m_tokens[index];
        }

        return std::optional<std::reference_wrapper<Token>>();
    }

    const std::string Parser::toString() const
    {
        Utils::Stream stream;
        stream.add(Font::fgreen, " ==== TOKENIZER ==== \n", Font::reset);
        stream.add(Font::fgreen, " 1. Input tokens \n", Font::reset);

        for (auto &token : m_definedTokens)
        {
            stream.add(
                Font::fyellow, "'", token.tokenName, "'", Font::reset, ": ", 
                Font::fcyan, token.regex.getPattern(), Font::reset
            );

            if (token.ignore)
            {
                stream.add(Font::fred, " [IGNORED]", Font::reset);
            }

            stream << '\n';
        }
        
        stream << '\n';
        stream.add(Font::fgreen, "2. Parsed tokens ---- \n", Font::reset);

        unsigned int oldLine = 0;
        for (const auto &token : m_tokens)
        {
            unsigned int linesBetween = m_lineCounter.numLinesInBetween(token.start, token.end);
            if (!token.ignore)
            {
                stream.add(token.name, "[", token.start, ",", token.end, "]", token.value, "  ");
            }
            for (unsigned int i = 0; i < linesBetween; i++)
            {
                stream << '\n';
            }
        }

        return stream.end();
    }


    void Parser::print() const
    {
        std::cout << toString() << std::endl;
    }
};