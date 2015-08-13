/*
 * Util.cpp
 *
 *  Created on: Sep 6, 2014
 *      Author: eeeyre
 */

#include "Util.h"
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
/*
 * Parser.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: eeeyre
 */

Datalog::Datalog(std::string infile, std::string outfile) {
	try {
		//outFile=outfile;
		S = new Scanner(infile);
		T = S->getMajorToken();
		match(SCHEMES);
		match(COLON);
		SchemeList();
		match(FACTS);
		match(COLON);
		FactList();
		match(RULES);
		match(COLON);
		RuleList();
		match(QUERIES);
		match(COLON);
		QueryList();
		match(END);
		//WriteOutput = SchemestoString();
		//WriteOutput += FactstoString();
		//WriteOutput += RulestoString();
		//WriteOutput += QueriestoString();
		//WriteOutput += DomaintoString();
		delete S;
	} catch (Token t) {
		delete S;
		//WriteOutput = "Failure!\n";
		//WriteOutput += "  "+t.toString();
		return;
	}

	//WriteOutput = "Success!\n"+WriteOutput;
}
void Datalog::SchemeList() {
	Schemes.push_back(Scheme());
	if (!check(FACTS)) {
		SchemeList();
	}

}
void Datalog::FactList() {
	if (!check(RULES)) {
		Facts.push_back(Fact());
		FactList();
	}
}
void Datalog::RuleList() {
	if (!check(QUERIES)) {
		Rules.push_back(Rule());
		RuleList();
	}
}
void Datalog::QueryList() {
	Queries.push_back(Query());
	if (!check(END)) {
		QueryList();
	}
}
PredicateStorage Datalog::Scheme() {
	return Predicate();
}
PredicateStorage Datalog::Fact() {
	PredicateStorage P = Predicate();
	match(PERIOD);
	return P;
}
RuleStorage Datalog::Rule() {
	RuleStorage R;
	R.Head = Predicate();
	match(COLON_DASH);
	R.Tail = PredicateList();
	match(PERIOD);
	return R;
}
PredicateStorage Datalog::Query() {
	PredicateStorage P = Predicate();
	match(Q_MARK);
	return P;
}
std::vector<PredicateStorage> Datalog::PredicateList() {
	std::vector<PredicateStorage> V;
	V.push_back(Predicate());
	while (check(COMMA)) {
		match(COMMA);
		V.push_back(Predicate());
	}
	return V;
}
PredicateStorage Datalog::Predicate() {
	PredicateStorage PS;
	PS.Name = T.value;
	match(ID);
	match(LEFT_PAREN);
	PS.Params = (ParameterList());
	match(RIGHT_PAREN);
	return PS;
}
ParameterStorage Datalog::ParameterList() {
	ParameterStorage V;
	V.Param.push_back(Parameter());
	while (check(COMMA)) {
		match(COMMA);
		V.Param.push_back(Parameter());
	}
	return V;
}
std::string Datalog::Parameter() {
	std::string ret = T.value;
	if (check(STRING)) {
		match(STRING);
		ret = "'" + ret;
		ret += "'";
		Domain.insert(ret);
	} else {
		match(ID);
	}

	return ret;

}
bool Datalog::check(Kind K) {
	if (T.kind == K) {
		return true;
	}
	return false;
}
bool Datalog::match(Kind K) {
	if (T.kind == K) {
		if (T.kind != END)
			T = S->getMajorToken();
		return true;
	}
	throw(T);
}

std::string Datalog::SchemestoString() {
	std::ostringstream convert;
	convert << Schemes.size();
	std::string output = "Schemes(" + convert.str();
	output += "):\n";
	for (unsigned int i = 0; i < Schemes.size(); i++) {
		output += "  " + Schemes[i].toString();
		output += "\n";
	}
	return output;
}
std::string Datalog::FactstoString() {
	std::ostringstream convert;
	convert << Facts.size();
	std::string output = "Facts(" + convert.str();
	output += "):\n";
	for (unsigned int i = 0; i < Facts.size(); i++) {
		output += "  " + Facts[i].toString();
		output += "\n";
	}
	return output;
}
std::string Datalog::RulestoString() {
	std::ostringstream convert;
	convert << Rules.size();
	std::string output = "Rules(" + convert.str();
	output += "):\n";
	for (unsigned int i = 0; i < Rules.size(); i++) {
		output += "  " + Rules[i].toString();
		output += "\n";
	}
	return output;
}
std::string Datalog::QueriestoString() {
	std::ostringstream convert;
	convert << Queries.size();
	std::string output = "Queries(" + convert.str();
	output += "):\n";
	for (unsigned int i = 0; i < Queries.size(); i++) {
		output += "  " + Queries[i].toString();
		output += "\n";
	}
	return output;
}
std::string Datalog::DomaintoString() {
	std::ostringstream convert;
	convert << Domain.size();
	std::string output = "Domain(" + convert.str();
	output += "):\n";
	std::set<int>::iterator iter;
	for (std::set<std::string>::iterator i = Domain.begin(); i != Domain.end();
			i++) {
		output += "  " + *i;
		output += "\n";
	}
	return output;
}
void Datalog::write() {
	//std::remove (outFile.c_str());
	//std::ofstream ofile;
	//ofile.open (outFile.c_str(), std::ios::app);
	//ofile << WriteOutput;
	//ofile.close();
}

