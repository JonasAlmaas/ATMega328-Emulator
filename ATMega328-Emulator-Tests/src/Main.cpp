#include "ATMega328Emulator/CPU.h"

using namespace ATMega328Emulator;

int main()
{
	Memory memory;
	CPU cpu;

	cpu.Reset(memory);
	
	// TODO: Inject program into memory

	cpu.Execute(0, memory);
	
	return 0;
}
