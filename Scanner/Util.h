/*
 * Util.h
 *
 *  Created on: Sep 6, 2014
 *      Author: eeeyre
 */

#ifndef Kind_H_
#define Kind_H_
#include <string>
enum Kind
{
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
	SID,
	COD
};
#endif /* State_H_ */

#ifndef Token_H_
#define Token_H_
#include <sstream>
#include <string>
class Token{
public:
	Token(Kind k, std::string v, unsigned int l);
	virtual ~Token(){};
	std::string toString();
	Kind kind;
	std::string value;
	unsigned int line_num;
};
#endif /* Token_H_ */
