/*
 * Parser.h
 *
 *  Created on: Sep 12, 2014
 *      Author: eeeyre
 */
#include "Util.h"
#include <vector>
#include <set>
#include <sstream>

#ifndef Parameter_H_
#define Parameter_H_
class ParameterStorage{
public:
	ParameterStorage(){};
	virtual ~ParameterStorage(){};
	std::vector<std::string> Param;
	std::string toString()
	{
		std::string output;
		if (Param.size() > 0)
		{
			output = Param[0];
		}
		for (unsigned int i=1;i<Param.size();i++)
		{
			output += ","+Param[i];
		}
		return output;
	}

};
#endif

#ifndef Predicate_H_
#define Predicate_H_
class PredicateStorage{
public:
	PredicateStorage(){};
	virtual ~PredicateStorage(){};
	std::string Name;
	ParameterStorage Params;
	std::string toString()
	{
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
class RuleStorage
{
public:
	RuleStorage(){};
	virtual ~RuleStorage(){};
	PredicateStorage Head;
	std::vector<PredicateStorage> Tail;
	std::string toString()
	{
		std::string output;
		output = Head.toString() + " :- ";
		output += Tail[0].toString();
		for (unsigned int i=1;i<Tail.size();i++)
		{
			output+=","+Tail[i].toString();
		}
		return output;
	}
};
#endif


#ifndef DATALOG_H_
#define DATALOG_H_
class Datalog{
public:
	Datalog(std::string infile,std::string outfile);
	virtual ~Datalog(){};
	std::string SchemestoString();
	std::string FactstoString();
	std::string RulestoString();
	std::string QueriestoString();
	std::string DomaintoString();
	void write();
private:
	Token T;
	Scanner* S;
	std::vector<PredicateStorage> Facts;
	std::vector<PredicateStorage> Schemes;
	std::vector<RuleStorage> Rules;
	std::vector<PredicateStorage> Queries;
	std::set <std::string> Domain;
	void SchemeList();
	void FactList();
	void RuleList();
	void QueryList();
	PredicateStorage Scheme();
	PredicateStorage Fact();
	RuleStorage Rule();
	PredicateStorage Query();
	std::vector<PredicateStorage> PredicateList();
	PredicateStorage Predicate();
	ParameterStorage ParameterList();
	std::string Parameter();
	bool check(Kind K);
	bool match(Kind K);
	std::string outFile;
	std::string WriteOutput;

};
#endif

