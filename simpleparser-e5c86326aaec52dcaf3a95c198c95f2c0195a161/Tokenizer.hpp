#pragma once

#include <vector>
#include <string>

namespace simpleparser {

    using namespace std;

    enum TokenType {
        WHITESPACE, // No token ever has this type.
        IDENTIFIER,
        INTEGER_LITERAL,
        DOUBLE_LITERAL,
        STRING_LITERAL,
        OPERATOR,
        STRING_ESCAPE_SEQUENCE,
        POTENTIAL_DOUBLE,
        POTENTIAL_COMMENT,
        COMMENT,
        POTENTIAL_IF,
        VARIABLE,
        
        
    };

    static const char *sTokenTypeStrings[] = {
        "WHITESPACE",
        "IDENTIFIER",
        "INTEGER_LITERAL",
        "DOUBLE_LITERAL",
        "STRING_LITERAL",
        "OPERATOR",
        "STRING_ESCAPE_SEQUENCE",
        "POTENTIAL_DOUBLE",
        "POTENTIAL_COMMENT",
        "COMMENT",
        "POTENTIAL_IF",
        "IF",
        "VARIABLE",

        
    };

    class Token {
    public:
        enum TokenType mType{WHITESPACE};
        string mText;
        size_t mLineNumber{0};

        void debugPrint() const;
    };

    class Tokenizer {
    public:
        vector<Token> parse(const string &inProgram);

    private:
        void endToken(Token &token, vector<Token> &tokens);
    };

}

