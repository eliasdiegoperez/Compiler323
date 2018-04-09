//
//  lexer.h
//  compiler
//
//  Created by Elias Perez on 4/5/18.
//  Copyright © 2018 CPSC 323. All rights reserved.
//

#ifndef lexer_h
#define lexer_h


#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

//ostream fout;

struct token_type {
    string token;
    string lexeme;
    int lineNumber = 0;
};

class Lexer
{
public:
    //Function prototypes
    Lexer();
    token_type iskeyword(string* n);
    token_type isIdentifier(string* n);		//uses a FSM
    token_type isInteger(string* n);
    token_type isReal(string* n);			//Uses a FSM
    token_type isSeperator(string* n);
    token_type isOperator(string* n);
    void comp_tok(string* n);				//exists if there is a token without any whitespace;
    void set_token(string* n);				//splits up the token
    token_type lexical(string* n);
    vector<token_type> tokens;
private:
    const char* keyword[14] = { "if", "while", "get", "for", "else" ,"int", "put", "endif", "return", "real", "boolean", "true", "false", "function" };
    const char* seperator[10] = { "(", ")", "[","]", "{", "}", ",", ";", ":", "%%" };
    const char* operat[10] = {"==","^=","<",">","=>", ">=", "+", "-", "*", "/"};
    int identifer[6][3] = {
        { 2,3,3 },
        { 2,4,5 },
        { 3,3,3 },
        { 6,4,5 },
        { 3,3,5 },
        { 6,4,5 }
    };
    int real[5][2] = {
        {2,5},
        {2,3},
        {4,3},
        {4,5},
        {5,5}
    };
    
};

//it adds in the neccessay whitespace then splits it in tokens
Lexer::Lexer()
{

}

token_type Lexer::lexical(string* n)
{
    //Contains seperators
    token_type tkn;

    int len = 0;
    len = sizeof(seperator) / sizeof(seperator[0]);
    int i = 0;
    while (i < len && !n->empty())
    {
        if (n->find(seperator[i]) != std::string::npos)
        {
            tkn = isSeperator(n);
            n->clear();
        }
        i++;
    }
    
    //Contains operator
    len = sizeof(operat) / sizeof(operat[0]);
    i = 0;
    while (i < len && !n->empty())
    {
        if (n->find(operat[i]) != std::string::npos)
        {
            tkn = isOperator(n);
            n->clear();
        }
        i++;
    }

    //real integers
    if (!n->empty())
    {
        if (((n->front() >= '0' && n->front() <= '9') && n->length() >= 1) || n->front() == '.')
        {
            tkn = isReal(n);
        }
        else if ((n->front() >= 'a' && n->front() <= 'z') || (n->front() >= 'A' && n->front() <= 'Z') ||
                 (n->front() >= '0' && n->front() <= '9'))
        {
            tkn = isIdentifier(n);
        }
        else if ((n->front() >= '0' && n->front() <= '9') && n->length() == 1)
        {
            tkn = isInteger(n);
        }
    }
    return tkn;
}

void Lexer::set_token(string* n)
{
    //Lexer lex;
    token_type tkn;
    string token;
    int size = n->length()-1;
    int i = 0;
    while (i <= size)
    {
        if (n->at(i) != ' ' && n->at(i) != '\0')
        {
            token += n->at(i);
        }
        else
        {
            tkn = lexical(&token);
			if(!tkn.lexeme.empty())
				Lexer::tokens.push_back(tkn);		//adding token to vector
			token.clear();
        }
        i++;
    }
    tkn = lexical(&token);
	if (!tkn.lexeme.empty())
		Lexer::tokens.push_back(tkn);		//adding token to vector
    token.clear();
}

token_type Lexer::isIdentifier(string* n)
{
    
    token_type tkn;
    int r = 0, c = 0, pos = 0;
    unsigned long size = n->length();
    
    for (int i = 0; i < size; i++)
    {
        if (n->at(i) != ' ' && n->at(i) != '\0')
        {
            if ((n->at(i) >= 'a' && n->at(i) <= 'z') || (n->at(i) >= 'A' && n->at(i) <= 'Z'))
            {
                c = 0;
                pos = identifer[r][c];
                r = pos - 1;
            }
            else if ((n->at(i) >= '0' && n->at(i) <= '9'))
            {
                c = 1;
                pos = identifer[r][c];
                r = pos - 1;
            }
            else
            {
                c = 2;
                pos = identifer[r][c];
                r = pos - 1;
            }
        }
    }
    if (pos == 2 || pos == 5 || pos == 6)
    {
        tkn = iskeyword(n);						//---------------------edit
    }
    else
    {
        tkn.lexeme = n->data();
        tkn.token = "\tNot Accepted";
    }
    return tkn;
}

