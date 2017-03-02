#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cstdio>
#include <map>
#include <list>
#include <stack>

using namespace std;

FILE *apunta;
char c;
int ref = 0, avan = 0, cont = 1;
std::map<std::string, std::map<std::string, int > > controlador;
std::stack<std::string> pila;

typedef enum
{
	_identifier,
	_nums,
	_dot,
	_comma,
	_pard,
	_pari,
	_cord,
	_cori,
	_dotcomma,
	_ddot,
	_add,
	_sus,
	_mult,
	_div,
	_throw,
	_comment,
	_eof,
	_error
} token;

char sig()
{
	char d = fgetc(apunta);
	avan = ftell(apunta);
	return d;
}

void retro()
{

	fseek(apunta, -1, SEEK_CUR);
	avan = ftell(apunta);
	
}

void acepta()
{
	ref = avan;
}

void fail()
{
	avan = ref;
	fseek(apunta, ref, SEEK_SET);
}

token identifier()
{
	int anterior = 0, actual=1;
	char c;
	string reserved = "";
	string dictionary[1] = {"throw"};
	
	while(actual != -1)
	{
		anterior = actual;
		c = sig();
		
		switch(actual)
		{
			case 1:
				if(isalpha(c))
				{
					actual = 2;
					reserved += c;
				}
				else if(c == '_')
				{
					actual = 3;
					reserved += c;
				}
				else
					actual = -1;
			break;
			
			case 2:
				if(c == '_' || isalnum(c))
				{
					actual = 2;
					reserved += c;
				}
				else if(c == 39)
				{
					actual = 4;
					reserved += c;
				}
				else
					actual = -1;
			break;
			
			case 3:
				if(isalpha(c))
				{
					actual = 2;
					reserved += c;
				}
				else if(c == '_')
				{
					actual = 2;
					reserved += c;
				}
				else
					actual = -1;
			break;
			
			case 4:
				if(c == 39)
				{
					actual = 4;
					reserved += c;
				}
				else
					actual = -1;
			break;
					
		}
	}
	
	if(anterior == 2 || anterior == 4)
	{
		if(reserved == dictionary[0] )
		{
			retro();
			acepta();
			return _throw;
		}
			
		else if(c == EOF)
		{
			acepta();
			return _eof;
		}
		else
		{
			retro();
			acepta();
			return _identifier;
			
		}
	}
	else
	{
		fail();
		return _error;
	}
}

