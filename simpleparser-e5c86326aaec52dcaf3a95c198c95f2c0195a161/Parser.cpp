#include "Parser.hpp"
#include "regex.hpp"
#include <map>
#include <regex>

#include <iostream>

namespace simpleparser
{

    using namespace std;

    struct OperatorEntry
    {
        string mName;
        size_t mPrecedence;
    };

    static map<string, OperatorEntry> sOperators{
        // precedence 0 is reserved for "no operator".
        {"=", OperatorEntry{"=", 1}},
        {"<", OperatorEntry{"<", 5}},
        {">", OperatorEntry{">", 5}},
        {"+", OperatorEntry{"+", 10}},
        {"-", OperatorEntry{"-", 10}},
        {"/", OperatorEntry{"/", 50}},
        {"*", OperatorEntry{"*", 50}},
        {"(", OperatorEntry{"(", 100}},
        {")", OperatorEntry{")", 100}},
    };

    bool Parser::expectFunctionDefinition()
    {
        vector<Token>::iterator parseStart = mCurrentToken;
        optional<Type> possibleType = expectType();
        if (possibleType.has_value())
        { // We have a type!
            optional<Token> possibleName = expectIdentifier();

            if (possibleName.has_value())
            { // We have a name!
                optional<Token> possibleOperator = expectOperator("(");

                if (possibleOperator.has_value())
                { // We have a function!

                    FunctionDefinition func;
                    func.mReturnsSomething = possibleType->mName != "void";
                    func.mName = possibleName->mText;

                    while (!expectOperator(")").has_value())
                    {
                        optional<Type> possibleParamType = expectType();
                        if (!possibleParamType.has_value())
                        {
                            throw runtime_error("Expected a type at start of argument list.");
                        }
                        optional<Token> possibleVariableName = expectIdentifier();

                        ParameterDefinition param;
                        param.mType = possibleParamType->mName;
                        if (possibleVariableName.has_value())
                        {
                            param.mName = possibleVariableName->mText;
                        }
                        func.mParameters.push_back(param);

                        if (expectOperator(")").has_value())
                        {
                            break;
                        }
                        if (!expectOperator(",").has_value())
                        {
                            throw runtime_error("Expected ',' to separate parameters or ')' to indicate end of argument list.");
                        }
                    }

                    optional<vector<Statement>> statements = parseFunctionBody();
                    if (!statements.has_value())
                    {
                        mCurrentToken = parseStart;
                        return false;
                    }
                    func.mStatements.insert(func.mStatements.begin(), statements->begin(), statements->end());

                    mFunctions[func.mName] = func;

                    return true;
                }
                else
                {
                    mCurrentToken = parseStart;
                }
            }
            else
            {
                mCurrentToken = parseStart;
            }
        }
        return false;
    }

    void Parser::parse(vector<Token> &tokens)
    {
        mEndToken = tokens.end();
        mCurrentToken = tokens.begin();

        while (mCurrentToken != mEndToken)
        {
            if (expectFunctionDefinition())
            {
            }
            else
            {
                cerr << "Unknown identifier " << mCurrentToken->mText << "." << endl;
                ++mCurrentToken;
            }
        }
    }

    optional<Token> Parser::expectIdentifier(const string &name)
    {
        if (mCurrentToken == mEndToken)
        {
            return nullopt;
        }
        if (mCurrentToken->mType != IDENTIFIER)
        {
            return nullopt;
        }
        if (!name.empty() && mCurrentToken->mText != name)
        {
            return nullopt;
        }

        Token returnToken = *mCurrentToken;
        ++mCurrentToken;
        return returnToken;
    }

    optional<Token> Parser::expectOperator(const string &name)
    {
        if (mCurrentToken == mEndToken)
        {
            return nullopt;
        }
        if (mCurrentToken->mType != OPERATOR)
        {
            return nullopt;
        }
        if (!name.empty() && mCurrentToken->mText != name)
        {
            return nullopt;
        }

        Token returnToken = *mCurrentToken;
        ++mCurrentToken;
        return returnToken;
    }

    Parser::Parser()
    {
        mTypes["void"] = Type("void", VOID);
        mTypes["int"] = Type("signed int", INT32);
        mTypes["unsigned"] = Type("unsigned int", UINT32);
        mTypes["char"] = Type("signed char", INT8);
        mTypes["uint8_t"] = Type("uint8_t", INT8);
        mTypes["double"] = Type("double", DOUBLE);
        mTypes["bool"] = Type("bool", BOOL);
        mTypes["auto"] = Type("auto", AUTO);
        mTypes["let"] = Type("let", LET);
        mTypes["string"] = Type("string", STRING);
    }

