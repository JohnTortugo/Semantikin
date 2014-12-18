/*
 * ErrorReporting.h
 *
 *  Created on: Dec 18, 2014
 *      Author: divcesar
 */

#ifndef ERRORREPORTING_H_
#define ERRORREPORTING_H_

#include <string>

using std::string;

class ParsingError {
public:
	static int findPos(const std::string& msg, int virtPos);

	static void replaceTabs(std::string& msg);

	static void printParsingError(string line, string errorMsg, int beg_line, int beg_col, int end_line, int end_col);
};



#endif /* ERRORREPORTING_H_ */
