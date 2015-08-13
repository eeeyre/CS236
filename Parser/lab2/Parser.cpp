/*
 * Parser.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: eeeyre
 */

#include "Parser.h"
#include <iostream>
#include <fstream>

Datalog::Datalog(std::string infile, std::string outfile) {
	try {
		outFile=outfile;
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
		WriteOutput = SchemestoString();
		WriteOutput += FactstoString();
		WriteOutput += RulestoString();
		WriteOutput += QueriestoString();
		WriteOutput += DomaintoString();
		delete S;
	} catch (Token t) {
		delete S;
		WriteOutput = "Failure!\n";
		WriteOutput += "  "+t.toString();
		return;
	}

	WriteOutput = "Success!\n"+WriteOutput;
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
	for (std::set<std::string>::iterator i = Domain.begin(); i != Domain.end();i++) {
		output += "  "+*i;
		output += "\n";
	}
	return output;
}
void Datalog::write()
{
	std::remove (outFile.c_str());
	std::ofstream ofile;
	ofile.open (outFile.c_str(), std::ios::app);
	ofile << WriteOutput;
	ofile.close();
}
