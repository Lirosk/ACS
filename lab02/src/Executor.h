
#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"
#include <map>

class Executor
{
public:
    void Execute(InstructionPtr& instr, Word ip)
    {
        /* YOUR CODE HERE */
        
        /*"вычисление Instruction->{_data, _addr, _nextIp}"*/

		fprintf(stderr, "\n\nip is %d\n", ip);

		//for IType::{J, Jr, Auipc}
		instr->_nextIp = ip;

		Word dataCopied = instr->_data;

        /*"блок АЛУ"*/
		if (instr->_aluFunc != AluFunc::None)
		if (instr->_src1)
		{
			if (instr->_imm)
			{
				AluFuncs[instr->_aluFunc](instr->_src1Val, *(instr->_imm), instr->_data);
				if (instr->_type == IType::Ld || instr->_type == IType::St)
				{
					instr->_addr = instr->_data;
					instr->_data = dataCopied;
				}
			}
			else if (instr->_src2)
			{
				AluFuncs[instr->_aluFunc](instr->_src1Val, instr->_src2Val, instr->_data);
				if (instr->_type == IType::Ld || instr->_type == IType::St)
				{
					instr->_addr = instr->_data;
					instr->_data = dataCopied;
				}
			}
		}

		/*"блок логики"*/
		if (instr->_type != IType::Unsupported && instr->_type != IType::Alu
		&&  instr->_type != IType::Ld          && instr->_type != IType::Br)
		{
			ITypes[instr->_type](instr);
		}	

        /*"блок ветвелния"*/
		if (instr->_src1 && instr->_src2)
		{
			if (BrFuncs[instr->_brFunc](instr->_src1Val, instr->_src2Val))
			{
				switch (instr->_type)
				{
				case IType::Br:
				case IType::J:
					instr->_nextIp = ip + *(instr->_imm);
					break;
				case IType::Jr:
					instr->_nextIp = instr->_src1Val + *(instr->_imm);
					break;
				default:
					break;
				}
			}   
			else
			{
				instr->_nextIp = ip + 4;
			}     
		}
		else
		{
			instr->_nextIp = ip + 4;
		}		
    }

private:
    /* YOUR CODE HERE */
    
    std::map<AluFunc, void(*)(Word, Word, Word&)> AluFuncs
    {
    	{
    		AluFunc::Add,
    		[](Word x1, Word x2, Word& data)
    		{
				data = x1 + x2;
			}
    	},
    	{
    		AluFunc::Sub,
    		[](Word x1, Word x2, Word& data)
    		{
				data = x1 - x2;
			}
    	},
    	{
    		AluFunc::And,
    		[](Word x1, Word x2, Word& data)
    		{
				data = x1 & x2;
			}
    	},
    	{
    		AluFunc::Or,
    		[](Word x1, Word x2, Word& data)
    		{
				data = x1 | x2;
			}
    	},
    	{
    		AluFunc::Xor,
    		[](Word x1, Word x2, Word& data)
    		{
				data = x1 ^ x2;
			}
    	},
    	{
    		AluFunc::Slt,
    		[](Word x1, Word x2, Word& data)
    		{
				data = (Word)(((SignedWord)x1) < ((SignedWord)x2));
			}
    	},
    	{
    		AluFunc::Sltu,
    		[](Word x1, Word x2, Word& data)
    		{
				data = (Word)(x1 < x2);
			}
    	},
		{
    		AluFunc::Sll,
    		[](Word x1, Word x2, Word& data)
    		{
				data = x1 << (x2 % 32);
			}
    	},
		{
    		AluFunc::Srl,
    		[](Word x1, Word x2, Word& data)
    		{
				data = x1 >> (x2 % 32);
			}
    	},
		{
    		AluFunc::Sra,
    		[](Word x1, Word x2, Word& data)
    		{
				data = (((SignedWord)x1) >> (x2 % 32));
			}
    	}	
    };

	std::map<IType, void(*)(InstructionPtr&)> ITypes
	{
		{
			IType::Csrr,
			[](InstructionPtr& instr)
			{
				instr->_data = instr->_csrVal;
				//instr->_addr = instr->_csrVal;
			}
		},
		{
			IType::Csrw,
			[](InstructionPtr& instr)
			{
				instr->_data = instr->_src1Val;
				//instr->_addr = instr->_src1Val;
			}
		},
		{
			IType::St,
			[](InstructionPtr& instr)
			{
				instr->_data = instr->_src2Val;
				//instr->_addr = instr->_src2Val;
			}
		},
		{
			IType::J,
			[](InstructionPtr& instr)
			{
				instr->_data = instr->_nextIp + 4;
				//instr->_addr = instr->_nextIp + 4;
			}
		},
		{
			IType::Jr,
			[](InstructionPtr& instr)
			{
				instr->_data = instr->_nextIp + 4;
				//instr->_addr = instr->_nextIp + 4;
			}
		},
		{
			IType::Auipc,
			[](InstructionPtr& instr)
			{
				instr->_data = instr->_nextIp + *(instr->_imm);
				//instr->_addr = instr->_nextIp + *(instr->_imm);
			}
		},

	};

	std::map<BrFunc, bool(*)(Word, Word)> BrFuncs
	{
		{
			BrFunc::Eq,
			[](Word x1, Word x2)
			{
				return x1 == x2;
			}
		},
		{
			BrFunc::Neq,
			[](Word x1, Word x2)
			{
				return x1 != x2;
			}
		},
		{
			BrFunc::Lt,
			[](Word x1, Word x2)
			{
				return ((SignedWord)x1) < ((SignedWord)x2);
			}
		},
		{
			BrFunc::Ltu,
			[](Word x1, Word x2)
			{
				return x1 < x2;
			}
		},
		{
			BrFunc::Ge,
			[](Word x1, Word x2)
			{
				return ((SignedWord)x1) >= ((SignedWord)x2);
			}
		},
		{
			BrFunc::Geu,
			[](Word x1, Word x2)
			{
				return x1 >= x2;
			}
		},
		{
			BrFunc::AT,
			[](Word x1, Word x2)
			{
				return true;
			}
		},
		{
			BrFunc::NT,
			[](Word x1, Word x2)
			{
				return false;
			}
		}
	};
};

#endif // RISCV_SIM_EXECUTOR_H
