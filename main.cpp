//
//  main.cpp
//  compiler
//
//  Created by Elias Perez on 4/5/18.
//  Copyright © 2018 CPSC 323. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "lexer.h"
#include "syntax.h"


int main(int argc, const char * argv[]) {
    
	ofstream fout;
	ifstream fin;

	Lexer lex;
	syntax* SA;
    token_type tkn;
    string current = "";
    string infilepath = "";
    string outfilepath = "";
    
    
    //Input file to read from
    cout << "Before you begin, make sure the input test file is in\nthe same folder as the .exe of this program.\n";
    cout << "----------------------------------------------------------------------\n";
    cout << "Please enter the file name and extension of the input file (input.txt).\n";
    cout << "Input: ";
    getline(cin, infilepath);
    
    //Output file to write to
    cout << "Please enter the file and extension of the output file (output.txt).\n";
    cout << "Input: ";
    getline(cin, outfilepath);
    
    
    
    fin.open(infilepath);
    fout.open(outfilepath);
    
	
    
    //Catch issue with opening file
    if (!fin)
    {
        cout << "Error. Unable to read file." << endl;
        return -1;
    }
    string token;
    int i = 1;
	int prev_size = 0;
    while (fin.peek() != '0' && !fin.eof())
    {
		prev_size = lex.tokens.size();
        getline(fin, token);
		lex.set_token(&token);
		for (int j = prev_size; j < lex.tokens.size(); j++)
		{
			if (!lex.tokens.empty())
			{
				lex.tokens[j].lineNumber = i;
			}
		}
		i++;	//comment increment
    }
	SA = new syntax(lex);
	SA->Rat18s();
    
    fin.close();
    fout.close();

	exit(0);
}
