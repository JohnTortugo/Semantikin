#include "ErrorReporting.h"
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;

int ParsingError::findPos(const std::string& msg, int virtPos) {
	int actPos = 0;

	for (int i=0; i<virtPos; i++) {
		if (msg[i] == '\t') actPos += 4;
		else actPos++;
	}

	return actPos;
}

void ParsingError::replaceTabs(std::string& msg) {
	for (int pos; (pos = msg.find("\t")) != std::string::npos; )
		msg.replace(pos, 1, "    ");
}

void ParsingError::printParsingError(string line, string errorMsg, int beg_line, int beg_col, int end_line, int end_col) {
	int beg = findPos(line, beg_col) - 1;
	int end = findPos(line, end_col);
	int dif = end - beg;
	replaceTabs(line);

	std::cerr << beg_line << ":" << beg_col << "-" << end_col << ": " << errorMsg << "." << endl;
	std::cerr << line << endl;
	std::cerr << std::setw(beg) << std::setfill('.') << "" << std::setw(dif) << std::setfill('^') << "" << endl;
}
