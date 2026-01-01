#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

#include "regex/regex.h"
#include "utils/lineCounter.h"

#include "traits/Stringify.h"

namespace Parsing::Tokens
{

    struct DefinedToken
    {
        std::string tokenName;
        Regex::Regex regex;
        bool ignore = false;
    };

    struct Token// : public Traits::Stringify
    {
        unsigned int start, end;
        std::string name;
        bool ignore = false;

        // Token(unsigned int start, unsigned int end, bool ignore)
        //     : start(start), end(end), ignore(ignore)
        // {}

        // [[nodiscard]] const std::string toString() const override
        // {
        //     return "TOKENT";   
        // }
    };

    class Parser : public Traits::Stringify
    {
        std::string m_filePath;
        Utils::LineCounter m_lineCounter;

        std::vector<DefinedToken> m_definedTokens;
        std::unordered_map<std::string, std::size_t> m_definedTokensMap;

        std::vector<Token> m_tokens;
        std::vector<Token> m_tokensFull;

    public:
        Parser() {}
        Parser(const std::string &file_path);
        // FileParser(const FileParser &other);
        // Parser(Parser &&other) noexcept;
        
        bool parseDefinedTokens(const std::string &file_path);
        bool parseTokens(const std::string &file_path);

        [[nodiscard]] bool isTokenDefined(const std::string& token) const;
        [[nodiscard]] const std::vector<Token>& getTokens() const;
        [[nodiscard]] std::optional<std::reference_wrapper<Token>> operator[](std::size_t index);

        [[nodiscard]] const std::string toString() const override;
        void print() const override;

        // MappedToken& operator[](std::string name)
        // {

        // };

        // inline TokenMap &getTokenMaps()
        // {
        //     return tokenMaps;
        // }
    };
};