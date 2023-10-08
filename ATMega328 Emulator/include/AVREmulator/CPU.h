#pragma once

#include <cstdint>

#include "AVREmulator/Types.h"
#include "AVREmulator/Memory.h"

class CPU
{
public:
	void Reset(Memory& memory);

	// Fetches a Word from memory.
	// Takes 2 cycles.
	// Increments the program counter by 1.
	inline Word FetchWord(uint32_t& cycles, Memory& mem)
	{
		// 6502 is little endian
		Byte lo = ReadByte(cycles, PC * 2, mem);
		Byte hi = ReadByte(cycles, PC * 2 + 1, mem);
		
		++PC;

		#if PLATFORM_BIG_ENDIAN
			return hi | (lo << 8);
		#else
			return lo | (hi << 8);
		#endif
	}

	// Reads a byte from memory.
	// Takes 1 cycle.
	inline Byte ReadByte(uint32_t& cycles, Byte address, Memory& mem)
	{
		--cycles;
		return mem[address];
	}

	void Execute(uint32_t cycles, Memory& memory);

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
	Byte R26, R27;
		
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
	Word SPH;   // Stack for return address and pushed registers
	Word SPL;   // Stack Pointer to STACK

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

	// Rd - Destination (and source) register in the Register File
	// Rr - Source register in the Register File

	// Rd and Rr are registers in the range R0–R31
	// Rdh and Rrh are registers in the range R16–R31(high half)
	// Rdq and Rrq are registers in the range R16–R23 (one quarter of the register file)
	// Rp is a register pair R25:R24, R27:R26 (X), R29:R28 (Y) or R31:R30 (Z)

	// R - Result after instruction is executed
	
	//Byte Rd;    // Destination (and source) register in the Register File
	//Byte Rr;    // Source register in the Register File
	//Byte R;     // Result after instruction is executed
	//Byte K;     // Constant data
	//Byte k;     // Constant address
	//Byte b;     // Bit in the Register File or I/O Register (3-bit)
	//Byte s;     // Bit in the Status Register (3-bit)
	
	//Byte A;     // I/O location address
	//Byte q;     // Displacement for direct addressing (6-bit)
	//Byte UU;    // Unsigned x Unsigned operands
	//Byte SS;    // Signed x Signed operands
	//Byte SU;    // Signed x Unsigned operands
	
};
