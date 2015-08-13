/*
 * Scanner.cpp
 *
 *  Created on: Sep 6, 2014
 *      Author: eeeyre
 */
#include <fstream>
#include <istream>
#include "Scanner.h"
#include <stdio.h>
Scanner::Scanner(std::string infile, std::string opfile)
{
	ifile.open(infile.c_str());
	filename=opfile;
	line = 1;
	count = 0;
	std::remove (filename.c_str());
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
	write(out);
}
void Scanner::write(std::string output)
{
	ofile.open (filename.c_str(), std::ios::app);
	ofile << output << std::endl;
	ofile.close();
}
void Scanner::closewrite()
{
	std::ostringstream convert;
	convert << count;
	std::string out ="";
	out +="Total Tokens = ";
	out += convert.str();
	write(out);
}
