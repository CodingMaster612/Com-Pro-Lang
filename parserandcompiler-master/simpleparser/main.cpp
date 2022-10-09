#include "Tokenizer.cpp"
#include "Tokenizer.hpp"
#include "Parser.cpp"
#include "Parser.hpp"
#include "FunctionDefinition.cpp"
#include "FunctionDefinition.hpp"
#include "Type.hpp"
#include "Statement.cpp"
#include "Statement.hpp"


#include <iostream>

using namespace std;
using namespace simpleparser;

int main()
{
    try
    {
        std::cout << "simpleparser 0.1\n"
                  << endl;

        FILE *fh = fopen("C:\\Users\\Tim\\Documents\\GitHub\\Com-Pro-Lang\\parserandcompiler-master\\simpleparser\\FunctionTest.myc", "r");
        if (!fh)
        {
            cerr << "Can't find file." << endl;
        }
        fseek(fh, 0, SEEK_END);
        size_t fileSize = ftell(fh);
        fseek(fh, 0, SEEK_SET);
        string fileContents(fileSize, ' ');
        fread(fileContents.data(), 1, fileSize, fh);

        cout << fileContents << endl
             << endl;

        Tokenizer tokenizer;
        vector<Token> tokens = tokenizer.parse(fileContents);

        for (Token currToken : tokens)
        {
            currToken.debugPrint();
        }

        Parser parser;
        parser.parse(tokens);

        parser.debugPrint();
    }
    catch (exception &err)
    {
        cerr << "Error: " << err.what() << endl;
        return 2;
    }
    catch (...)
    {
        cerr << "Unknown Error." << endl;
        return 1;
    }

    return 0;
}
