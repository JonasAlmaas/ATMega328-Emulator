//https://en.wikipedia.org/wiki/ATmega328
//https://ww1.microchip.com/downloads/en/DeviceDoc/AVR-Instruction-Set-Manual-DS40002198A.pdf
//https://ww1.microchip.com/downloads/en/devicedoc/atmel-0856-avr-instruction-set-manual.pdf
//https://sourceware.org/binutils/docs/as/AVR-Opcodes.html
//https://en.wikipedia.org/wiki/Atmel_AVR_instruction_set

#include "AVREmulator/CPU.h"

int main()
{
	Memory memory;
	CPU cpu;

	cpu.Reset(memory);
	
	// TODO: Inject program into memory

	cpu.Execute(0, memory);
	
	return 0;
}