/*
 * main.cpp

 *
 *  Created on: Sep 6, 2014
 *      Author: eeeyre
 */
#include <iostream>
#include "Util.h"
#include "Database.h"

int main(int argc, char* argv[]) {
	if (argc < 3) {
		std::cout << "Please include an input and output file" << std::endl;
		return 1;
	}
	Database D(argv[1], argv[2]);

	return 0;
}