std::string KindtoString(Kind k) {
	std::string result = "";
	switch (k) {
	case S:
		result = "START";
		break;
	case COMMA:
		result = "COMMA";
		break;
	case PERIOD:
		result = "PERIOD";
		break;
	case Q_MARK:
		result = "Q_MARK";
		break;
	case LEFT_PAREN:
		result = "LEFT_PAREN";
		break;
	case RIGHT_PAREN:
		result = "RIGHT_PAREN";
		break;
	case COLON:
		result = "COLON";
		break;
	case COLON_DASH:
		result = "COLON_DASH";
		break;
	case SCHEMES:
		result = "SCHEMES";
		break;
	case FACTS:
		result = "FACTS";
		break;
	case RULES:
		result = "RULES";
		break;
	case QUERIES:
		result = "QUERIES";
		break;
	case ID:
		result = "ID";
		break;
	case STRING:
		result = "STRING";
		break;
	case NUL:
		result = "";
		break;
	case END:
		result = "EOF";
		break;
	case UNKNOWN:
		result = "unknown";
		break;
	}
	return result;
}

Token::Token(Kind k, std::string v, unsigned int l) {
	kind = k;
	value = v;
	line_num = l;
}
Token::Token() {
	kind = S;
	value = "";
	line_num = 0;
}

std::string Token::toString() {
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
Scanner::Scanner(std::string infile) {
	ifile.open(infile.c_str());

	line = 1;
	count = 0;
	std::remove(filename.c_str());
	statecheck = START;
}

Token Scanner::getMajorToken() {
	Token check = getNextToken();
	while (check.kind == NUL) {
		check = getNextToken();
	}
	if (check.kind == UNKNOWN)
		fail();
	else {
		if (check.kind != END)
			count++;
	}
	return check;
}

Token Scanner::getNextToken() {

	if (statecheck != SID && statecheck != COD)
		ifile >> std::noskipws >> c;
	statecheck = START;
	if (!ifile.std::ios::fail()) {
		std::string ch = std::string(1, c);
		switch (c) {
		case '#':
			return comment();
			break;
		case EOF:
			return Token(END, "", line);
			break;
		case '\n':
			line++;
			return Token(NUL, "newline", line);
			break;
		case '\r':
			line++;
			return Token(NUL, "newline", line);
			break;
		case ',':
			return Token(COMMA, ch, line);
			break;
		case '.':
			return Token(PERIOD, ch, line);
			break;
		case '?':
			return Token(Q_MARK, ch, line);
			break;
		case '(':
			return Token(LEFT_PAREN, ch, line);
			break;
		case ')':
			return Token(RIGHT_PAREN, ch, line);
			break;
		default:
			if (std::isspace(c)) {
				return Token(NUL, "Whitespace", line);
			} else
				return complexToken();
			break;

		}
	} else
		return Token(END, "#", line);
}

Token Scanner::complexToken() {
	if (c == '\'') {
		return stringToken();
	} else if (std::isalpha(c)) {
		return idToken();
	} else if (c == ':') {
		return colonDash();
	} else
		return Token(UNKNOWN, "", line);

}

Token Scanner::stringToken() {
	std::ostringstream s;
	int linestart = line;
	ifile >> std::noskipws >> c;
	while (c != '\'' && c != EOF && !ifile.std::ios::eof() && c != '\n'
			&& c != '\r') {
		s << c;
		ifile >> std::noskipws >> c;

	}
	if (c != '\'') {
		line = linestart;
		return Token(UNKNOWN, "unterminated string", line);
	}
	return Token(STRING, s.str(), linestart);
}
Token Scanner::idToken() {
	statecheck = SID;
	std::ostringstream s;
	while (!isspace(c) && isalnum(c)) {
		s << c;
		ifile >> std::noskipws >> c;
	}
	if (s.str() == "Facts")
		return Token(FACTS, s.str(), line);
	else if (s.str() == "Rules")
		return Token(RULES, s.str(), line);
	else if (s.str() == "Queries")
		return Token(QUERIES, s.str(), line);
	else if (s.str() == "Schemes")
		return Token(SCHEMES, s.str(), line);

	return Token(ID, s.str(), line);
}
Token Scanner::colonDash() {
	ifile >> std::noskipws >> c;
	if (c != '-') {
		statecheck = COD;
		return Token(COLON, ":", line);
	} else {
		return Token(COLON_DASH, ":-", line);
	}
}
Token Scanner::comment() {
	while (c != '\r' && c != '\n') {
		ifile >> std::noskipws >> c;
	}
	line++;
	return Token(NUL, "Comment", line);
}
void Scanner::fail() {
	std::ostringstream convert;
	convert << line;
	std::string out = "";
	out += "Input Error on line ";
	out += convert.str();
//	write(out);
}
void Scanner::write(std::string output) {
	//ofile.open (filename.c_str(), std::ios::app);
	//ofile << output << std::endl;
	//ofile.close();
}
void Scanner::closewrite() {
	std::ostringstream convert;
	convert << count;
	std::string out = "";
	out += "Total Tokens = ";
	out += convert.str();
//	write(out);
}
