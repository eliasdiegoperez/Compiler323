#include "Syntax.h"

int main() {
    
    Lexer lex;
    Syntax syn;
    vector<TokenType> tokens;                   
    string current = "";
    string infilepath;
    string outfilepath;
    int lineNumber = 0;
    
    cout << "Enter the source code file name: ";
    cin >> infilepath;
    
    cout << "Enter the output file: ";
    cin >> outfilepath;
    
    fin.open(infilepath);
    fout.open(outfilepath);
    
    if (!fin) {
        cout << "Error. Unable to read file." << endl;
        return -1;
    }
    
    while (getline(fin, current)) {
        
        lineNumber++;
        tokens = lex.lexical(current, lineNumber);
        for (int i = 0; i < tokens.size(); i++) {
            tokens[i].linenum = lineNumber;
        }
        
        tokenList.insert(tokenList.end(), tokens.begin(), tokens.end());
        tokens.clear();
    }
    
    fin.close();
    syn.Rat18S();
    fout.close();
    cout << "Completed.\n";
    
    return 0;
}
