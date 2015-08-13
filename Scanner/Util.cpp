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
