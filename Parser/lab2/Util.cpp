/*
 * Util.cpp
 *
 *  Created on: Sep 6, 2014
 *      Author: eeeyre
 */

#include "Util.h"
#include <sstream>
#include <string>

std::string KindtoString(Kind k)
{
	std::string result = "";
	switch (k)
	{
	case S:					result = "START";		break;
	case COMMA:				result = "COMMA"; 		break;
	case PERIOD:			result = "PERIOD"; 		break;
	case Q_MARK:			result = "Q_MARK";		break;
	case LEFT_PAREN:		result = "LEFT_PAREN";	break;
	case RIGHT_PAREN:		result = "RIGHT_PAREN"; break;
	case COLON:				result = "COLON";		break;
	case COLON_DASH:		result = "COLON_DASH";	break;
	case SCHEMES:			result = "SCHEMES";		break;
	case FACTS:				result = "FACTS";		break;
	case RULES:				result = "RULES";		break;
	case QUERIES:			result = "QUERIES";		break;
	case ID:				result = "ID";			break;
	case STRING:			result = "STRING";		break;
	case NUL:				result = "";			break;
	case END:				result = "EOF";			break;
	case UNKNOWN:			result = "unknown";		break;
	}
	return result;
}

Token::Token(Kind k, std::string v, unsigned int l)
{
	kind = k;
	value = v;
	line_num = l;
}
Token::Token()
{
	kind = S;
	value = "";
	line_num = 0;
}

std::string Token::toString()
{
	std::string line_number;
	std::ostringstream convert;
	convert << line_num;
	std::string result = "";
	result += "(";
	result += KindtoString(kind);
	result += ",\"";
	result += value;
	result += "\",";
	result += convert.str();
	result += ")";
	return result;
}

/*
 * Scanner.cpp
 *
 *  Created on: Sep 6, 2014
 *      Author: eeeyre
 */
#include <fstream>
#include <istream>
#include <stdio.h>
Scanner::Scanner(std::string infile)
{
	ifile.open(infile.c_str());

	line = 1;
	count = 0;
	std::remove (filename.c_str());
	statecheck = START;
}

Token Scanner::getMajorToken()
{
	Token check = getNextToken();
	while (check.kind == NUL)
	{
		check = getNextToken();
	}
	if (check.kind == UNKNOWN)
		fail();
	else
	{
		if (check.kind != END)
			count++;
	}
	return check;
}

Token Scanner::getNextToken()
{

	if (statecheck != SID && statecheck !=COD)
		ifile >> std::noskipws >> c;
	statecheck = START;
	if(!ifile.std::ios::fail())
	{
		std::string ch = std::string(1,c);
		switch (c)
		{
		case '#':	return comment();					break;
		case EOF: 	return Token(END,"",line);			break;
		case '\n':	line ++; return Token(NUL,"newline",line); break;
		case '\r':	line ++; return Token(NUL,"newline",line); break;
		case ',':	return Token(COMMA,ch,line); 		break;
		case '.':	return Token(PERIOD,ch,line); 	break;
		case '?':	return Token(Q_MARK,ch,line); 	break;
		case '(':	return Token(LEFT_PAREN,ch,line); break;
		case ')':	return Token(RIGHT_PAREN,ch,line); break;
		default:
			if (std::isspace(c))
			{
				return Token(NUL, "Whitespace", line);
			}
			else return complexToken();
			break;

		}
	}
	else return Token(END,"#",line);
}

Token Scanner::complexToken()
{
	if (c == '\'')
	{
		return stringToken();
	}
	else if (std::isalpha(c))
	{
		return idToken();
	}
	else if (c == ':')
	{
		return colonDash();
	}
	else
		return Token(UNKNOWN,"",line);

}

Token Scanner::stringToken()
{
	std::ostringstream s;
	int linestart=line;
	ifile >> std::noskipws >> c;
	while (c!='\'' && c!=EOF && !ifile.std::ios::eof()&& c!='\n'&&c!='\r')
	{
		s<<c;
		ifile >> std::noskipws >> c;

	}
	if (c!='\'')
	{
		line = linestart;
		return Token(UNKNOWN,"unterminated string",line);
	}
	return Token(STRING,s.str(),linestart);
}
Token Scanner::idToken()
{
	statecheck = SID;
	std::ostringstream s;
	while (!isspace(c)&&isalnum(c))
	{
		s<<c;
		ifile >> std::noskipws >> c;
	}
	if (s.str()=="Facts")
		return Token(FACTS, s.str(),line);
	else if (s.str()=="Rules")
		return Token(RULES,s.str(),line);
	else if (s.str()=="Queries")
		return Token(QUERIES,s.str(),line);
	else if (s.str()=="Schemes")
		return Token(SCHEMES,s.str(),line);

	return Token(ID,s.str(),line);
}
Token Scanner::colonDash()
{
	ifile>>std::noskipws>>c;
	if (c!='-')
	{
		statecheck = COD;
		return Token(COLON,":",line);
	}
	else
	{
		return Token(COLON_DASH,":-",line);
	}
}
Token Scanner::comment()
{
	while (c!='\r'&&c!='\n')
	{
		ifile>>std::noskipws>>c;
	}
	line++;
	return Token(NUL,"Comment",line);
}
void Scanner::fail()
{
	std::ostringstream convert;
	convert << line;
	std::string out ="";
	out +="Input Error on line ";
	out += convert.str();
//	write(out);
}
void Scanner::write(std::string output)
{
	//ofile.open (filename.c_str(), std::ios::app);
	//ofile << output << std::endl;
	//ofile.close();
}
void Scanner::closewrite()
{
	std::ostringstream convert;
	convert << count;
	std::string out ="";
	out +="Total Tokens = ";
	out += convert.str();
//	write(out);
}
