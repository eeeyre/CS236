/*
 * Util.cpp
 *
 *  Created on: Sep 6, 2014
 *      Author: eeeyre
 */
#include "Database.h"
#include "Util.h"
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
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
	}
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
	if (RT.schema.size() > 0)
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
//write("\n");

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
	//std::cout<<output;
}

void Database::EvaluateRules() {
	write("Rule Evaluation\n");
	int size = -1;
	int testsize = getsize();
	while (size != testsize) {
		size = testsize;
		currentrule = 0;
		for (int unsigned a = 0; a < executeorder.size(); a++) { //Step through each rule
			currentrule = executeorder.at(a);
			Relation r1;
			Relation r2;
			Schema qs;
			Schema rs;
			r1 = relations[D.Rules.at(currentrule).Tail.at(0).Name];
			jselect(r1, D.Rules.at(currentrule).Tail.at(0).Params.Param);
			r1.Name = D.Rules.at(currentrule).Tail.at(0).Name;
			r1.schema = D.Rules.at(currentrule).Tail.at(0).Params.Param;
			//writeRelation(r1);
			if (D.Rules.at(currentrule).Tail.size() > 1) {
				for (int unsigned o = 1;
						o < D.Rules.at(currentrule).Tail.size(); o++) {
					r2 = relations[D.Rules.at(currentrule).Tail.at(o).Name];
					jselect(r2,
							D.Rules.at(currentrule).Tail.at(o).Params.Param);
					r2.schema = D.Rules.at(currentrule).Tail.at(o).Params.Param;
					//writeRelation(r2);
					r1 = join(r1, r2);
					r1.Name = D.Rules.at(currentrule).Head.Name;
					r1.schema = newScheme(r1.schema,
							D.Rules.at(currentrule).Tail.at(o).Params.Param);
					//writeRelation(r1);
				}
			}
			r1 = projectRule(r1, D.Rules.at(currentrule).Head.Params.Param); // Should this schema be the real schema or the query schema?
			r1.Name = D.Rules.at(currentrule).Head.Name;
			jselect(r1, D.Rules.at(currentrule).Head.Params.Param);
			//printRelation(r1);
			r1.schema = relations[D.Rules.at(currentrule).Head.Name].schema;
			insertNewFacts(r1);

		}
		converge++;
		if (bedge.size() == 0) {
			break;
		}
		testsize = getsize();
	}
}

void Database::ReportRules() {
	write("\nConverged after " + std::to_string(converge));
	write(" passes through the Rules.\n\n");
	for (auto a : relations) {
		if (a.second.schema.size() > 0) {
			write(a.second.Name + "\n");
			for (auto i : a.second.Tuples) {
				if (i.size() > 0) {
					write("  ");
					write(a.second.schema.at(0) + "=" + i.at(0));

					for (int unsigned j = 1; j < i.size(); j++) {
						write(" ");
						write(a.second.schema.at(j) + "=" + i.at(j));
					}
				}
				write("\n");
			}
			write("\n");
		}
	}
}

Relation Database::projectRule(Relation r, Schema qs) {
	Relation out;
	out.Name = r.Name;
	out.schema = qs;
	for (auto f : r.Tuples) {
		Tuple t;
		for (int unsigned i = 0; i < qs.size(); i++) {
			int loc = find(r.schema, qs.at(i));
			if (loc != -1) {
				t.push_back(f.at(loc));
			}
		}
		out.Tuples.insert(t);
	}
	return out;
}
Relation Database::join(Relation r1, Relation r2) {
	Relation out;
	out.schema = newScheme(r1.schema, r2.schema);
	for (auto i : r1.Tuples) {
		for (auto j : r2.Tuples) {
			if (checkjoin(i, j, r1.schema, r2.schema, out.schema)) {
				Tuple t = newtup(i, j, r1.schema, r2.schema, out.schema);
				out.Tuples.insert(t);
			}
		}
	}
	return out;
}

bool Database::checkjoin(Tuple t1, Tuple t2, Schema s1, Schema s2,
		Schema joinschema) {
	Tuple t;
	for (int unsigned i = 0; i < joinschema.size(); i++) {
		int testint = find(s1, joinschema.at(i));
		int testint1 = find(s2, joinschema.at(i));
		if (testint != -1 && testint1 != -1
				&& t1.at(testint) != t2.at(testint1)) {
			return false;
		}
	}
	return true;
}

