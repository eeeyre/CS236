/*
 * Database.h
 *
 *  Created on: Oct 25, 2014
 *      Author: eeeyre
 */
#include "Util.h"
#include <map>

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
	void write(std::string output);
	std::string outfile;
	Relation select(int unsigned position, std::string value);
	Relation select(int unsigned position1, int unsigned position2);
	Relation Temp;
	Relation Proj;
	/*
	 * The following Public functions are for Project 4
	 * Other functions related to Database, but
	 * were used for Project 3 have been moved to Util.cpp
	 */

	void EvaluateRules();
	/*
	 * The following Public functions are for Project 5
	 * Note that Project 4 functions have been moved to Util.cpp
	 * but they may not be in use for this project.
	 */
	void QueryDependencies();
	void RulesDependencies();
	void EvaluateQueries(PredicateStorage curQuer);
	void WriteQueryHeader(PredicateStorage i);

private:
	Schema TempQS;
	std::string QuerytoString();
	void printRename(Relation TR);
	Relation Project();
	Relation Rename(Relation PR);
	std::vector<int>* GetPositionstoProject();
	void Project_to_String(Relation RT);
	std::ofstream ofile;
	/*
	 * The following private functions are for Project 4
	 * Other functions related to Database, but
	 * were used for Project 3 have been moved to Util.cpp
	 * */
	void jselect(Relation &r, Schema s);
	void printRule(Relation r);
	Relation join(Relation r1, Relation r2);
	Tuple newtup(Tuple t1, Tuple t2, Schema s1, Schema s2, Schema joinschema);
	int currentrule = 0;
	int find(std::vector<std::string> domain, std::string value);
	void printRelation(Relation r);
	Schema newScheme(Schema p1, Schema p2);
	bool checkjoin(Tuple t1, Tuple t2, Schema s1, Schema s2, Schema joinschema);
	void insertNewFacts(Relation r);
	Relation projectRule(Relation r, Schema qs);
	int getsize();
	int getsize(std::string name);
	void ReportRules();
	int converge = 0;
	void writeRelation(Relation r);
	/*
	 * The following Public functions are for Project 5
	 * Note that Project 4 functions have been moved to Util.cpp
	 * but they may not be in use for this project.
	 */
	std::map<std::string, Node > dependencyGraph;
	void CreateDependencyGraph();
	void PrintDependencyGraph();
	void GetPostOrder(int i);
	void WritePostOrder(int i);
	void ResetPostOrder();
	int assignPostOrder(Node &n,int start);
	std::set<std::string> visited;
	void findEvalOrder(int i);
	void findBackwardsEdges(std::string i);
	std::vector<int> executeorder;
	std::set<std::string> bedge;
	void WriteBackwardsEdges();
}
;

#endif /* DATABASE_H_ */

