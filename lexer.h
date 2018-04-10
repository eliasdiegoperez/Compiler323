//
//  lexer.h
//  Compiler
//
//  Created by Elias Perez on 4/5/18.
//  Copyright © 2018 CPSC 323. All rights reserved.
//

#ifndef lexer_h
#define lexer_h

#define SEPARATOR_SIZE 9
#define OPERATER_SIZE 7
#define RELOP_SIZE 4
#define KEYWORD_SIZE 14

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

ifstream fin;
ofstream fout;

struct TokenType
{
    string token;
    string lexeme;
    int linenum;
};

class Lexer {
public:
    Lexer();
    vector<TokenType> lexical (string, int&);
    void push_token(TokenType&, int&, string&, char, char, vector<TokenType>&);
    void is_bang(string &line, char &curr_char, int &string_counter, int&);
    bool is_keyword(string);
    bool is_separator(char);
    bool is_rel_operat(char, char);
    bool is_operator(char, char);
    
private:
    int stateTable[9][4] {
                           // STATES
        { 2, 3, 1, 1 },    // 0
        { 1, 1, 1, 1 },    // 1
        { 4, 5, 6, 1 },    // 2 accepting
        { 1, 3, 1, 7 },    // 3 accepting
        { 4, 5, 6, 1 },    // 4 accepting
        { 4, 5, 6, 1 },    // 5
        { 1, 1, 1, 1 },    // 6 accepting
        { 1, 8, 1, 1 },    // 7
        { 1, 8, 1, 1 }     // 8
    };


    char separators[SEPARATOR_SIZE] = {'(', ')', '[',']', ':', ';', '{', '}', ','};
    char operators[OPERATER_SIZE] = {'=', '+', '-', '/', '*', '<', '>'};
    string rel_opert[RELOP_SIZE] = {"==", "^=", "=>", "=<"};
    string keywords[KEYWORD_SIZE] = { "if", "while", "get", "for", "else",
                            "int", "put", "endif", "return", "real",
                            "boolean", "true", "false", "function" };
};

Lexer::Lexer() {
    // Default Constructor
}

// Handles comments signified by '!' until next '!'
void Lexer::is_bang(string &curr_line, char &curr_char, int &string_counter, int &lineNum) {
    
    bool endComment = false;
    if(isspace(curr_line[0])) {
        curr_line.erase(curr_line.begin());
    }
    while (!endComment) {
        if (string_counter <= curr_line.length()) {
            curr_char = curr_line[++string_counter];
            if (curr_char == '!') {
                endComment = true;
                curr_line.erase(0, string_counter+1);
            }
            
        }
        else {
            getline(fin, curr_line);
            lineNum++;
            string_counter = 0;
        }
    }
}

bool Lexer::is_keyword(string contestant) {
    
    bool found = false;
    int i = 0;
    while(i < KEYWORD_SIZE && !found) {
        if(keywords[i] == contestant) {
            found = true;
        }
        else
            i++;
    }
    return found;
}

bool Lexer::is_separator(char contestant) {
    
    bool found = false;
    int i = 0;
    while(i < SEPARATOR_SIZE && !found) {
        if(separators[i] == contestant) {
            found = true;
        }
        else
            i++;
    }
    return found;
}

bool Lexer::is_rel_operat(char curr_char, char next_char) {
    bool found = false;
    string temp = "";
    temp += curr_char;
    temp += next_char;
    
    if (temp == "%%") {
        found = true;
    }
    else {
        int i = 0;
        while (i < RELOP_SIZE && !found) {
            if (temp == rel_opert[i]) {
                found = true;
            }
            else
                i++;
        }
    }
    
    
    return found;
}

bool Lexer::is_operator(char contestant, char next_char) {
    
    bool found = false;
    int i = 0;
    while(i < OPERATER_SIZE && !found) {
        if(operators[i] == contestant) {
            if (is_rel_operat(contestant, next_char)) {
                return false;
            }
            else
                found = true;
        }
        else
            i++;
    }
    return found;
}

void Lexer::push_token(TokenType &access, int &state, string &curr_lex, char curr_char, char next_char, vector<TokenType> &tokens) {
    
    if (state == 4 || state == 6 || state == 2) {
        if (is_keyword(curr_lex)) {
            
            access.token = "Keyword";
            access.lexeme = curr_lex;
        }
        else if (is_separator(curr_char) || curr_lex == "%%") {
            
            access.token = "Separator";
            access.lexeme = curr_lex;
        }
        else if (is_operator(curr_char, next_char) || curr_lex == "==" || curr_lex == "^="
                                        || curr_lex == "=<" || curr_lex == "=>") {
            if (is_rel_operat(curr_char, next_char)) {
                curr_lex.clear();
                curr_lex += curr_char;
                curr_lex += next_char;
            }
            access.token = "Operator";
            access.lexeme = curr_lex;
        }
        else {
            
            access.token = "Identifier";
            access.lexeme = curr_lex;
        }
    }
    else if (state == 8) {
        
        access.token = "Real";
        access.lexeme = curr_lex;
    }
    else if (state == 3) {
        
        access.token = "Integer";
        access.lexeme = curr_lex;
    }
    else {
        
        access.token = "Unknown";
        access.lexeme = curr_lex;
    }
    
    if (curr_lex != "") {
        tokens.push_back(access);
    }
    state = 0;
    curr_lex.clear();
}

vector<TokenType> Lexer::lexical(string curr_line, int &lineNum) {
    
    TokenType access;
    vector<TokenType> tokens;
    char curr_char = ' ';
    char next_char = ' ';
    int state = 0;
    int row = 0;
    int column = 0;
    
    string current_token;
    
    int i = 0;
    while (i < curr_line.length()) {
        
        // get next char
        curr_char = curr_line[i];
        if (i != curr_line.length()) {
            next_char = curr_line[i+1];
        }
        
        
        if (is_rel_operat(curr_char, next_char)) {
            state = 4;
            current_token += curr_char;
            current_token += next_char;
            push_token(access, state, current_token, curr_char, next_char, tokens);
            i++;
        }
        else if (is_separator(curr_char) || is_operator(curr_char, next_char) || curr_char == '!') {
            state = 4;
            
            if (curr_char != '!') {
                if (current_token == "") {
                    current_token += curr_char;
                }
                push_token(access, state, current_token, curr_char, next_char, tokens);
            }
            else {
                is_bang(curr_line, curr_char, i, lineNum);
                if (curr_line != "") {
                    i = 0;
                    while (i < curr_line.length() && isspace(curr_line[i])) {
                        curr_line.erase(curr_line.begin());
                    }
                    push_token(access, state, current_token, curr_char, next_char, tokens);
                }
            }

        }
        else if (!isspace(curr_char) && curr_char != '\0') {
            
            if (isalpha(curr_char)) {
                
                column = 0;
                row = state;
            }
            else if(isdigit(curr_char)) {
                
                column = 1;
                row = state;
            }
            else if (curr_char == '$') {
                
                column = 2;
                row = state;
            }
            else if (curr_char == '.') {
                
                column = 3;
                row = state;
            }
           
            current_token += curr_char;
            state = stateTable[row][column];
            
            if (is_separator(curr_line[i+1]) || is_operator(curr_line[i+1], next_char) || curr_line[i+1] == '!' || isspace(curr_line[i+1])) {
                push_token(access, state, current_token, curr_char, next_char, tokens);
            }
            
        }
        else {
            // do nothing
        }
        
        i++;
    }
    
    push_token(access, state, current_token, curr_char, next_char, tokens);
    return tokens;
}

#endif /* lexer_h */
