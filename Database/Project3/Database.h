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
	void write(std::string output);
	std::string outfile;
	Relation select(int unsigned position, std::string value);
	Relation select(int unsigned position1, int unsigned position2);
	Relation Temp;
	Relation Proj;
private:
	Schema TempQS;
	std::string QuerytoString();
	void printRename(Relation TR);
	Relation Project();
	Relation Rename(Relation PR);
	std::vector<int>* GetPositionstoProject();
	void Project_to_String(Relation RT);
	std::ofstream ofile;
}
;

#endif /* DATABASE_H_ */

