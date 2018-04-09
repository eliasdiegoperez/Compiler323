#include<iostream>
#include<iomanip>
#include<string>
#include<fstream>
#include<vector>
using namespace std;

void lexical(string* n, vector<string>* sa_Token);
void set_token(string* n, vector<string>* sa_Token);	//splits up the token
void iskeyword(string* n);
void isIdentifier(string* n);	//uses a FSM
void isInteger(string* n);
void isReal(string* n);			//Uses a FSM
void isSeperator(string* n);
void isOperator(string* n);
void comp_tok(string* n);	//exists if there is a token without any whitespace;
							//it adds in the neccessay whitespace then splits it in tokens

ifstream fin;
ofstream fout;
vector<string> sa_Tokens;
const char* keyword[] = { "if", "while", "get", "for", "else" ,"int", "put", "endif", "return", "real", "bool", "true", "false", "function" };
const char* seperator[] = { "(", ")", "[","]", "{", "}", ",", ";", ":", "%%" };
const char* operat[] = {"==","^=","<",">","=>", ">=", "+", "-", "*", "/"};

int main()
{
	string in, out;
	cout << "Insert test file destination: ";
	cin >> in;

	cout << "Output file destination: ";
	cin >> out;

	fin.open(in, ios::in);
	fout.open(out);

	string token;

	while (fin.peek() != '0' && !fin.eof())
	{
		getline(fin, token);
		
		set_token(&token, &sa_Tokens);

		for (int i = 0; i < sa_Tokens.size(); i++)
		{
			cout << sa_Tokens[i].data() << endl;
		}
		sa_Tokens.clear();
	}


	cout << endl;
	system("Pause");
	exit(0);
}

void lexical(string* n, vector<string>* sa_Token)
{
	//Contains seperators
	int len = 0;
	
	len = sizeof(seperator) / sizeof(seperator[0]);
	int i = 0;
	while (i < len && !n->empty())
	{
		if (n->find(seperator[i]) != std::string::npos)
		{
			isSeperator(n);
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
			isOperator(n);
			n->clear();
		}
		i++;

	}

	if (!n->empty())
	{
		sa_Token->push_back(n->data());
	}
	//real integers
	if (!n->empty())
	{
		if ((n->front() >= '0' && n->front() <= '9') && n->length() >= 1 || n->front() == '.')
		{
			isReal(n);
		}
		else if ((n->front() >= 'a' && n->front() <= 'z') || (n->front() >= 'A' && n->front() <= 'Z') ||
			(n->front() >= '0' && n->front() <= '9'))
		{
			isIdentifier(n);
		}
		else if ((n->front() >= '0' && n->front() <= '9') && n->length() == 1)
		{
			isInteger(n);
		}
	}
}

void set_token(string* n, vector<string>* sa_token)
{
	string token;
	int size = n->length()-1;
	int i = 0;
	while (i <= size)
	{
		if (n->at(i) != ' ' && n->at(i) != '\0' && n->at(i) != '!')
		{
			token += n->at(i);
		}
		else if (n->at(i) == '!')
		{
			n->erase(n->begin() + i, n->end());
			size = n->length();
		}
		else
		{
			lexical(&token, sa_token);
			//sa_token->push_back(token);
			token.clear();
		}
		i++;
	}
	lexical(&token, sa_token);
	//sa_token->push_back(token);
	token.clear();
}

void isIdentifier(string* n)
{
	int identifer[6][3] = { { 2,3,3 },{ 2,4,5 },{ 3,3,3 },{ 6,4,5 },{ 3,3,5 },{ 6,4,5 } };

	int r = 0, c = 0, pos = 0;
	int size = n->length();

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
		iskeyword(n);
	}
	else
	{
		fout << n->data() << "\tNot accepted" << endl;
	}
}

void iskeyword(string* n)
{
	bool found = false;
	string str = n->data();
	int i = 0;
	while (i < size(keyword) && !found)
	{
		if (str.compare(keyword[i]) == 0)
		{
			found = true;
			fout << n->data() << "\tKeyword" << endl;
		}
		i++;
	}
	if (found == false)
	{
		fout << n->data() << "\tIdentifier" << endl;
	}
}

void isInteger(string* n)
{
	fout << n->data() << "\tInteger" << endl;
}
void isReal(string* n)
{
	//it is a identifier
	int i = 0;
	int len = n->length();
	bool isletter = false;
	while ((i < len) && !isletter)
	{
		if ((n->at(i) >= 'a' && n->at(i) <= 'z') || (n->at(i) >= 'A' && n->at(i) <= 'Z'))
		{
			isletter = true;
			isIdentifier(n);
		}
		i++;
	}

	//it is a real token
	if (isletter != true)
	{
		i = n->find(".");
		if (i == std::string::npos)
		{
			isInteger(n);
		}
		else
		{
			int real[5][2] = { {2,5},{2,3},{4,3},{4,5},{5,5} };
			int r = 0, c = 0, pos = 0;
			int size = n->length();
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
				fout << n->data() << "\tReal" << endl;
			}
			else
			{
				fout << n->data() << "\tNot accepted" << endl;
			}
		}
	}
}
void isSeperator(string* n)
{
	if (n->length() == 1)
	{
		sa_Tokens.push_back(n->data());
		fout << n->data() << "\t Seperator" << endl;
	}
	else
	{
		comp_tok(n);
	}
}
void isOperator(string* n)
{
	if (n->length() == 1)
	{
		sa_Tokens.push_back(n->data());
		fout << n->data() << "\t Operator" << endl;
	}
	else
	{
		comp_tok(n);
	}
}

void comp_tok(string* n)
{
	//splitting up token without whitespaces
	int size = n->length();
	int pos = 0;
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
	//set_token(n,&sa_Tokens);
	
	string token;
	size = n->length() - 1;
	int i = 0;
	while (i <= size)
	{
		if (n->at(i) != ' ' && n->at(i) != '\0')
		{
			token += n->at(i);
		}
		else
		{
			lexical(&token, &sa_Tokens);
			token.clear();
		}
		i++;
	}
	lexical(&token, &sa_Tokens);
	token.clear();
}