void Database::jselect(Relation &r, Schema s) {
	for (int unsigned k = 0; k < s.size(); k++) { //check each item in the Query Schema
		if (s.at(k)[0] == '\'') {
			Temp = r;
			r = select(k, s.at(k));
		} else {
			for (int unsigned l = 0; l < s.size(); l++) {
				if (k < l && s.at(k) == s.at(l)) {
					Temp = r;
					r = select(k, l);
				}
			}
		}
	}
	return;
}

Tuple Database::newtup(Tuple t1, Tuple t2, Schema s1, Schema s2,
		Schema joinschema) { //This joins 2 tuples if possible
	Tuple t;
	for (int unsigned i = 0; i < joinschema.size(); i++) {
		int testint = find(s1, joinschema.at(i));
		int testint1 = find(s2, joinschema.at(i));
		if (testint != -1 && testint1 == -1) {
			t.push_back(t1.at(testint));

		} else if (testint == -1 && testint1 != -1) {
			t.push_back(t2.at(testint1));

		} else {
			if (t1.at(testint) == t2.at(testint1)) {
				t.push_back(t1.at(testint));
			}
		}
	}
	return t;
}

Schema Database::newScheme(Schema p1, Schema p2) {
	Schema s;
	for (int unsigned i = 0; i < p1.size(); i++) {
		if (std::find(s.begin(), s.end(), p1[i]) == s.end()) {
			s.push_back(p1[i]);
		}
	}
	for (int unsigned i = 0; i < p2.size(); i++) {
		if (std::find(s.begin(), s.end(), p2[i]) == s.end()) {
			s.push_back(p2[i]);
		}
	}
	return s;
}

int Database::find(std::vector<std::string> domain, std::string value) {
	for (int unsigned i = 0; i < domain.size(); i++) {
		if (domain.at(i) == value) {
			return i;
		}
	}
	return -1;
}

void Database::printRule(Relation r) {
	if (r.schema.size() < 1)
		return;
	write(D.Rules[currentrule].toString());
	write("\n");
	for (auto i : r.Tuples) {
		write("  ");
		write(r.schema.at(0) + "=" + i.at(0));

		for (int unsigned j = 1; j < i.size(); j++) {
			write(" ");
			write(r.schema.at(j) + "=" + i.at(j));

		}
		write("\n");
	}
}

void Database::insertNewFacts(Relation r) {
	int size = getsize(r.Name);
	write(D.Rules[currentrule].toString());
			write("\n");
	if (r.Tuples.size() != 0) {
		for (auto p : r.Tuples) {
			if (p.size() != 0) {
				relations[r.Name].Tuples.insert(p);
				int testsize = getsize(r.Name);
				if (size != testsize) {

					write(" ");
					for (int unsigned m = 0; m < r.schema.size(); m++) {
						write(" ");
						write(r.schema.at(m) + "=" + p.at(m));
					}
					write("\n");
					size = testsize;
					//std::cout<<std::to_string(size)<<std::endl;
				}
			}
		}
	}
}

void Database::printRelation(Relation r) {
	if (r.schema.size() < 1) {
		//std::cout << "Empty Schema\n" << r.Name << std::endl << r.Tuples.size()<< std::endl << std::endl;
		return;
	}
	write(r.Name + "\n");
	for (auto i : r.Tuples) {
		if (i.size() > 0) {
			write("  ");
			write(r.schema.at(0) + "=" + i.at(0));

			for (int unsigned j = 1; j < i.size(); j++) {
				write(" ");
				write(r.schema.at(j) + "=" + i.at(j));

			}
		}
		write("\n");
	}
	write("\n");
}

void Database::writeRelation(Relation r) {
	if (r.schema.size() < 1) {
		//std::cout << "Empty Schema\n" << r.Name << std::endl << r.Tuples.size()<< std::endl << std::endl;
		return;
	}
	std::cout << (r.Name + "\n");
	for (auto i : r.Tuples) {
		if (i.size() > 0) {
			std::cout << ("  ");
			std::cout << (r.schema.at(0) + "=" + i.at(0));

			for (int unsigned j = 1; j < i.size(); j++) {
				std::cout << (" ");
				std::cout << (r.schema.at(j) + "=" + i.at(j));

			}
		}
		std::cout << ("\n");
	}
	std::cout << ("\n");
}

int Database::getsize() {
	int count = 0;
	for (auto i : relations) {
		count += i.second.Tuples.size();
	}
	return count;
}

int Database::getsize(std::string name) {
	return relations[name].Tuples.size();

}

Node::Node() {
	name = "";
}
Node::Node(std::string n) {
	name = n;
}
