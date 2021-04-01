
#ifndef RISCV_SIM_CPU_H
#define RISCV_SIM_CPU_H

#include "Memory.h"
#include "Decoder.h"
#include "RegisterFile.h"
#include "CsrFile.h"
#include "Executor.h"
#include <optional>

class Cpu
{
public:
    Cpu(Memory& mem)
        : _mem(mem)
    {
	
    }

    void ProcessInstruction()
    {	
    	/* YOUR CODE HERE */
    
    	
    	/*"получение слова инструкции типа Word
    	   из модуля памяти Memory
    	   по указателю инструкции _ip"*/
    	Word data = _mem.Request(_ip);    	
    	
    	/*"декодирование инструкции
    	   в структуру типа Instruction"*/
    	InstructionPtr instr = _decoder.Decode(data);  
    	
    	/*"чтение требуемых инструкцией регистров
    	   из регистровых файлов RegisterFile и CsrFile"*/
    	_csrf.Read(instr);
    	_rf.Read(instr);
    	  	
    	/*"исполнение инструкции в модуле Executor"*/
    	_exe.Execute(instr, _ip);
    	
    	/*"обращение в память"*/
    	_mem.Request(instr);
    	
    	/*"запись результата в регистровые файлы
    	   RegisterFile и CsrFile"*/
    	_csrf.Write(instr);
    	_rf.Write(instr);
		_csrf.InstructionExecuted();
    	
    	/*"обновление регистров в CsrFile"*/
    	/*"вычисление нового _ip"*/
		Reset(instr->_nextIp);
    }

    void Reset(Word ip)
    {
        _csrf.Reset();
        _ip = ip;
    }

    std::optional<CpuToHostData> GetMessage()
    {
        return _csrf.GetMessage();
    }

private:
    Reg32 _ip;
    Decoder _decoder;
    RegisterFile _rf;
    CsrFile _csrf;
    Executor _exe;
    Memory& _mem;
};


#endif //RISCV_SIM_CPU_H