token_type Lexer::iskeyword(string* n)
{
    bool found = false;
    token_type tkn;
    string str = n->data();
    int i = 0;
    while (i < size(keyword) && !found)
    {
        if (str.compare(keyword[i]) == 0)
        {
            found = true;
            tkn.lexeme = n->data();
            tkn.token = "\tKeyword";
        }
        i++;
    }
    if (found == false)
    {
        tkn.lexeme = n->data();
        tkn.token = "\tIdentifier";
    }
    return tkn;
}

token_type Lexer::isInteger(string* n)
{
    token_type tkn;
    
    tkn.lexeme = n->data();
    tkn.token = "\tInteger";
    
    return tkn;
    
}
token_type Lexer::isReal(string* n)
{
    //it is a identifier
    unsigned long i = 0;
    unsigned long len = n->length();
    bool isletter = false;
    token_type tkn;
    while ((i < len) && !isletter)
    {
        if ((n->at(i) >= 'a' && n->at(i) <= 'z') || (n->at(i) >= 'A' && n->at(i) <= 'Z'))
        {
            isletter = true;
            tkn = isIdentifier(n);				//-------------------edit
        }
        i++;
    }
    
    //it is a real token
    if (isletter != true)
    {
        i = n->find(".");
        if (i == std::string::npos)
        {
            tkn = isInteger(n);					//---------------------edit
        }
        else
        {
            int r = 0, c = 0, pos = 0;
            unsigned long size = n->length();
            bool oneDot = false;
            for (int i = 0; i < size; i++)
            {
                if ((n->at(i) >= '0' && n->at(i) <= '9'))
                {
                    c = 0;
                    pos = real[r][c];
                    r = pos - 1;
                }
                else if(n->at(i) == '.' && !oneDot)
                {
                    oneDot = true;
                    c = 1;
                    pos = real[r][c];
                    r = pos - 1;
                }
                else
                {
                    c = 1;
                    pos = real[4][c];
                    r = pos - 1;
                }
            }
            
            if (pos == 4)
            {
                tkn.lexeme = n->data();
                tkn.token = "\tReal";
            }
            else
            {
                tkn.lexeme = n->data();
                tkn.token = "\tNot Accepted";
            }
        }
    }
    return tkn;
}
token_type Lexer::isSeperator(string* n)
{
    token_type tkn;
    if (n->length() == 1 || (n->compare("%%") == 0))
    {
        tkn.lexeme = n->data();
        tkn.token = "\tSeperator";
    }
    else
    {
        comp_tok(n);
    }
    
    return tkn;
}
token_type Lexer::isOperator(string* n)
{
    token_type tkn;
    if (n->length() <= 2)
    {
        tkn.lexeme = n->data();
        tkn.token = "\tNot Accepted";
    }
    else
    {
        comp_tok(n);
    }
    return tkn;
}

void Lexer::comp_tok(string* n)
{
    //splitting up token without whitespaces
    unsigned long size = n->length();
    unsigned long pos = 0;
    int len = sizeof(seperator) / sizeof(seperator[0]);
    
    for (int i = 0; i < len; i++)
    {
        pos = n->find(seperator[i]);
        if (pos != std::string::npos)
        {
            if (pos == size)
            {
                n->insert(pos, " ");
            }
            else if(pos == 0)
            {
                n->insert(pos+1, " ");
            }
            //else if (pos > 0 && pos < size)
            //{
            //	n->insert(pos + 1, " ");
            //}
            else
            {
                n->insert(pos, " ");
            }
        }
    }
    
    len = sizeof(operat) / sizeof(operat[0]);
    for (int j = 0; j < len; j++)
    {
        pos = n->find(operat[j]);
        if (pos != std::string::npos)
        {
            n->insert(pos, " ");
            n->insert((pos+2), " ");
        }
    }
    
    //calling lexical for split tokens
	set_token(n);
	//token_type tkn;
 //   string token;
 //   size = n->length() - 1;
 //   int i = 0;
 //   while (i <= size)
 //   {
 //       if (n->at(i) != ' ' && n->at(i) != '\0')
 //       {
 //           token += n->at(i);
 //       }
 //       else
 //       {
 //           tkn = lexical(&token);
	//		lex.tokens.push_back(tkn);
 //           token.clear();
 //       }
 //       i++;
 //   }
 //   tkn = lexical(&token);
	//lex.tokens.push_back(tkn);
 //   token.clear();
}

#endif /* lexer_h */
