#include "Syntax.h"



int main() {
    
    Lexer lex;
    Syntax syn;
    vector<TokenType> tokens;                     //vector to hold tokens as they are being inputted
    string current = "";
    string infilepath = "fin.txt";
    string outfilepath = "fout.txt";
    int lineNumber = 0;
    
    
    
    //Input file to read from
    //cout << "Enter the source code file name: ";
    //cin >> infilepath;
    
    //Output file to write to
    //cout << "Enter the output file: ";
    //cin >> outfilepath;
    
    
    
    fin.open(infilepath);
    fout.open(outfilepath);
    
    
    //Catch issue with opening file
    if (!fin) {
        cout << "Error. Cannot read file." << endl;
        return -1;
    }
    
    //While not end of file, read every line.
    while (getline(fin, current)) {
        
        lineNumber++;
        tokens = lex.lexical(current, lineNumber);
        for (int i = 0; i < tokens.size(); i++) {
            tokens[i].linenum = lineNumber;
            //fout << tokens[i].lexeme << " - " << tokens[i].token << endl;
        }
        
        tokenList.insert(tokenList.end(), tokens.begin(), tokens.end());
        tokens.clear();
    }
    
    fin.close();
    
    
    
    syn.Rat18S();
    fout.close();
    cout << "Completed" << endl;
    
    return 0;
}
