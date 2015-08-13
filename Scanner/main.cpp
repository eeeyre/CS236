/*
 * main.cpp

 *
 *  Created on: Sep 6, 2014
 *      Author: eeeyre
 */
#include <iostream>
#include "Util.h"
#include "Scanner.h"

int main(int argc, char* argv[]) {
	Scanner* S = new Scanner (argv[1],argv[2]);
	Token gt = S->getMajorToken();
	while (gt.kind != NUL && gt.kind != END && gt.kind!=UNKNOWN)
	{
		S->write(gt.toString());
		gt = S->getMajorToken();
	}
	if (gt.kind!=UNKNOWN)
		S->closewrite();
	delete S;
	return 1;
}
