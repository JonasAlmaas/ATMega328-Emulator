#pragma once

#include <cstdint>

#include "ATMega328Emulator/Types.h"
#include "ATMega328Emulator/Memory.h"

// Rd - Destination (and source) register in the Register File
// Rr - Source register in the Register File
// R - Result after instruction is executed

// Rd and Rr are registers in the range R0-R31
// Rdh and Rrh are registers in the range R16-R31(high half)
// Rdq and Rrq are registers in the range R16-R23 (one quarter of the register file)
// Rp is a register pair R25:R24, R27:R26 (X), R29:R28 (Y) or R31:R30 (Z)

// Rd - Destination (and source) register in the Register File
// Rr - Source register in the Register File
// R - Result after instruction is executed
// K - Constant data
// k - Constant address
// b - Bit in the Register File or I/O Register (3-bit)
// s - Bit in the Status Register (3-bit)

// A - I/O location address
// q - Displacement for direct addressing (6-bit)
// UU - Unsigned * Unsigned operands
// SS - Signed * Signed operands
// SU - Signed * Unsigned operands

namespace ATMega328Emulator {
	
	class CPU
	{
	public:
		void Reset(Memory& memory);

		// Fetches a Word from memory.
		// Takes 1 cycles.
		// Increments the program counter by 1.
		inline Word FetchWord(int& cycles, Memory& memory)
		{
			// 6502 is little endian
			Word address = PC * 2;

			Byte lo = memory[address];
			Byte hi = memory[address + 1];
		
			++PC;
			--cycles;
			
			#if PLATFORM_BIG_ENDIAN
				return hi | (lo << 8);
			#else
				return lo | (hi << 8);
			#endif
		}
		
		void Execute(int cycles, Memory& memory);

	public:
		// General purpose registers
		Byte R00, R01;
		Byte R02, R03;
		Byte R04, R05;
		Byte R06, R07;
		Byte R08, R09;
		Byte R10, R11;
		Byte R12, R13;
		Byte R14, R15;
		Byte R16, R17;
		Byte R18, R19;
		Byte R20, R21;
		Byte R22, R23;
		Byte R24, R25;
		
		union {
			struct { Byte R26, R27; };
			Word X; // Indirect Address Register
		};

		union {
			struct { Byte R28, R29; };
			Word Y; // Indirect Address Register
		};

		union {
			struct { Byte R30, R31; };
			Word Z; // Indirect Address Register
		};

		// Stack Pointer
		Word SPL;   // Stack Pointer to STACK
		Word SPH;   // Stack for return address and pushed registers

		// Program Counter
		Word PC;

		// I/O Registers
		Byte RAMPD; // Extended direct
		Byte RAMPX; // Extended X
		Byte RAMPY; // Extended Y
		Byte RAMPZ; // Extended Z
		Byte EIND; // Extended indirect
	
		struct StatusRegister {
			Byte I : 1;   // Global Interrupt Enable/Disable Flag
			Byte T : 1;   // Transfer bit used by BLD and BST instructions
			Byte H : 1;   // Half Carry Flag
			Byte S : 1;   // N ^ V, For signed tests
			Byte V : 1;   // Two's complement overflow indicator
			Byte N : 1;   // Negative Flag
			Byte Z : 1;   // Zero Flag
			Byte C : 1;   // Carry Flag
		} SREG;
		
		Byte* SRAM[2048]; // Internal SRAM (Should be at an offset of 0x0100)

	private:
		bool handleInstruction(Word instruction, int& cycles, Memory& memory);
		
	};
}
