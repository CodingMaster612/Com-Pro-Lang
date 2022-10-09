Statement functionCall;
        functionCall.mKind = StatementKind::FUNCTION_CALL;
        functionCall.mName = possibleFunctionName->mText;
        size_t lineNo = (mCurrentToken != mEndToken) ? mCurrentToken->mLineNumber : SIZE_MAX;

        // if (!expectOperator("("))
        // {
        //     throw runtime_error(string("Expected opening parenthesis after \"if\" on line ") + to_string(lineNo) + ".");
        // }

        if (mCurrentToken != mEndToken)
        {
            lineNo = mCurrentToken->mLineNumber;
        }
        optional<Statement> condition = expectExpression();
        if (!condition)
        {
            throw runtime_error(string("Expected loop condition after \"if\" statement on line ") + to_string(lineNo) + ".");
        }

        functionCall.mParameters.push_back(condition.value());

        if (!expectOperator(")"))
        {
            throw runtime_error(string("Expected closing parenthesis after \"()\" condition on line ") + to_string(lineNo) + ".");
        }

        if (!expectOperator("{"))
        {
            throw runtime_error(string("Expected opening curly bracket after \"(condition)\" condition on line ") + to_string(lineNo) + ".");
        }

        while (mCurrentToken != mEndToken && !expectOperator("}"))
        {
            auto currentStatement = expectStatement();
            if (!currentStatement)
            {
                break;
            }
            functionCall.mParameters.push_back(currentStatement.value());

            if (!expectOperator(";").has_value())
            {
                size_t lineNo = (mCurrentToken != mEndToken) ? mCurrentToken->mLineNumber : 999999;
                throw runtime_error(string("Expected ';' at end of statement in line ") + to_string(lineNo) + ".");
            }
        }

        return functionCall;
    }