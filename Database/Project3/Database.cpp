/*
 * Database.cpp
 *
 *  Created on: Oct 25, 2014
 *      Author: eeeyre
 */

#include "Database.h"
#include <iostream>

Database::Database(char* argv1, char* argv2) {
	// TODO Auto-generated constructor stub
	D = Datalog(argv1, argv2);
	outfile = argv2;
	std::remove(outfile.c_str());
	ofile.open(outfile.c_str(), std::ios::app);
	EvaluateSchemas();
	EvaluateFacts();
	ReportFacts();
	EvaluateQueries();

}

Database::~Database() {
	// TODO Auto-generated destructor stub
	ofile.close();
}

void Database::EvaluateQueries() {
	write("Query Evaluation\n\n");
	for (int unsigned i = 0; i < D.Queries.size(); i++) {
		Temp = relations[D.Queries.at(i).Name];
		TempQS.clear();
		for (int unsigned q = 0; q < D.Queries.at(i).Params.Param.size(); q++) {
			TempQS.push_back(D.Queries.at(i).Params.Param.at(q));
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
	}

}

void Database::Project_to_String(Relation RT) {
	write("project");
	if (RT.schema.size()>0)
		write("\n");
	for (auto i : RT.Tuples) {
		if (RT.schema.size() > 0) {
			write("  ");
			write(RT.schema.at(0) + "=");
			write(i.at(0));
		}
		for (int unsigned j = 1; j < i.size(); j++) {
			write(" ");
			write(RT.schema.at(j) + "=");
			write(i.at(j));
		}
		write("\n");
	}
}

Relation Database::Project() {
	Relation PT;
	std::vector<int>* pos1 = NULL;
	pos1 = GetPositionstoProject();
	std::vector<int> pos = *pos1;
	delete pos1;
	PT.Name = Temp.Name;
	for (int unsigned i = 0; i < pos.size(); i++) {
		PT.schema.push_back(Temp.schema.at(pos.at(i)));
	}
	for (auto i : Temp.Tuples) {
		Tuple T;
		for (int unsigned j = 0; j < pos.size(); j++) {
			T.push_back(i.at(pos.at(j)));
		}
		PT.Tuples.insert(T);
	}
	return PT;
}

std::vector<int>* Database::GetPositionstoProject() {
	std::vector<int>* pos = new std::vector<int>();
	for (int unsigned i = 0; i < TempQS.size(); i++) {
		if (TempQS.at(i)[0] != '\'') {
			bool found = false;
			for (int unsigned j = 0; j < i; j++) {
				if (TempQS.at(i) == TempQS.at(j)) {
					found = true;
				}
			}
			if (!found) {
				pos->push_back(i);
			}
		}
	}
	return pos;
}

std::string Database::QuerytoString() {
	std::string output = Temp.Name + "(";
	output += Temp.schema.at(0);
	for (int unsigned i = 1; i < Temp.schema.size(); i++) {
		output += "," + Temp.schema.at(i);
	}
	output += ")?";
	return output;
}

Relation Database::Rename(Relation PR) {
	Relation TR;
	std::vector<int>* pos1 = NULL;
	pos1 = GetPositionstoProject();
	std::vector<int> pos = *pos1;
	delete pos1;
	for (int unsigned i = 0; i < pos.size(); i++) {
		TR.schema.push_back(TempQS.at(pos.at(i)));
	}
	for (auto i : PR.Tuples) {
		TR.Tuples.insert(i);
	}
	return TR;
}

void Database::printRename(Relation TR) {
	write("rename\n");
	for (auto i : TR.Tuples) {
		if (TR.schema.size() > 0) {
			write("  ");
			write(TR.schema.at(0));
			write("=" + i.at(0));
			for (int unsigned j = 1; j < TR.schema.size(); j++) {
				write(" " + TR.schema.at(j));
				write("=" + i.at(j));
			}
			write("\n");
		}
	}
}

void Database::ReportQueries() {
	int unsigned results = Temp.Tuples.size();
	std::string output = "";
	output += Temp.Name + "(" + TempQS.at(0);
	for (int unsigned i = 1; i < TempQS.size(); i++) {
		output += "," + TempQS.at(i);
	}
	output += ")?";
	if (results != 0) {
		output += " Yes(" + std::to_string(results);
		output += ")\nselect\n";
		for (auto j : Temp.Tuples) {
			output += "  ";
			output += Temp.schema.at(0);
			output += "=";
			output += j.at(0);
			for (int unsigned k = 1; k < j.size(); k++) {
				output += " ";
				output += Temp.schema.at(k);
				output += "=";
				output += j.at(k);
			}
			output += "\n";
		}
		write(output);
		Relation PRR = Project();
		Project_to_String(PRR);
		Relation RER = Rename(PRR);
		printRename(RER);
	} else {
		output += " No\n";
		write(output);
	}
write("\n");

}

Relation Database::select(int unsigned position, std::string value) {
	Relation R;
	R.Name = Temp.Name;
	R.schema = Temp.schema;
	for (auto i : Temp.Tuples) {
		if (i.at(position) == value) {
			R.Tuples.insert(i);
		}
	}
	return R;
}

Relation Database::select(int unsigned position1, int unsigned position2) {
	Relation R;
	R.Name = Temp.Name;
	R.schema = Temp.schema;
	for (auto i : Temp.Tuples) {
		if (i.at(position1) == i.at(position2)) {
			R.Tuples.insert(i);
		}
	}
	return R;
}

void Database::ReportFacts() {
	for (auto i : relations) {
		write(i.second.Name + "\n");
		int count = 1;
		for (auto j : i.second.Tuples) {
			write("  ");
			write(i.second.schema.at(0));
			write("=");
			write(j.at(0));
			for (int unsigned k = 1; k < j.size(); k++) {
				write(" ");
				write(i.second.schema.at(k));
				write("=");
				write(j.at(k));

			}
			count++;
			write("\n");

		}
		write("\n");
	}
}

void Database::EvaluateFacts() {

	for (int unsigned i = 0; i < D.Facts.size(); i++) {
		Tuple T;
		for (int unsigned j = 0; j < D.Facts.at(i).Params.Param.size(); j++) {
			T.push_back(D.Facts.at(i).Params.Param.at(j));
		}
		relations[D.Facts.at(i).Name].Tuples.insert(T);
	}
	write("Fact Evaluation\n\n");
}
void Database::EvaluateSchemas() {
	for (int unsigned i = 0; i < D.Schemes.size(); i++) {
		Schema S;
		for (int unsigned j = 0; j < D.Schemes.at(i).Params.Param.size(); j++) {
			S.push_back(D.Schemes.at(i).Params.Param.at(j));
		}
		//write(")");

		relations[D.Schemes.at(i).Name] = Relation(S, D.Schemes.at(i).Name);
	}
	write("Scheme Evaluation\n\n");
}

void Database::write(std::string output) {
	ofile << output;
}
