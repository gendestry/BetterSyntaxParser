#include "Parser.h"
#include <fstream>
#include <iostream>

#include "utils/font.h"
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

            PRINT(std::cout << " === POS[" << pos << "] === " << std::endl;)
            intermediate = input.substr(pos);

            bool matched = false;
            for (auto &token : m_definedTokens)
            {
                std::string printableIntermediate = Utils::sanitizeString(intermediate);
                std::cout << " - Testing token: "
                          << Font::fyellow << "'" << token.tokenName << "'" << Font::reset
                          << " on: " << Font::fcyan << "'" << printableIntermediate << "' " << Font::reset;

                if (token.regex.match(intermediate))
                {
                    std::cout << Font::fgreen << "ok" << Font::reset << std::endl;

                    unsigned int _maxMatch = token.regex.getMaxMatch();
                    unsigned int _numLinesBetween = m_lineCounter.numLinesInBetween(pos, pos + _maxMatch);
                    unsigned int _lineOffset = m_lineCounter.getXOffset(pos);
                    unsigned int _start = pos - _lineOffset;
                    unsigned int _end = pos + _maxMatch - _lineOffset;

                    unsigned int _line = m_lineCounter.accumulate(pos);

                    if (_numLinesBetween == 0)
                    {
                        std::cout << Font::fgreen << "Matched token[" << _line + 1 << "][";
                    }
                    else
                    {
                        std::cout << Font::fgreen << "Matched token[" << _line + 1 << "-" << _line + _numLinesBetween << "][";
                    }

                    std::cout << _start << ", " << _end << "]: "
                              << Font::fyellow << "'" << token.tokenName << "'" << Font::reset << ": "
                              << Font::fcyan << "'" << Utils::sanitizeString(token.regex.getMatch()) << "'" << Font::reset << std::endl;

                    if(!token.ignore)
                    {
                        m_tokens.push_back({pos, pos + _maxMatch, token.tokenName, token.ignore});
                    }
                    m_tokensFull.push_back({pos, pos + _maxMatch, token.tokenName, token.ignore});

                    pos += _maxMatch;
                    matched = true;
                    break;
                }
                else
                {
                    std::cout << Font::fred << "err" << Font::reset << std::endl;
                }
            }

            if (!matched)
            {
                std::cout << Font::bold << Font::fred << "NO MATCH FOUND FOR: '" << intermediate << "'" << Font::reset << std::endl;
                return false;
            }
        }

        std::cout << Font::bold << Font::fgreen << "SUCCESS" << Font::reset << std::endl;

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

    void Parser::printDefinedTokens() const
    {
        std::cout << Font::bold << Font::fgreen << " ==== INPUT TOKENS ==== " << Font::reset << std::endl;
        for (auto &token : m_definedTokens)
        {
            std::cout << Font::fyellow << "'" << token.tokenName << "'" << Font::reset << ": "
                      << Font::fcyan << token.regex.getPattern() << Font::reset;

            if (token.ignore)
            {
                std::cout << Font::fred << " [IGNORED]" << Font::reset;
            }

            std::cout << std::endl;
        }
    }

    void Parser::printTokens() const
    {
        std::cout << "\n"
                  << Font::fgreen << " ==== PARSED TOKENS ==== \n"
                  << Font::reset;

        unsigned int oldLine = 0;
        for (const auto &token : m_tokens)
        {
            unsigned int linesBetween = m_lineCounter.numLinesInBetween(token.start, token.end);
            if (!token.ignore)
            {
                std::cout << token.name << "[" << token.start << "," << token.end << "] ";
            }
            for (unsigned int i = 0; i < linesBetween; i++)
            {
                std::cout << std::endl;
            }
        }

        std::cout << std::endl;
    }
};