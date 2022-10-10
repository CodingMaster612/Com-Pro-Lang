#include "Parser.hpp"


#include <iostream>

namespace simpleparser
{

    using namespace std;

    optional<Statement> Parser::expectLoopStatement()
    {
        vector<Token>::iterator startToken = mCurrentToken;
        optional<Type> possibleType = expectType();
        if (!possibleType.has_value())
        {
            mCurrentToken = startToken;
            return nullopt;
        }

        optional<Token> possibleVariableName = expectIdentifier();
        if (!possibleType.has_value())
        {
            mCurrentToken = startToken;
            return nullopt;
        }

        Statement loop;

        loop.mKind = StatementKind::FOR_LOOP;
        loop.mName = possibleVariableName->mText;
        loop.mType = possibleType.value();

        if(expectIdentifier("if").has_value()){
            optional<Statement> Value = expectExpression();
            if (!Value.has_value()) {
                throw runtime_error("expected if statement.");
            }
        loop.mParameters.push_back(Value.value());
        }

        return loop;
    }

}