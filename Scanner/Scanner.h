/*
 * Scanner.h
 *
 *  Created on: Sep 6, 2014
 *      Author: eeeyre
 */
#include "Util.h"
#include <fstream>

#ifndef SCANNER_H_
#define SCANNER_H_

class Scanner {
public:
	Scanner(std::string infile,std::string opfile);
	virtual ~Scanner(){};
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
	State statecheck = START;
	char c;

};

#endif /* SCANNER_H_ */
