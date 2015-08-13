/*
 * Database.cpp
 *
 *  Created on: Oct 25, 2014
 *      Author: eeeyre
 */
#include "Database.h"
#include <iostream>
#include <map>
#include <string>

Database::Database(char* argv1, char* argv2) {
	// TODO Auto-generated constructor stub
	executeorder.clear();
	bedge.clear();
	visited.clear();
	D = Datalog(argv1, argv2);
	outfile = argv2;
	std::remove(outfile.c_str());
	ofile.open(outfile.c_str(), std::ios::app);
	EvaluateSchemas();
	EvaluateFacts();
	ReportFacts();

	//EvaluateRules();
	//ReportRules();
	//EvaluateQueries();
	CreateDependencyGraph();
	write("Query Evaluation\n");
	//write(std::to_string(D.Queries.size()));
	for (int unsigned i = 0; i < D.Queries.size(); i++) {
		//PrintDependencyGraph();
		WriteQueryHeader(D.Queries.at(i));
		GetPostOrder(i);
		EvaluateRules();
		write("\n");
		EvaluateQueries(D.Queries.at(i));
		ResetPostOrder();
		executeorder.clear();
		bedge.clear();
		visited.clear();
	}
	write("\n");
}
void Database::findBackwardsEdges(std::string i) {
	visited.insert(i);
	std::string out = "";
	for (auto j : dependencyGraph[i].dependencies) {

		if (visited.find(j) == visited.end()) {
			visited.insert(j);
			findBackwardsEdges(j);
		}
		if (dependencyGraph[j].postOrder >= dependencyGraph[i].postOrder) {
			out += " " + j;
		}

	}
	if (out.size() > 1)
		bedge.insert(i + ":" + out);
	return;
}
void Database::WriteBackwardsEdges() {
	write("Backward Edges\n");
	for (auto j : bedge) {
		write("  " + j);
		write("\n");
	}
	write("\n");
}
void Database::findEvalOrder(int i) {
	write("Rule Evaluation Order\n");
	int post = 1;
	int rule = 0;
	while (post <= i) {
		for (auto j : dependencyGraph) {
			if (j.second.postOrder == post) {
				rule = std::stoi(j.second.name.erase(0, 1));
				executeorder.push_back(rule - 1);
				write("  R" + std::to_string(rule));
				write("\n");
				post++;
			}
		}
	}
	write("\n");
}
void Database::ResetPostOrder() {
	for (auto &i : dependencyGraph) {
		i.second.postOrder = 0;
	}
}
void Database::GetPostOrder(int i) {
	//visited.clear();
	//ResetPostOrder();
	int total = assignPostOrder(dependencyGraph["Q" + std::to_string(i + 1)],
			1);
	//write("\n\nTotal = " + std::to_string(total));
	WritePostOrder(i);
	visited.clear();
	findEvalOrder(total - 2);
	findBackwardsEdges("Q" + std::to_string(i + 1));
	WriteBackwardsEdges();
}
void Database::WritePostOrder(int i) {
	write("\nPostorder Numbers\n");
	for (auto i : dependencyGraph) {
		if (i.second.postOrder != 0) {
			std::string out = "  " + i.first;
			out += ": " + std::to_string(i.second.postOrder);
			out += "\n";
			write(out);
		}
	}
	write("\n");
}
int Database::assignPostOrder(Node &n, int start) {
	int posto = start;
	visited.insert(n.name);
	for (auto i : n.dependencies) {
		if (visited.find(i) == visited.end())
			posto = assignPostOrder(dependencyGraph[i], posto);
	}
	n.postOrder = posto;
	posto++;
	//write("\nposto= "+std::to_string(posto));
	return posto;
}
void Database::WriteQueryHeader(PredicateStorage i) {
	std::string output = "\n";
	output += i.Name + "(" + i.Params.Param.at(0);
	for (int unsigned j = 1; j < i.Params.Param.size(); j++) {
		output += "," + i.Params.Param.at(j);
	}
	output += ")?\n";
	write(output);
}
void Database::EvaluateQueries(PredicateStorage curQuer) {
	//std::map<std::string, Relation> protect = relations;
	Temp = relations[curQuer.Name];
	TempQS.clear();
	for (int unsigned q = 0; q < curQuer.Params.Param.size(); q++) {
		TempQS.push_back(curQuer.Params.Param.at(q));
	}
	for (int unsigned k = 0; k < TempQS.size(); k++) {
		if (TempQS.at(k)[0] == '\'') {
			Temp = select(k, TempQS.at(k));
		} else {
			for (int unsigned l = 0; l < TempQS.size(); l++) {
				if (k < l && TempQS.at(k) == TempQS.at(l)) {
					Temp = select(k, l);
				}
			}
		}
	}
	ReportQueries();
	//relations = protect;
}
void Database::CreateDependencyGraph() {
	QueryDependencies();
	RulesDependencies();
	PrintDependencyGraph();
}

void Database::QueryDependencies() {
	for (int unsigned i = 0; i < D.Queries.size(); i++) {
		Node insert;
		insert.name = "Q" + std::to_string(i + 1);
		for (int unsigned j = 0; j < D.Rules.size(); j++) {
			if (D.Rules.at(j).Head.Name == D.Queries.at(i).Name) {
				insert.dependencies.insert("R" + std::to_string(j + 1));
			}
		}
		dependencyGraph["Q" + std::to_string(i + 1)] = insert;
	}
}
void Database::RulesDependencies() {
	for (int unsigned i = 0; i < D.Rules.size(); i++) {
		Node insert;
		insert.name = "R" + std::to_string(i + 1);
		for (int unsigned j = 0; j < D.Rules.at(i).Tail.size(); j++) {
			for (int unsigned l = 0; l < D.Rules.size(); l++) {
				if (D.Rules.at(i).Tail.at(j).Name == D.Rules.at(l).Head.Name) {
					insert.dependencies.insert("R" + std::to_string(l + 1));
				}
			}
		}
		dependencyGraph["R" + std::to_string(i + 1)] = insert;
	}
}
void Database::PrintDependencyGraph() {
	write("Dependency Graph\n");
	for (auto i : dependencyGraph) {
		write("  ");
		write(i.first + ":");
		write("");
		for (auto j : i.second.dependencies) {
			write(" " + j);
		}
		write("\n");
	}
	write("\n");
}

