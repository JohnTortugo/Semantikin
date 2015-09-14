#include "BasicBlock.h"

namespace IR {

	void BasicBlock::dumpToDot(stringstream& buffer) {
//		stringstream aux;
//
//		for (auto irLine : *this->_instructions) {
//			/* Do we have a label here?  */
//			if (irLine.first != nullptr)
//				aux << std::setfill(' ') << std::setw(15) << irLine.first->getName() << ": ";
//			else
//				aux << std::setfill(' ') << std::setw(17) << " ";
//
//			/* Is it just a label? */
//			if (irLine.second != nullptr) {
//				irLine.second->dump(aux);
//			}
//			else {
//				aux << "|";
//			}
//		}
//
//		// remove the last "\n", convert to html (dot) labels.
//		string str( Util::escapeStr(aux.str().substr(0, aux.str().size() - 1)) );
//		std::replace(str.begin(), str.end(), '\n', '|');
//		buffer << str;
	}

}
