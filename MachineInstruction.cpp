#include "MachineInstruction.h"

namespace Backend {
    MachineInstruction::~MachineInstruction() { 

    }

    void MachineInstruction::dump(std::stringstream& buf) {
    }

    X64Instruction::~X64Instruction() {
    }

    void X64Instruction::dump(std::stringstream& buf) {
        switch (this->_code) {
            case SCopy_M_R:
                buf << "move " << std::endl;
                break;

            case IAdd_R_R:
                buf << "add " << this->_irInstr->tgtDataName() << "," << std::endl;
                break;

            case IAdd_M_R:
                buf << "add " << std::endl;
                break;

            case IAdd_R_M:
                buf << "add " << std::endl;
                break;
        }
    }
}
