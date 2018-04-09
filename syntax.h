#ifndef syntax_h
#define syntax_h

#pragma once
#include<iomanip>
#include<iostream>
#include<string>
#include<fstream>
#include<vector>

#include "lexer.h"
using namespace std;

ofstream fout("SA.txt", std::ofstream::binary);

class syntax
{
public:
	syntax(Lexer&);
	void lexAdv();
	void Rat18s();
	void Opt_Function_Def();
	void Function_Def();
	void Function();
	void Opt_Parameter_list();
	void Parameter_List();
	void Parameter();
	void Qualifier();
	void Body();
	void Opt_Declaration_list();
	void Declaration_List();
	void Declaration();
	void IDs();
	void Statement_list();
	void Statement();
	void Compound();
	void Assign();
	void If();
	void Return();
	void Print();
	void Scan();
	void While();
	void Condition();
	void Relop();
	void Expression();
	void LRExpression();
	void Term();
	void LRTerm();
	void Factor();
	void Primary();
	void Empty();
	~syntax();

private:
	Lexer lex;
	bool printSwitch;
	int index;
	string currTkn;
};

syntax::syntax(Lexer& vLex)
	//: lex()
{
	lex = vLex;
	printSwitch = true;
	index = 0;
}

void syntax::lexAdv()
{
	if (index < lex.tokens.size())
	{
		currTkn = lex.tokens[index].lexeme;
		index++;
		if (printSwitch)
		{
			fout << "Token: " << lex.tokens[index].token << "\t Lexeme: " << lex.tokens[index].lexeme << "\r\n";
		}	
	}
}

void syntax::Rat18s()
{
	//lexAdv();
	fout << "Token: " << lex.tokens[index].token << "\t Lexeme: " << lex.tokens[index].lexeme << "\r\n";
	if(printSwitch)
	{
		fout << "\t<Rat18S>::= <Opt Function> %% <Opt Declaration List> <Statement List>\r\n";
	}
	Opt_Function_Def();
	if (lex.tokens[index].lexeme == "%%")
	{
		lexAdv();
		Opt_Declaration_list();
		Statement_list();
		fout << "Finished\r\n";
	}
	else
	{
		fout << "\r\n Syntax Error! expecting %% after " << lex.tokens[index].lexeme << " on line " << lex.tokens[index].lineNumber  << "\r\n"  << "\r\n";
		return;
	}
	if (index < lex.tokens.size())
	{
		lexAdv();
		Rat18s();
		cout << "infinite loop\n";
	}
}

void syntax::Opt_Function_Def()
{
	if (printSwitch) {
		fout << "\t<Opt Function Definition> ::= <Function Definitions> | <Empty>\r\n";
	}
	if (lex.tokens[index].lexeme == "function") {
		Function_Def();
	}
	else if (lex.tokens[index].lexeme == "%%") {
		Empty();
	}
	else {
		fout << "\r\n Syntax Error, expecting 'function' or '%%' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n"  << "\r\n";
		return;
	}
}

void syntax::Function_Def()
{
	if (printSwitch) {
		fout << "\t<Function Definitions> ::= <Function> | <Function> <Function Definitions>\r\n";
	}
	while (lex.tokens[index].lexeme == "function") {
		Function();
	}
}

