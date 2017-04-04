#pragma once
#include "Semantikin.h"
#include "IR.h"

#include <memory>
#include <sstream>

namespace Backend {
	class MachineInstruction : public std::enable_shared_from_this<MachineInstruction> {
        protected:
            Instruction_sptr _irInstr;

        public:
            virtual ~MachineInstruction();

            virtual void dump(std::stringstream& buf);
	};

    class X64Instruction : public MachineInstruction {
        public:
            enum X86InstructionCode {
                SCopy_M_R,

                IAdd_R_R,
                IAdd_R_M,
                IAdd_M_R
            };

            X64Instruction(X86InstructionCode code, Instruction_sptr irInstr) : _code(code) {
                this->_irInstr = irInstr;
            }

            ~X64Instruction();

            void dump(std::stringstream& buf);

        private:
            X86InstructionCode _code;
    };
}
