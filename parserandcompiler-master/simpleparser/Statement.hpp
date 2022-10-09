#pragma once

#include <string>
#include <vector>
#include "Type.hpp"

namespace simpleparser {

    using namespace std;

    enum class StatementKind: int {
        VARIABLE_DECLARATION,
        FUNCTION_CALL,
        LITERAL,
        OPERATOR_CALL,
        VARIABLE_NAME,
        WHILE_LOOP,
        IF,
        
    
       
    };

    static const char* sStatementKindStrings[] = {
        "VARIABLE_DECLARATION",
        "FUNCTION_CALL",
        "LITERAL",
        "OPERATOR_CALL",
        "VARIABLE_NAME",
        "WHILE_LOOP",
        "IF",
        
        
        //create something like function call for if statements
        
    };

    class Statement {
    public:
        string mName;
        Type mType{Type("void", VOID)};
        vector<Statement> mParameters;
        StatementKind mKind{StatementKind::FUNCTION_CALL};
        

        void debugPrint(size_t indent);
    };
}
