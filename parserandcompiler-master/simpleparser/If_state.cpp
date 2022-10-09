#include "Parser.hpp"
#include <iostream>
#include <vector>
#include <string>

namespace simpleparser
{
    using namespace std;

    optional<Statement> Parser::expect_if()
    {
        Statement ifstate{"", Type{"void", VOID}, {}, StatementKind::IF};

        size_t lineNo = (mCurrentToken != mEndToken) ? mCurrentToken->mLineNumber : SIZE_MAX;
        if (!expectIdentifier("if"))
        {
            return nullopt;
        }

        if (!expectOperator("("))
        {
            throw runtime_error(string("Expected opening parenthesis after \"if\" on line ") + to_string(lineNo) + ".");
        }

        if (mCurrentToken != mEndToken)
        {
            lineNo = mCurrentToken->mLineNumber;
        }
        optional<Statement> condition = expectExpression();
        if (!condition)
        {
            throw runtime_error(string("Expected loop condition after \"if\" statement on line ") + to_string(lineNo) + ".");
        }

        ifstate.mParameters.push_back(condition.value());

        if (!expectOperator(")"))
        {
            throw runtime_error(string("Expected closing parenthesis after \"if\" condition on line ") + to_string(lineNo) + ".");
        }

        if (!expectOperator("{"))
        {
            throw runtime_error(string("Expected opening curly bracket after \"if\" condition on line ") + to_string(lineNo) + ".");
        }

        while (mCurrentToken != mEndToken && !expectOperator("}"))
        {
            auto currentStatement = expectStatement();
            if (!currentStatement)
            {
                break;
            }
            ifstate.mParameters.push_back(currentStatement.value());

            if (!expectOperator(";").has_value())
            {
                size_t lineNo = (mCurrentToken != mEndToken) ? mCurrentToken->mLineNumber : 999999;
                throw runtime_error(string("Expected ';' at end of statement in line ") + to_string(lineNo) + ".");
            }
        }

        return ifstate;
    }

    optional<Statement> Parser::expectStatement_state()
    {
        optional<Statement> result_state = expect_if();

        if (!result_state.has_value())
        {
            result_state = expectVariableDeclaration();
        }
        if (!result_state.has_value())
        {
            result_state = expectExpression();
        }

        return result_state;
    }

}