    optional<Type> Parser::expectType()
    {
        optional<Token> possibleType = expectIdentifier();
        if (!possibleType)
        {
            return nullopt;
        }

        map<string, Type>::iterator foundType = mTypes.find(possibleType->mText);
        if (foundType == mTypes.end())
        {
            --mCurrentToken;
            return nullopt;
        }

        return foundType->second;
    }

    optional<vector<Statement>> Parser::parseFunctionBody()
    {
        if (!expectOperator("{").has_value())
        {
            return nullopt;
        }

        vector<Statement> statements;

        while (!expectOperator("}").has_value())
        {
            optional<Statement> statement = expectStatement();
            if (statement.has_value())
            {
                statements.push_back(statement.value());
            }

            if (!expectOperator(";").has_value())
            {
                size_t lineNo = (mCurrentToken != mEndToken) ? mCurrentToken->mLineNumber : 999999;
                throw runtime_error(string("Expected ';' at end of statement in line ") + to_string(lineNo) + ".");
            }
        }

        return statements;
    }

    void Parser::debugPrint() const
    {
        for (auto funcPair : mFunctions)
        {
            funcPair.second.debugPrint();
        }
    }

    optional<Statement> Parser::expectOneValue()
    {
        optional<Statement> result;
        auto savedToken = mCurrentToken;

        if (mCurrentToken != mEndToken && mCurrentToken->mType == DOUBLE_LITERAL)
        {
            Statement doubleLiteralStatement;
            doubleLiteralStatement.mKind = StatementKind::LITERAL;
            doubleLiteralStatement.mName = mCurrentToken->mText;
            doubleLiteralStatement.mType = Type("double", DOUBLE);
            result = doubleLiteralStatement;
            ++mCurrentToken;
        }
        else if (mCurrentToken != mEndToken && mCurrentToken->mType == INTEGER_LITERAL)
        {
            Statement integerLiteralStatement;
            integerLiteralStatement.mKind = StatementKind::LITERAL;
            integerLiteralStatement.mName = mCurrentToken->mText;
            integerLiteralStatement.mType = Type("signed integer", INT32);
            result = integerLiteralStatement;
            ++mCurrentToken;
        }
        else if (mCurrentToken != mEndToken && mCurrentToken->mType == STRING_LITERAL)
        {
            Statement stringLiteralStatement;
            stringLiteralStatement.mKind = StatementKind::LITERAL;
            stringLiteralStatement.mName = mCurrentToken->mText;
            stringLiteralStatement.mType = Type("string", UINT8);
            result = stringLiteralStatement;
            ++mCurrentToken;
        }

        else if (expectOperator("(").has_value())
        {
            result = expectExpression();
            if (!expectOperator(")").has_value())
            {
                throw runtime_error("Unbalanced '(' in parenthesized expression.");
            }
        }
        else if (auto variableName = expectIdentifier())
        {
            if (expectOperator("("))
            {
                mCurrentToken = savedToken;
            }

            else
            {

                Statement variableNameStatement;
                variableNameStatement.mKind = StatementKind::VARIABLE_NAME;
                variableNameStatement.mName = variableName->mText;

                result = variableNameStatement;
            }
        }
        if (!result.has_value())
        {
            result = expectFunctionCall();
        }
        return result;
    }

    optional<Statement> Parser::expectVariableDeclaration()
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

        Statement statement;

        statement.mKind = StatementKind::VARIABLE_DECLARATION;
        statement.mName = possibleVariableName->mText;
        statement.mType = possibleType.value();

        if (expectOperator("=").has_value())
        {
            optional<Statement> initialValue = expectExpression();
            if (!initialValue.has_value())
            {
                throw runtime_error("Expected initial value to right of '=' in variable declaration.");
            }

            statement.mParameters.push_back(initialValue.value());
        }