token octal_hex_real()
{
	int anterior = 0, actual = 1;
	
	char c;
	string num = "";
	
	while(actual != -1)
	{
		anterior = actual;
		c = sig();
		
		switch(actual)
		{
			case 1:
				if(c == '0')
				{
					actual = 2;
					num += c;
				}
				else if(c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
				{
						actual = 3;
						num += c;
				}
				else
					actual = -1;
			break;
			
			case 2:
				if(c== '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' )
				{
					actual = 5;
					num += c;
				}
				else if(c == '.')
				{
					actual = 4;
					num += c;
				}
				else if(c == 'X' || c == 'x')
				{
					actual = 6;
					num += c;
				}
				else
					actual = -1;
			break;
			
			case 3:
				if(isdigit(c))
				{
					actual = 3;
					num += c;
				}
				else if(c == '.')
				{
					actual = 4;
					num += c;
				}
				else if(c == 'E' || c == 'e')
				{
					actual = 9;
					num += c;
				}
				else
					actual = -1;
			break;
			
			case 4:
				if(isdigit(c))
				{
					actual = 7;
					num += c;
				}
				else
					actual = -1;
			break;
			
			case 5:
				if(c== '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' )
				{
					actual = 5;
					num += c;
				}
				else
					actual = -1;

			break;
			
			case 6:
				if(isdigit(c) || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F')
				{
					actual = 8;
					num += c;
				}
				else
					actual = -1;
			break;
			
			case 7:
				if(c == 'E' || c == 'e')
				{
					actual = 9;
					num += c;
				}
				else if(isdigit(c))
				{
					actual = 7;
					num +=c;
				}
				else
					actual = -1;
			break;
			
			case 8:
				if(isdigit(c) || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F')
				{
					actual = 10;
					num += c;
				}
				else
					actual = -1;
			break;
			
			case 9:
				if(isdigit(c))
				{
					actual = 11;
					num += c;
				}
				else if(c == '+' || c == '-')
				{
					actual = 12;
					num += c;
				}
				else
					actual = -1;
			break;
			
			case 10:
				if(isdigit(c) || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F')
				{
					actual = 8;
					num += c;
				}
				else
					actual = -1;
			break;
			
			case 11:
				if(isdigit(c))
				{
					actual = 11;
					num += c;
				}
				else
					actual = -1;
			break;		
			
			case 12:
				if(isdigit(c))
				{
					actual = 11;
					num += c;
				}
				else
					actual = -1;

			break;

		}
	}
	if(anterior == 2 || anterior == 5)
	{
		//cout<< "Num Octal: " << num << endl;
		retro();
		acepta();
		return _nums;
	}
	else if(anterior == 3 || anterior == 7 || anterior == 11)
	{
		//cout<< "Num Real: " << num << endl;
		retro();
		acepta();
		return _nums;
	}
	else if(anterior == 10)
	{
		//cout<< "Num Hexadecimal: " << num << endl;
		retro();
		acepta();
		return _nums;
	}
	else
	{
		fail();
		return _error;
	}
	
}

token signs()
{
	int anterior = 0, actual = 1;
	char temp;
	
	char c;
	string simb = "";
	token _res;
	
	while(actual != -1)
	{
		anterior = actual;
		c = sig();
		switch(actual)
		{
			case 1:
				if(c == '(' || c == ')' || c == '[' || c == ']')
					actual = 2;
				else if(c == '+' || c == '-' || c == '*' || c =='/')
					actual = 3;
				else if(c == ',' || c == ';' || c == '.')
					actual = 4;
				else if(c == ':')
					actual = 5;
				else
					actual = -1;
			break;
			
			case 2:
				if(c == '(')
				{
					actual = -1;
					temp = c;
				}
				else if(c == ')')
				{
					actual = -1;
					temp = c;
				}
				else if(c == '[')
				{
					actual = -1;
					temp = c;
				}
				else if(c == ']')
				{
					actual = -1;
					temp = c;
				}
				else
				{
					actual = -1;
				}
			break;
			
			case 3:
				if(c == '+')
				{
					actual = -1;
					temp = c;
				}
				else if(c == '-')
				{
					actual = -1;
					temp = c;
				}
				else if(c == '*')
				{
					actual = -1;
					temp = c;
				}
				else if(c == '/')
				{
					actual = -1;
					temp = c;
				}
				else
				{
					actual = -1;
				}
			break;
			
			case 4:
				if(c == ',')
				{
					actual = -1;
					temp = c;
				}
				else if(c == ';')
				{
					actual = -1;
					temp = c;
				}
				else if(c == '.')
				{
					actual = -1;
					temp = c;
				}
				else
				{
					actual = -1;
				}
			break;
			
			case 5:
				if(c == ':')
				{
					actual = -1;
					temp = c;
				}
				else
				{
					actual = -1;
				}
			break;

		}
	
		if(actual == 2)
		{
			if(c == '(')
			{
			//	cout<< "Parentesis Izquierdo000" << endl;
			//	retro();
				acepta();
			//	actual = -1;
				return _pari;
				//_res = _pari;
			}
			else if(c == ')')
			{
				//cout<< "Parentesis Derecho " << endl;
				//retro();
				acepta();
				return _pard;
			}
			else if(c == '[')
			{
				//cout<< "Corchete Izquierdo " << endl;
				//retro();
				acepta();
				return _cori;
				//_res = _cori;
			}
			else if(c == ']')
			{
				//cout<< "Corchete Derecho " << endl;
				//retro();
				acepta();
				return _cord;
			}
			else
			{
				//cout<<"ADIOSSSSSSSSSSS";
				fail();
				return _error;
			}
		}
		else if(actual == 3)
		{
			if(c == '+')
			{
				//cout<< "Signo de adicion " << endl;
			//	retro();
				acepta();
				return _add;
			}
			else if(c == '-')
			{
				//cout<< "Signo de sustraccion " << endl;
				//retro();
				acepta();
				return _sus;
			}
			else if(c == '*')
			{
				//cout<< "Signo de multiplicacion " << endl;
				//retro();
				acepta();
				return _mult;
			}
			else if(c == '/')
			{
				//cout<< "Signo de division " << endl;
				retro();
				acepta();
				return _div;
			}
			else
			{
				//cout<<"ADIOSSSSSSSSSSS";
				fail();
				return _error;
			}
			
		}
		else if(actual == 4)
		{
			if(c == ',')
			{
				//cout<< "coma" << endl;
				//retro();
				acepta();
				return _comma;
			}
			else if(c == ';')
			{
				//cout<< "Punto y coma " << endl;
			//	retro();
				acepta();
				return _dotcomma;
			}
			else if(c == '.')
			{
				//cout<< "Punto y coma " << endl;
				//retro();
				acepta();
				return _dot;
			}
			else
			{
				fail();
				return _error;
			}
		}
		else if(actual == 5)
		{
			if(c == ':')
			{
			//	retro();
				acepta();
				return _ddot;
			}
			else
			{
				fail();
				return _error;
			}
		}
		else
		{
			fail();
			return _error;
		}
	}
	
}

token comments()
{
	int anterior = 0, actual = 1;
	
	char c;
	string comm = "";
	
	while(actual != -1)
	{
		anterior = actual;
		c = sig();
		
		switch(actual)
		{
			case 1:
				if(c == '!')
				{
					actual = 2;
					comm += c;
				}
				else
					actual = -1;
			break;
			
			case 2:
				if(c == 10)
					actual = -1;
				else
					actual = 2;
					comm += c;
			break;
		}
	}
	if(anterior == 2 )
	{
		//cout<< "Comentario: " << comm;
		retro();
		acepta();
		return _comment;
	}
	else
	{
		fail();
		return _error;
	}
}

bool spaces()
{		
	c = sig();
	if (isspace(c))
	{
		if(c == 10)
			cont ++;	
		acepta();
		return true;
	}
	else
	{
		fail();
		return false;
	}
}

token eof()
{
	int f = sig();
	
	//if(sig() != NULL)
		//if(feof(apunta) == 0)
		if(f == EOF)
		{
			//cout<<"Fin de Archivo"<< endl;
			acepta();
			return _eof;
		}
	else
	{
		fail();
		return _error;
	}
		
}

token scanner()
{
	
	while(spaces());
		//cout<<"ESPACIO"<<endl;
		
	token _res = identifier();
	
	if(_res == _identifier)
	{
		//cout<<"identifier"<<endl;
		return _identifier;
	}
	if(_res == _throw)
	{
		//cout<<"throw"<<endl;
	 return _throw;
	}
	if(octal_hex_real() == _nums)
	{
		//cout<<"num"<<endl;
		return _nums;
	}
	
	 _res = signs();
	
	if(_res == _pard)
	{
		//cout<<"parentesisD"<<endl;
		return _pard;
	}
	if(_res == _pari)
	{
		//cout<<"ParentesisIZ"<<endl;
		return _pari;
	}
	if(_res == _cord)
	{
		//cout<<"corcheteD"<<endl;
		return _cord;
	}
	if(_res == _cori)
	{
		//cout<<"CorcheteIz"<<endl;
		return _cori;
	}
	if(_res == _dot)
	{
		//cout<<"punto"<<endl;
		return _dot;
	}
	if(_res == _add)
	{
		//cout<<"suma"<<endl;
		return _add;
	}
	if(_res == _sus)
	{
		//cout<<"resta"<<endl;
		return _sus;
	}
	if(_res == _mult)
	{
		//cout<<"mult"<<endl;
		return _mult;
	}
	if(_res == _div)
	{
		//cout<<"division"<<endl;
		return _div;
	}
	if(_res == _dotcomma)
	{
		//cout<<"puntocoma"<<endl;
		return _dotcomma;
	}
	if(_res == _ddot)
	{
		//cout<<"doblepunto"<<endl;
		return _ddot;
	}
	if(_res == _comma)
	{
		//cout<<"coma"<<endl;
		return _comma;
	}
	if(comments() == _comment)
		return _comment;
	if(eof() == _eof)
	{
		//cout<<"EOF"<<endl;
		return _eof;
	}
	
	else
	{
		cout<< "ERROR" << endl;
		return _error;	
	}	
}

void controller()
{
	controlador["MAIN"]["identifier"] = 0;
	controlador["MAIN"]["throw"] = 0;
	controlador["STSEQUENCE"]["identifier"] = 1;
	controlador["STSEQUENCE"]["throw"] = 1;
	controlador["STSEQUENCE'"][";"] = 2;
	controlador["STSEQUENCE'"]["."] = 3;
	controlador["STATEMENT"]["identifier"] = 4;
	controlador["STATEMENT"]["throw"] = 5;
	controlador["ASSIGNMENT"]["identifier"] = 6;
	controlador["THROWING"]["throw"] = 7;
	controlador["MEXPRESSION"]["("] = 8;
	controlador["MEXPRESSION"]["["] = 8;
	controlador["MEXPRESSION"]["identifier"] = 8;
	controlador["MEXPRESSION'"]["+"] = 9;
	controlador["MEXPRESSION'"]["-"] = 10;
	controlador["MEXPRESSION'"][";"] = 11;
	controlador["MEXPRESSION'"]["."] = 11;
	controlador["MEXPRESSION'"][")"] = 11;
	controlador["MTERM"]["("] = 12;
	controlador["MTERM"]["["] = 12;
	controlador["MTERM"]["identifier"] = 12;
	controlador["MTERM'"]["+"] = 14;
	controlador["MTERM'"]["-"] = 14;
	controlador["MTERM'"]["*"] = 13;
	controlador["MTERM'"][";"] = 14;
	controlador["MTERM'"]["."] = 14;
	controlador["MTERM'"][")"] = 14;
	controlador["MFACTOR"]["("] = 15;
	controlador["MFACTOR"]["["] = 16;
	controlador["MFACTOR"]["identifier"] = 17;
	controlador["MDEFINITION"]["["] = 18;
	controlador["ROWSEQUENCE"]["-"] = 19;
	controlador["ROWSEQUENCE"]["("] = 19;
	controlador["ROWSEQUENCE"]["numbers"] = 19;
	controlador["ROWSEQUENCE'"][";"] = 20;
	controlador["ROWSEQUENCE'"]["]"] = 21;
	controlador["ROW"][";"] = 20;
	controlador["ROW"]["-"] = 22;
	controlador["ROW"]["("] = 22;
	controlador["ROW"]["numbers"] = 22;	
	controlador["ROW'"][";"] = 24;
	controlador["ROW'"][","] = 23;	
	controlador["ROW'"]["]"] = 24;
	controlador["EXPRESSION"]["-"] = 25;
	controlador["EXPRESSION"]["("] = 25;
	controlador["EXPRESSION"]["numbers"] = 25;
	controlador["EXPRESSION'"]["+"] = 26;
	controlador["EXPRESSION'"]["-"] = 27;
	controlador["EXPRESSION'"][";"] = 28;
	controlador["EXPRESSION'"][","] = 28;	
	controlador["EXPRESSION'"][")"] = 28;
	controlador["EXPRESSION'"]["]"] = 28;	
	controlador["TERM"]["-"] = 29;
	controlador["TERM"]["("] = 29;
	controlador["TERM"]["numbers"] = 29;
	controlador["TERM'"]["+"] = 32;
	controlador["TERM'"]["-"] = 32;
	controlador["TERM'"]["*"] = 30;
	controlador["TERM'"]["/"] = 31;
	controlador["TERM'"][";"] = 32;
	controlador["TERM'"][","] = 32;
	controlador["TERM'"][")"] = 32;
	controlador["TERM'"]["]"] = 32;
	controlador["FACTOR"]["-"] = 33;
	controlador["FACTOR"]["("] = 34;
	controlador["FACTOR"]["numbers"] = 35;
	
}

std::string translate(token t)
{
	switch(t)
	{
		case _identifier:
			return "identifier";
		
		case _nums:
			return "numbers";
		
		case _dot:
			return ".";
			
		case _ddot:
			return ":";
		
		case _comma:
			return ",";
		
		case _pard:
			return ")";
		
		case _pari:
			return "(";
		
		case _cord:
			return "]";
			
		case _cori:
			return "[";
		
		case _dotcomma:
			return ";";
			
		case _add:
			return"+";
		
		case _sus:
			return "-";
		
		case _mult:
			return "*";
		
		case _div:
			return "/";
			
		case _throw:
			return "throw";
		case _eof:
			return "eof";
			
		default: return "error"; 
	}
	
}

int T(std::string a, std::string b)
{
	if(controlador.find(a) != controlador.end())
	{
		if(controlador[a].find(b) != controlador[a].end())
			return controlador[a][b];
	}
	else
	{
		return 36;	
	}
	
	
}

bool feedStack(int regla)
{
	switch(regla)
	{
		case 0:
			pila.push("."); pila.push("STSEQUENCE");
			return true;
		
		case 1:
			pila.push("STSEQUENCE'"); pila.push("STATEMENT");
			return true;
		
		case 2:
			pila.push("STSEQUENCE'"); pila.push("STATEMENT"); pila.push(";");
			return true;
		
		case 3:
			return true;
		case 4:
			pila.push("ASSIGNMENT");
			return true;
		
		case 5:
			pila.push("THROWING");
			return true;
		
		case 6:
			pila.push("MEXPRESSION"); pila.push(":"); pila.push("identifier");
			return true;
		
		case 7:
			pila.push("MEXPRESSION"); pila.push("throw");
			return true;
		
		case 8:
			pila.push("MEXPRESSION'"); pila.push("MTERM");
			return true;
		
		case 9:
			pila.push("MEXPRESSION'"); pila.push("MTERM"); pila.push("+");
			return true;
		
		case 10:
			pila.push("MEXPRESSION'"); pila.push("MTERM"); pila.push("-");
			return true;
		
		case 11:
			return true;
		case 12:
			pila.push("MTERM'"); pila.push("MFACTOR");
			return true;
		
		case 13:
			pila.push("MTERM'"); pila.push("MFACTOR"); pila.push("*");
			return true;
		
		case 14:
			return true;
		
		case 15:
			pila.push(")"); pila.push("MEXPRESSION"); pila.push("(");
			return true;
		case 16:
			pila.push("MDEFINITION");
			return true;
		
		case 17:
			pila.push("identifier");
			return true;
		
		case 18:
			pila.push("]"); pila.push("ROWSEQUENCE"); pila.push("[");
			return true;
		
		case 19:
			pila.push("ROWSEQUENCE'"); pila.push("ROW");
			return true;
		
		case 20:
			pila.push("ROWSEQUENCE'"); pila.push("ROW"); pila.push(";");
			return true;
		
		case 21:
			return true;
		
		case 22:
			pila.push("ROW'"); pila.push("EXPRESSION");
			return true;
		
		case 23:
			pila.push("ROW'"); pila.push("EXPRESSION"); pila.push(",");
			return true;
		
		case 24:
			return true;
		
		case 25:
			pila.push("EXPRESSION'"); pila.push("TERM");
			return true;
		
		case 26:
			pila.push("EXPRESSION'"); pila.push("TERM"); pila.push("+");
			return true;
		
		case 27:
			pila.push("EXPRESSION'"); pila.push("TERM"); pila.push("-");
			return true;
		
		case 28:
			return true;
		
		case 29:
			pila.push("TERM'"); pila.push("FACTOR");
			return true;
		
		case 30:
			pila.push("TERM'"); pila.push("FACTOR"); pila.push("*");
			return true;
		
		case 31:
			pila.push("TERM'"); pila.push("FACTOR"); pila.push("/");
			return true;
		
		case 32:
			return true;
		
		case 33:
			pila.push("FACTOR"); pila.push("-");
			return true;
		
		case 34:
			pila.push(")"); pila.push("EXPRESSION"); pila.push("(");
			return true;
		
		case 35:
			pila.push("numbers");
			return true;
		
		default: return false;
		
	}
	
	
}

void imprimePila(std::stack<std::string> p)
{
	std::stack<std::string> otrapila;
	while(!p.empty())
	{
		otrapila.push(p.top());
		cout<<' ' <<p.top()<<endl;
		p.pop();
		
	}
	cout<<endl<<endl;
	while(!otrapila.empty())
	{
		p.push(otrapila.top());
		otrapila.pop();
		
	}
	
}

bool LL()
{
	
	token t;
	std::string X;
	t = scanner();
	int pi;
	
	pila.push("MAIN");
	
	do
	{
		//imprimePila(pila);
		//system("pause");
		
		X = pila.top();
		pila.pop();
		//cout<< "X =   "<<X<<endl;
		//cout<<"T =     "<<translate(t)<<endl;
		if(X == translate(t))
		{
			t = scanner();
		}
		else
		{
			pi = T(X, translate(t));
			if(pi == 36) // 36 igual a error
				t = _error;
			else
			{
				
				feedStack(pi);
			}
		}
			
		
	}
	while(!pila.empty() && t != _error && t != _eof);
	
	return t == _eof;
	
}


int main()
{
	
	controller();
	char filen[30];
	cout<< "Introduce el nombre del archivo a procesar: ";
	cin >> filen;
	apunta = fopen(filen, "r");
	//(LL())? cout<<"ok": cout<< "NO" ;
	if(LL() == true)
		cout<<"OK"<<endl;
	else
		cout<<"NO"<<endl;
		
		
	fclose(apunta);
	system("pause");
	return 0;
}
