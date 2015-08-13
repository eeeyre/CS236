/*
 * Util.h
 *
 *  Created on: Sep 6, 2014
 *      Author: eeeyre
 */
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>

#ifndef Kind_H_
#define Kind_H_

enum Kind {
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

enum State {
	START, RULE, SID, COD
};
#endif /* State_H_ */

#ifndef Token_H_
#define Token_H_

class Token {
public:
	Token(Kind k, std::string v, unsigned int l);
	Token();
	virtual ~Token() {
	}
	;
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
	virtual ~Scanner() {
	}
	;
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

/*
 * Parser.h
 *
 *  Created on: Sep 12, 2014
 *      Author: eeeyre
 */

#ifndef Parameter_H_
#define Parameter_H_
class ParameterStorage {
public:
	ParameterStorage() {
	}
	;
	virtual ~ParameterStorage() {
	}
	;
	std::vector<std::string> Param;
	std::string toString() {
		std::string output;
		if (Param.size() > 0) {
			output = Param[0];
		}
		for (unsigned int i = 1; i < Param.size(); i++) {
			output += "," + Param[i];
		}
		return output;
	}
	std::vector<std::string> returnparams() {
		return Param;
	}

};
#endif

#ifndef Predicate_H_
#define Predicate_H_
class PredicateStorage {
public:
	PredicateStorage() {
	}
	;
	virtual ~PredicateStorage() {
	}
	;
	std::string Name;
	ParameterStorage Params;
	std::string toString() {
		std::string output = Name;
		output += "(";
		output += Params.toString();
		output += ")";
		return output;
	}
};
#endif

#ifndef RULE_H_
#define RULE_H_
class RuleStorage {
public:
	RuleStorage() {
	}
	;
	virtual ~RuleStorage() {
	}
	;
	PredicateStorage Head;
	std::vector<PredicateStorage> Tail;
	std::string toString() {
		std::string output;
		output = Head.toString() + " :- ";
		output += Tail[0].toString();
		for (unsigned int i = 1; i < Tail.size(); i++) {
			output += "," + Tail[i].toString();
		}
		return output;
	}
};
#endif

#ifndef DATALOG_H_
#define DATALOG_H_
class Datalog {
public:
	Datalog() {
	}
	;
	Datalog(std::string infile, std::string outfile);
	virtual ~Datalog() {
	}
	;
	std::string SchemestoString();
	std::string FactstoString();
	std::string RulestoString();
	std::string QueriestoString();
	std::string DomaintoString();
	void write();
	std::vector<PredicateStorage> Facts;
	std::vector<PredicateStorage> Schemes;
	std::vector<RuleStorage> Rules;
	std::vector<PredicateStorage> Queries;
private:
	Token T;
	Scanner* S;
	std::set<std::string> Domain;
	PredicateStorage Scheme();
	PredicateStorage Fact();
	RuleStorage Rule();
	PredicateStorage Query();
	void SchemeList();
	void FactList();
	void RuleList();
	void QueryList();

	std::vector<PredicateStorage> PredicateList();
	PredicateStorage Predicate();
	ParameterStorage ParameterList();
	std::string Parameter();
	bool check(Kind K);
	bool match(Kind K);
	//std::string outFile;
	//std::string WriteOutput;

};
#endif