        return statement;
    }

    optional<Statement> Parser::expectFunctionCall()
    {
        vector<Token>::iterator startToken = mCurrentToken;

        optional<Token> possibleFunctionName = expectIdentifier();
        if (!possibleFunctionName.has_value())
        {
            mCurrentToken = startToken;
            return nullopt;
        }

        if (!expectOperator("(").has_value())
        {
            mCurrentToken = startToken;
            return nullopt;
        }

        Statement functionCall;
        functionCall.mKind = StatementKind::FUNCTION_CALL;
        functionCall.mName = possibleFunctionName->mText;

        while (!expectOperator(")").has_value())
        {
            optional<Statement> parameter = expectExpression();
            if (!parameter.has_value())
            {
                throw runtime_error("Expected expression as parameter.");
            }
            functionCall.mParameters.push_back(parameter.value());

            if (expectOperator(")").has_value())
            {
                break;
            }
            if (!expectOperator(",").has_value())
            {
                // TODO: Check whether we still have a current token.
                throw runtime_error(string("Expected ',' to separate parameters, found '") + mCurrentToken->mText + "'.");
            }
        }

        return functionCall;
    }

    // optional<Statement> Parser::Loop()
    // {
    //     vector<Token>::iterator startToken = mCurrentToken;
    //     optional<Type> possibleType = expectType();
    //     if (!possibleType.has_value())
    //     {
    //         mCurrentToken = startToken;
    //         return nullopt;
    //     }

    //     optional<Token> possibleVariableName = expectIdentifier();
    //     if (!possibleType.has_value())
    //     {
    //         mCurrentToken = startToken;
    //         return nullopt;
    //     }

    //     Statement call;

    //     call.mKind = StatementKind::VARIABLE_NAME;
    //     call.mName = possibleVariableName->mText;
    //     call.mType = possibleType.value();

    //     if (expectIdentifier("i").has_value())
    //     {
    //         optional<Statement> initialValue = expectExpression();
    //         if (!initialValue.has_value())
    //         {
    //             throw runtime_error("Expected i value to right of 'int' in variable declaration.");
    //         }

    //         call.mParameters.push_back(initialValue.value());
    //     }

    //     return call ;
    // }

    optional<Statement> Parser::expectWhileLoop()
    {
        Statement whileLoop{"", Type{"void", VOID}, {}, StatementKind::WHILE_LOOP};

        size_t lineNo = (mCurrentToken != mEndToken) ? mCurrentToken->mLineNumber : SIZE_MAX;
        if (!expectIdentifier("while"))
        {
            return nullopt;
        }

        if (!expectOperator("("))
        {
            throw runtime_error(string("Expected opening parenthesis after \"while\" on line ") + to_string(lineNo) + ".");
        }

        if (mCurrentToken != mEndToken)
        {
            lineNo = mCurrentToken->mLineNumber;
        }
        optional<Statement> condition = expectExpression();
        if (!condition)
        {
            throw runtime_error(string("Expected loop condition after \"while\" statement on line ") + to_string(lineNo) + ".");
        }

        whileLoop.mParameters.push_back(condition.value());

        if (!expectOperator(")"))
        {
            throw runtime_error(string("Expected closing parenthesis after \"while\" condition on line ") + to_string(lineNo) + ".");
        }

        if (!expectOperator("{"))
        {
            throw runtime_error(string("Expected opening curly bracket after \"while\" condition on line ") + to_string(lineNo) + ".");
        }

        while (mCurrentToken != mEndToken && !expectOperator("}"))
        {
            auto currentStatement = expectStatement();
            if (!currentStatement)
            {
                break;
            }
            whileLoop.mParameters.push_back(currentStatement.value());

            if (!expectOperator(";").has_value())
            {
                size_t lineNo = (mCurrentToken != mEndToken) ? mCurrentToken->mLineNumber : 999999;
                throw runtime_error(string("Expected ';' at end of statement in line ") + to_string(lineNo) + ".");
            }
        }

        return whileLoop;
    }
    optional<Statement> Parser::expectForLoop()
    {
        Statement ForLoop{"", Type{"void", VOID}, {}, StatementKind::FOR_LOOP};

        size_t lineNo = (mCurrentToken != mEndToken) ? mCurrentToken->mLineNumber : SIZE_MAX;
        if (!expectIdentifier("for"))
        {
            return nullopt;
        }

        if (!expectOperator("("))
        {
            throw runtime_error(string("Expected opening parenthesis after \"for\" on line ") + to_string(lineNo) + ".");
        }

        if (mCurrentToken != mEndToken)
        {
            lineNo = mCurrentToken->mLineNumber;
        }
        optional<Statement> condition = expectExpression();
        if (!condition)
        {
            throw runtime_error(string("Expected loop condition after \"for\" statement on line ") + to_string(lineNo) + ".");
        }

        ForLoop.mParameters.push_back(condition.value());

        if (!expectOperator(")"))
        {
            throw runtime_error(string("Expected closing parenthesis after \"for\" condition on line ") + to_string(lineNo) + ".");
        }

        if (!expectOperator("{"))
        {
            throw runtime_error(string("Expected opening curly bracket after \"for\" condition on line ") + to_string(lineNo) + ".");
        }

        while (mCurrentToken != mEndToken && !expectOperator("}"))
        {
            auto currentStatement = expectStatement();
            if (!currentStatement)
            {
                break;
            }
            ForLoop.mParameters.push_back(currentStatement.value());

            if (!expectOperator(";").has_value())
            {
                size_t lineNo = (mCurrentToken != mEndToken) ? mCurrentToken->mLineNumber : 999999;
                throw runtime_error(string("Expected ';' at end of statement in line ") + to_string(lineNo) + ".");
            }
        }

        return ForLoop;
    }

    optional<Statement> Parser::expectIf()
    {
        Statement IfState{"", Type{"void", VOID}, {}, StatementKind::POTENTIAL_IF};

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

        IfState.mParameters.push_back(condition.value());

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
            IfState.mParameters.push_back(currentStatement.value());

            if (!expectOperator(";").has_value())
            {
                size_t lineNo = (mCurrentToken != mEndToken) ? mCurrentToken->mLineNumber : 999999;
                throw runtime_error(string("Expected ';' at end of statement in line ") + to_string(lineNo) + ".");
            }
        }

        return IfState;
    }
    optional<Statement> Parser::expectElseIf()
    {
        Statement Else_if_State{"", Type{"void", VOID}, {}, StatementKind::POTENTIAL_ELSE_IF};

        size_t lineNo = (mCurrentToken != mEndToken) ? mCurrentToken->mLineNumber : SIZE_MAX;
        if (!expectIdentifier("elif"))
        {
            return nullopt;
        }

        if (!expectOperator("("))
        {
            throw runtime_error(string("Expected opening parenthesis after \"elif\" on line ") + to_string(lineNo) + ".");
        }

        if (mCurrentToken != mEndToken)
        {
            lineNo = mCurrentToken->mLineNumber;
        }
        optional<Statement> condition = expectExpression();
        if (!condition)
        {
            throw runtime_error(string("Expected loop condition after \"elif\" statement on line ") + to_string(lineNo) + ".");
        }

        Else_if_State.mParameters.push_back(condition.value());

        if (!expectOperator(")"))
        {
            throw runtime_error(string("Expected closing parenthesis after \"elif\" condition on line ") + to_string(lineNo) + ".");
        }

        if (!expectOperator("{"))
        {
            throw runtime_error(string("Expected opening curly bracket after \"elif\" condition on line ") + to_string(lineNo) + ".");
        }

        while (mCurrentToken != mEndToken && !expectOperator("}"))
        {
            auto currentStatement = expectStatement();
            if (!currentStatement)
            {
                break;
            }
            Else_if_State.mParameters.push_back(currentStatement.value());

            if (!expectOperator(";").has_value())
            {
                size_t lineNo = (mCurrentToken != mEndToken) ? mCurrentToken->mLineNumber : 999999;
                throw runtime_error(string("Expected ';' at end of statement in line ") + to_string(lineNo) + ".");
            }
        }

        return Else_if_State;
    }

    optional<Statement> Parser::expectElse()
    {
        Statement ElseState{"", Type{"void", VOID}, {}, StatementKind::POTENTIAL_ELSE};

        size_t lineNo = (mCurrentToken != mEndToken) ? mCurrentToken->mLineNumber : SIZE_MAX;
        if (!expectIdentifier("else"))
        {
            return nullopt;
        }

        // if (!expectOperator("("))
        // {
        //     throw runtime_error(string("Expected opening parenthesis after \"else\" on line ") + to_string(lineNo) + ".");
        // }

        if (mCurrentToken != mEndToken)
        {
            lineNo = mCurrentToken->mLineNumber;
        }
        // optional<Statement> condition = expectExpression();
        // if (!condition)
        // {
        //     throw runtime_error(string("Expected loop condition after \"else\" statement on line ") + to_string(lineNo) + ".");
        // }

        // Else_Result.mParameters.push_back(condition.value());

        // if (!expectOperator(")"))
        // {
        //     throw runtime_error(string("Expected closing parenthesis after \"else\" condition on line ") + to_string(lineNo) + ".");
        // }

        if (!expectOperator("{"))
        {
            throw runtime_error(string("Expected opening curly bracket after \"else\" condition on line ") + to_string(lineNo) + ".");
        }

        while (mCurrentToken != mEndToken && !expectOperator("}"))
        {
            auto currentStatement = expectStatement();
            if (!currentStatement)
            {
                break;
            }
            ElseState.mParameters.push_back(currentStatement.value());

            if (!expectOperator(";").has_value())
            {
                size_t lineNo = (mCurrentToken != mEndToken) ? mCurrentToken->mLineNumber : 999999;
                throw runtime_error(string("Expected ';' at end of statement in line ") + to_string(lineNo) + ".");
            }
        }

        return ElseState;
    }

    optional<Statement> Parser::expectStatement()
    {
        optional<Statement> WhileLoops = expectWhileLoop();
        optional<Statement> ForLoops = expectForLoop();
        optional<Statement> If_Result = expectIf();
        optional<Statement> Else_Result = expectElse();
        optional<Statement> ElseIf_Result = expectElseIf();

        if (!WhileLoops.has_value())
        {
            WhileLoops = expectVariableDeclaration();
        }
        if (!WhileLoops.has_value())
        {
            WhileLoops = expectExpression();
        }
        else
        {
            return WhileLoops;
        }
        if (!ForLoops.has_value())
        {
            ForLoops = expectVariableDeclaration();
        }
        if (!ForLoops.has_value())
        {
            ForLoops = expectExpression();
        }
        else
        {
            return ForLoops;
        }

        if (!If_Result.has_value())
        {
            If_Result = expectVariableDeclaration();
        }
        if (!If_Result.has_value())
        {
            If_Result = expectExpression();
        }
        else
        {
            return If_Result;
        }

        // optional<Statement> ForLoops = expectForLoop();

        if (!Else_Result.has_value())
        {
            Else_Result = expectVariableDeclaration();
        }
        if (!Else_Result.has_value())
        {
            Else_Result = expectExpression();
        }
        else
        {

            return Else_Result;
        }

        if (!ElseIf_Result.has_value())
        {
            ElseIf_Result = expectVariableDeclaration();
        }
        if (!ElseIf_Result.has_value())
        {
            ElseIf_Result = expectExpression();
        }
        else
        {
            return ElseIf_Result;
        }
        return WhileLoops;
    }

    optional<Statement> Parser::expectExpression()
    {
        optional<Statement> lhs = expectOneValue();
        if (!lhs.has_value())
        {
            return nullopt;
        }

        while (true)
        {
            optional<Token> op = expectOperator();
            if (!op.has_value())
            {
                break;
            }
            int rhsPrecedence = operatorPrecedence(op->mText);
            if (rhsPrecedence == 0)
            {
                --mCurrentToken;
                return lhs;
            }
            optional<Statement> rhs = expectOneValue();
            if (!rhs.has_value())
            {
                --mCurrentToken;
                return lhs;
            }

            Statement *rightmostStatement = findRightmostStatement(&lhs.value(), rhsPrecedence);
            if (rightmostStatement)
            {
                Statement operatorCall;
                operatorCall.mKind = StatementKind::OPERATOR_CALL;
                operatorCall.mName = op->mText;
                operatorCall.mParameters.push_back(rightmostStatement->mParameters.at(1));
                operatorCall.mParameters.push_back(rhs.value());
                rightmostStatement->mParameters[1] = operatorCall;
            }
            else
            {
                Statement operatorCall;
                operatorCall.mKind = StatementKind::OPERATOR_CALL;
                operatorCall.mName = op->mText;
                operatorCall.mParameters.push_back(lhs.value());
                operatorCall.mParameters.push_back(rhs.value());
                lhs = operatorCall;
            }
        }

        return lhs;
    }

    Statement *Parser::findRightmostStatement(Statement *lhs, size_t rhsPrecedence)
    {
        if (lhs->mKind != StatementKind::OPERATOR_CALL)
        {
            return nullptr;
        }
        if (operatorPrecedence(lhs->mName) >= rhsPrecedence)
        {
            return nullptr;
        }

        Statement *rhs = &lhs->mParameters.at(1);
        rhs = findRightmostStatement(rhs, rhsPrecedence);
        if (rhs == nullptr)
        {
            return lhs;
        }
        return rhs;
    }

    size_t Parser::operatorPrecedence(const string &operatorName)
    {
        map<string, OperatorEntry>::iterator foundOperator = sOperators.find(operatorName);
        if (foundOperator == sOperators.end())
        {
            return 0;
        }
        return foundOperator->second.mPrecedence;
    }
}
