/*
 * Util.h
 *
 *  Created on: Sep 6, 2014
 *      Author: eeeyre
 */
#include <string>
#include <fstream>
#include <sstream>


#ifndef Kind_H_
#define Kind_H_

enum Kind
{
	S,
	COMMA,
	PERIOD,
	Q_MARK,
	LEFT_PAREN,
	RIGHT_PAREN,
	COLON,
	COLON_DASH,
	SCHEMES,
	FACTS,
	RULES,
	QUERIES,
	ID,
	STRING,
	NUL,
	END,
	UNKNOWN
};
std::string KindtoString(Kind k);
#endif /* Enum_H_ */


#ifndef State_H_
#define State_H_

enum State
{
	START,
	RULE,
	SID,
	COD
};
#endif /* State_H_ */

#ifndef Token_H_
#define Token_H_

class Token{
public:
	Token(Kind k, std::string v, unsigned int l);
	Token();
	virtual ~Token(){};
	std::string toString();
	Kind kind;
	std::string value;
	unsigned int line_num;
};
#endif /* Token_H_ */

/*
 * Scanner.h
 *
 *  Created on: Sep 6, 2014
 *      Author: eeeyre
 */


#ifndef SCANNER_H_
#define SCANNER_H_

class Scanner {
public:
	Scanner(std::string infile);
	virtual ~Scanner(){};
	Token getMajorToken();
	void write(std::string output);
	void closewrite();
private:
	unsigned int line;
	std::ifstream ifile;
	std::ofstream ofile;
	std::string filename;
	Token getNextToken();
	Token complexToken();
	Token stringToken();
	Token idToken();
	Token colonDash();
	Token comment();
	unsigned int count;
	void fail();
	State statecheck;
	char c;

};

#endif /* SCANNER_H_ */

