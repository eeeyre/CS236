/*
 * Database.h
 *
 *  Created on: Oct 25, 2014
 *      Author: eeeyre
 */
#include "Util.h"
#include <map>
#ifndef TUPLE_H_
#define TUPLE_H_

class Tuple: public std::vector<std::string> {
public:
	Tuple() {
	}
	;
	virtual ~Tuple() {
	}
	;
};

#endif /* TUPLE_H_ */

#ifndef SCHEMA_H_
#define SCHEMA_H_

class Schema: public std::vector<std::string> {
public:
	Schema() {
	}
	Schema(std::vector<std::string> p)
	{
		for (int unsigned i=0;i<p.size();i++)
		{
			push_back(p.at(i));
		}
	}
	virtual ~Schema() {
	}
};

#endif /* SCHEMA_H_ */

#ifndef RELATION_H_
#define RELATION_H_

class Relation {
public:
	Relation() {
	}
	;
	Relation(Schema S, std::string n) {
		schema = S;
		Name = n;
	}
	virtual ~Relation() {
	}
	;
	std::set<Tuple> Tuples;
	std::string Name;
	Schema schema;

};

#endif /* RELATION_H_ */

#ifndef DATABASE_H_
#define DATABASE_H_

class Database {
public:
	Database(char* argv1, char* argv2);
	virtual ~Database();
	Datalog D;
	std::map<std::string, Relation> relations;
	void EvaluateSchemas();
	void EvaluateFacts();
	void ReportFacts();
	void EvaluateQueries();
	void ReportQueries();
	void EvaluateRules();
	void ReportRules();
	void write(std::string output);
	std::string outfile;
	Relation select(int unsigned position, std::string value);
	Relation select(int unsigned position1, int unsigned position2);
	Relation Temp;
	Relation Temp2;
	Relation Proj;
	Relation r1;
	Relation r2;
	Schema joinschema;
	Relation join(PredicateStorage p1, PredicateStorage p2);
	Relation join(Relation p1, PredicateStorage p2);
	Schema newScheme(Schema p1, Schema p2);
	int getsize();
	void insertfacts(Relation r);
	bool checkjoin(Tuple t1, Tuple t2);
	Tuple newtup(Tuple t1, Tuple t2);
	Relation rulecalc(Schema s, RuleStorage r);
	void joinselect(PredicateStorage p1);
	void joinselect(Schema p1);

private:
	int currentrule=0;
	void printRule(Relation r);
	Schema TempQS;
	int converge = 0;
	void printRelation(Relation r);
	//std::map<std::string,Relation> RulesOut;
	Relation projectrules(Relation r);
	std::string QuerytoString();
	void printRename(Relation TR);
	Relation Project();
	Relation Rename(Relation PR);
	std::vector<int>* GetPositionstoProject();
	void Project_to_String(Relation RT);
	std::ofstream ofile;
	int find(std::vector<std::string> domain, std::string value);
}
;

#endif /* DATABASE_H_ */