void syntax::Function()
{
	if (printSwitch) {
		fout << "\t<Function> ::= function <Identifier> [ <Opt Paramenter List> ] <Opt Declaration List> <fout>\r\n";
	}
	lexAdv();

	if (lex.tokens[index].token == "\tIdentifier") {
		lexAdv();
		if (lex.tokens[index].lexeme == "[") {
			lexAdv();
			Opt_Parameter_list();
			if (lex.tokens[index].lexeme == "]") {
				lexAdv();
				Opt_Declaration_list();
				Body();
			}
			else {
				fout << "\r\n Syntax Error, expecting ']' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
				return;
			}
		}
		else {
			fout << "\r\n Syntax Error, expecting '[' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
			return;
		}
	}
	else {
		fout << "\r\n Syntax Error, expecting <Identifier> before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::Opt_Parameter_list()
{
	if (printSwitch)
		fout << "\t<Opt Parameter List> ::= <Parameter List> | <Empty>\r\n";

	if (lex.tokens[index].token == "\tIdentifier") {
		Parameter_List();
	}
	else if (lex.tokens[index].lexeme == "]") {
		Empty();
	}
	else {
		fout << "\r\n Syntax Error, expecting <Identifier> before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::Parameter_List()
{
	if (printSwitch)
	{
		fout << "\t<Parameter List> ::= <Parameter> | <Parameter>, <Parameter List>\r\n";
	}

	if (lex.tokens[index].token == "\tIdentifier") {
		Parameter();
		if (lex.tokens[index].lexeme == ",") {
			lexAdv();
			Parameter_List();
		}
	}
}

void syntax::Parameter()
{
	if (printSwitch)
	{
		fout << "\t<Parameter> ::= <IDs> : <Qualifier>\r\n";
	}
	IDs();
	if (lex.tokens[index].lexeme == ":")
	{
		lexAdv();
		Qualifier();
	}
	else
	{
		fout << "\r\n Syntax Error, expecting ':' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::Qualifier()
{
	if (printSwitch)
	{
		fout << "\t<Qualifier> ::= int | boolean | real\r\n";
	}
	if (lex.tokens[index].lexeme == "int" || lex.tokens[index].lexeme == "true" || lex.tokens[index].lexeme == "false"
		|| lex.tokens[index].lexeme == "real" || lex.tokens[index].lexeme == "boolean")
	{
		lexAdv();
	}
	else
	{
		fout << "\r\n Syntax Error, expecting 'integer', 'boolean', or 'real' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::Body()
{
	if (printSwitch)
	{
		fout << "\t<fout> ::= { <Statement List> }\r\n";
	}
	
	if (lex.tokens[index].lexeme == "{")
	{
		lexAdv();
		Statement_list();
		if (lex.tokens[index].lexeme == "}")
		{
			lexAdv();
		}
		else
		{
			fout << "\r\n Syntax Error, expecting '}' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
			return;
		}
	}
	else
	{
		fout << "\r\n<><><> Syntax Error, expecting '{' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::Opt_Declaration_list()
{
	if (printSwitch) 
	{
		fout << "\t<Opt Declaration List> ::= <Declaration List> | <Empty>\r\n";
	}

	if (lex.tokens[index].lexeme == "{")
	{
		Empty();
	}
	else if (lex.tokens[index].lexeme == "int" || lex.tokens[index].lexeme == "boolean" || lex.tokens[index].lexeme == "real")
	{
		Declaration_List();
	}
	else
	{
		fout << "\r\n Syntax Error, expecting 'integer', 'boolean', or 'real' before '" << lex.tokens[index].lexeme << "' on line: " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::Declaration_List()
{
	if (printSwitch) 
	{
		fout << "\t<Declaration List> ::= <Declaration>; | <Declaration> ; <Declaration List>\r\n";
	}
	Declaration();
	if (lex.tokens[index].lexeme == ";")
	{
		lexAdv();
		if (lex.tokens[index].lexeme == "int" || lex.tokens[index].lexeme == "boolean" || lex.tokens[index].lexeme == "real") {
			Declaration();
		}
	}
	else 
	{
		fout << "\r\n Syntax Error, expecting ';' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::Declaration()
{
	if (printSwitch)
	{
		fout << "\t<Declaration> ::= <Qualifier> <IDs>\r\n";
	}

	Qualifier();
	IDs();
}

void syntax::IDs()
{
	if (printSwitch) {
		fout << "\t<IDs> ::= <Identifier> | <Identifier>, <IDs>\r\n";
	}
	if (lex.tokens[index].token == "\tIdentifier") {
		lexAdv();
		if (lex.tokens[index].lexeme == ",") {
			lexAdv();
			IDs();
		}
		else if (lex.tokens[index].token == "\tIdentifier") {
			fout << "\r\n Syntax Error, expecting ',' between multiple identifiers on line " << lex.tokens[index].lineNumber  << "\r\n";
			return;
		}
	}
	else {
		fout << "\r\n Syntax Error, expecting <Identifier> before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::Statement_list()
{
	if (printSwitch)
	{
		fout << "<Statement List>::= <Statement> | <Statement> <Statement List>\r\n";
	}
	while (lex.tokens[index].lexeme == "if" || lex.tokens[index].lexeme == "return" || lex.tokens[index].lexeme == "put" || lex.tokens[index].lexeme == "get"
		|| lex.tokens[index].lexeme == "while" || lex.tokens[index].lexeme == "\tIdentifier")
	{
		Statement();
	}
}

void syntax::Statement()
{
	if (printSwitch)
		fout << "\t<Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Write> | <Scan> | <While>\r\n";

	if (lex.tokens[index].lexeme == "{")
		Compound();
	else if (lex.tokens[index].token == "\tIdentifier")
		Assign();
	else if (lex.tokens[index].lexeme == "if")
		If();
	else if (lex.tokens[index].lexeme == "return")
		Return();
	else if (lex.tokens[index].lexeme == "put")
		Print();
	else if (lex.tokens[index].lexeme == "get")
		Scan();
	else if (lex.tokens[index].lexeme == "while")
		While();
	else {
		fout << "\r\n Syntax Error, expecting proper '<Statement>' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::Compound()
{
	if (printSwitch)
	{
		fout << "\t<Compound> ::= {<Statement List>}\r\n";
	}
	if (lex.tokens[index].lexeme == "{") {
		lexAdv();
		Statement_list();
		if (lex.tokens[index].lexeme == "}") {
			lexAdv();
		}
	}
}

void syntax::Assign()
{
	if (printSwitch)
	{
		fout << "\t<Assign> ::= <Identifier> = <Expression>;\r\n";
	}
	if (lex.tokens[index].token == "\tIdentifier") {
		lexAdv();
		if (lex.tokens[index].lexeme == "=") {
			lexAdv();
			Expression();
			if (lex.tokens[index].lexeme == ";")
				lexAdv();
			else {
				fout << "\r\n <Assign> Syntax Error";
				return;
			}
		}
		else {
			fout << "\r\n Syntax Error, expecting ':=' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
			return;
		}
	}
}

void syntax::If()
{
	if (printSwitch)
	{
		fout << "\t<If> ::= if (<Condition>) <Statement> endif | if (<Condition>) <Statement> else <Statement> endif\r\n";
	}
	if (lex.tokens[index].lexeme == "if") {
		lexAdv();
		if (lex.tokens[index].lexeme == "(") {
			lexAdv();
			Condition();
			if (lex.tokens[index].lexeme == ")") {
				lexAdv();
				Statement();
				if (lex.tokens[index].lexeme == "endif") {
					lexAdv();
				}
				else if (lex.tokens[index].lexeme == "else") {
					lexAdv();
					Statement();
					if (lex.tokens[index].lexeme == "endif") {
						lexAdv();
					}
					else {
						fout << "\r\n Syntax Error, expecting 'endif' on line " << lex.tokens[index].lineNumber  << "\r\n";
						return;
					}
				}
				else {
					fout << "\r\n Syntax Error, expecting 'endif' or 'else' on line " << lex.tokens[index].lineNumber  << "\r\n";
					return;
				}
			}
			else {
				fout << "\r\n Syntax Error, expecting ) after <Condition> on line " << lex.tokens[index].lineNumber  << "\r\n";
				return;
			}
		}
		else {
			fout << "\r\n Syntax Error, expecting ( on line " << lex.tokens[index].lineNumber  << "\r\n";
			return;
		}
	}
}

void syntax::Return()
{
	if (printSwitch)
	{
		fout << "\t<Return> ::= return; | return <Expression>;\r\n";
	}
	lexAdv();
	if (lex.tokens[index].lexeme == ";") {
		lexAdv();
	}
	else
	{
		Expression();
		if (lex.tokens[index].lexeme == ";")
			lexAdv();
		else {
			fout << "\r\n Syntax Error, expecting ';' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
			return;
		}
	}
}

void syntax::Print()
{
	if (printSwitch)
	{
		fout << "\t<Print> ::= put (<Expressions>);\r\n";
	}
	lexAdv();
	if (lex.tokens[index].lexeme == "(") {
		lexAdv();
		Expression();
		if (lex.tokens[index].lexeme == ")") {
			lexAdv();
			if (lex.tokens[index].lexeme == ";")
				lexAdv();
			else {
				fout << "\r\n Syntax Error, expecting ';' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
				return;
			}
		}
		else {
			fout << "\r\n Syntax Error, expecting ')' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
			return;
		}
	}
	else {
		fout << "\r\n Syntax Error, expecting '(' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::Scan()
{
	if (printSwitch)
	{
		fout << "\t<Scan> ::= get (<IDs>);\r\n";
	}
	lexAdv();
	if (lex.tokens[index].lexeme == "(")
	{
		lexAdv();
		IDs();
		if (lex.tokens[index].lexeme == ")")
		{
			lexAdv();
			if (lex.tokens[index].lexeme == ";")
			{
				lexAdv();
			}
			else
			{
				fout << "\r\n Syntax Error. Expecting ';' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
				return;
			}
		}
		else
		{
			fout << "\r\n Syntax Error. Expecting ')' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
			return;
		}
	}
	else
	{
		fout << "\r\n Syntax Error. Expecting '(' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::While()
{
	if (printSwitch)
	{
		fout << "\t<while> ::= while (<Condition>) <Statement>\r\n";
	}
	lexAdv();
	if (lex.tokens[index].lexeme == "(")
	{
		lexAdv();
		Condition();
		if (lex.tokens[index].lexeme == ")")
		{
			lexAdv();
			Statement();
		}
		else
		{
			fout << "\r\n Syntax Error. Expecting ')' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
			return;
		}
	}
	else
	{
		fout << "\r\n Syntax Error. Expecting '(' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::Condition()
{
	if (printSwitch)
	{
		fout << "<Condition>::= <Expression> <Relop> <Expression>\r\n";
	}
	Expression();
	Relop();
	Expression();
}

void syntax::Relop()
{
	if (printSwitch)
	{
		fout << "<Relop>::= == | ^= | > | < | => | =< \r\n";
	}
	if (lex.tokens[index].lexeme == "==" || lex.tokens[index].lexeme == "^=" || lex.tokens[index].lexeme == ">"
		|| lex.tokens[index].lexeme == "<" || lex.tokens[index].lexeme == "=>" || lex.tokens[index].lexeme == "=<")
	{
		lexAdv();
	}
	else
	{
		fout << "\r\n Syntax Error. Expecting 'operator' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::Expression()
{
	if (printSwitch)
	{
		fout << "<Expression>::= <Expression> + <Term> | <Expression> - <Term> | <Term>\r\n";
	}
	Term();
	LRExpression();
}

void syntax::LRExpression()
{
	if (printSwitch)
	{
		fout << "<LRExpression>::= + <Term> <LRExpression> | - <Term> <LRExpression> | <Empty\r\n>";
	}
	if (lex.tokens[index].lexeme == "+" || lex.tokens[index].lexeme == "-")
	{
		lexAdv();
		Term();
		LRExpression();
	}
	else if (lex.tokens[index].lexeme == "\tNot Accepted")
	{
		fout << "\r\n Syntax error, expecting '+', '-', or nothing before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
	else
	{
		Empty();
	}
}

void syntax::Term()
{
	if (printSwitch)
	{
		fout << "<Term>::= <Term> * <Factor> | <Term> / <Factor> | <Factor\r\n>";
	}
	Factor();
	LRTerm();
}

void syntax::LRTerm()
{
	if (printSwitch)
	{
		fout << "<LRTerm>::= * <Factor> <LRTerm> | / <Factor> <LRTerm> | <Empty>\r\n";
	}
	if (lex.tokens[index].lexeme == "*" || lex.tokens[index].lexeme == "/")
	{
		lexAdv();
		Factor();
		LRTerm();
	}
	else if (lex.tokens[index].lexeme == "\tNot Accepted")
	{
		fout << "\r\n Syntax error, expecting '*', '/', or nothing before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
	else
	{
		Empty();
	}
}

void syntax::Factor()
{
	if (printSwitch)
	{
		fout << "<Factor>::= - <Primary> | <Primary>\r\n";
	}
	if (lex.tokens[index].lexeme == "-")
	{
		lexAdv();
		Primary();
	}
	else if (lex.tokens[index].lexeme == "\tNot Accepted")
	{
		Primary();
	}
	else
	{
		fout << "\r\n Syntax error, expecting something before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::Primary()
{
	if (printSwitch)
	{
		fout << "<Primary>::= <Identifer> | <Integer> | <Identifier> (<IDs>) | (<Expression>) | <Real> | true | false\r\n";
	}
	if (lex.tokens[index].token == "\tIdentifier")
	{
		lexAdv();
		if (lex.tokens[index].lexeme == "(")
		{
			lexAdv();
			IDs();
			if (lex.tokens[index].lexeme == ")")
			{
				lexAdv();
			}
			else
			{
				fout << "\r\n Syntax error, expecting ')' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
				return;
			}
		}
		else
		{
		}
	}
	else if (lex.tokens[index].token == "\tInteger" || lex.tokens[index].token == "real")
	{
		lexAdv();
	}
	else if (lex.tokens[index].lexeme == "(")
	{
		lexAdv();
		Expression();
		if (lex.tokens[index].lexeme == ")")
		{
			lexAdv();
		}
		else
		{
			fout << "\r\n Syntax error, expecting ')' before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
			return;
		}
	}
	else if (lex.tokens[index].token == "true" || lex.tokens[index].token == "false")
	{
		lexAdv();
	}
	else
	{
		fout << "\r\n Syntax error, expecting <identifier>, <integer> or <Expression> before '" << lex.tokens[index].lexeme << "' on line " << lex.tokens[index].lineNumber  << "\r\n";
		return;
	}
}

void syntax::Empty()
{
	if (printSwitch)
	{
		fout << "<Empty>::= epsilon\r\n";
	}
}

syntax::~syntax()
{
}

#endif