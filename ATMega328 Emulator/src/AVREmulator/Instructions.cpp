#include "AVREmulator/Instructions.h"

namespace Instructions {
	
	namespace StatusFlag {
	
		// Set if there was a carry from bit 3; cleared otherwise.
		static inline void CarryBit3(CPU* cpu, Byte R, Byte Rd, Byte Rr)
		{
			cpu->SREG.H = (
				((Rd & 0x8) & (Rr & 0x8)) |
				((Rr & 0x8) & (~R & 0x8)) |
				((~R & 0x8) & (Rd & 0x8))
			) >> 3;
		}
			
		// N XOR V, for signed tests.
		static inline void NVSignedTest(CPU* cpu)
		{
			cpu->SREG.S = cpu->SREG.N ^ cpu->SREG.V;
		}
		
		// Set if two's complement overflow resulted from the operation; cleared otherwise.
		static inline void ByteTwosComplementOverflow(CPU* cpu, Byte R, Byte Rd, Byte Rr)
		{
			cpu->SREG.V =(
				((Rd & 0x80) & (Rr & 0x80) & (~R & 0x80)) |
				((~Rd & 0x80) & (~Rr & 0x80) & (R & 0x80))
			) >> 7;
		}

		static inline void WordTwosComplementOverflow(CPU* cpu, Word R, Word Rdh)
		{
			cpu->SREG.V = ((~Rdh & 0x80) >> 7) & ((R & 0x8000) >> 15);
		}
		
		// Set if Most Significant Bit(MSB) of the result is set; cleared otherwise.
		static inline void MSBSet(CPU* cpu, Byte result)
		{
			cpu->SREG.N = (result & 0x80) >> 7;
		}

		// Set if Zero; cleared otherwise.
		static inline void ByteNullRes(CPU* cpu, Byte result)
		{
			cpu->SREG.Z = result == 0;
		}

		// Set if Zero; cleared otherwise.
		static inline void WordNullRes(CPU* cpu, Word result)
		{
			cpu->SREG.Z = result == 0;
		}

		// Set if there was a carry from the Most Significant Bit(MSB) of the result; cleared otherwise.
		static inline void ByteCarryMSB(CPU* cpu, Byte R, Byte Rd, Byte Rr)
		{
			cpu->SREG.C = (
				((Rd & 0x80) & (Rr & 0x80)) |
				((Rr & 0x80) & (~R & 0x80)) |
				((~R & 0x80) & (Rd & 0x80))
			) >> 7;
		}

		// Set if there was a carry from the Most Significant Bit(MSB) of the result; cleared otherwise.
		static inline void WordCarryMSB(CPU* cpu, Word R, Word Rdh)
		{
			cpu->SREG.C = ((~R & 0x8000) >> 15) & ((Rdh & 0x80) >> 7);
		}
		
	}
	
	void Handle_ADC(Word instruction, Memory& memory, CPU* cpu)
	{
		Byte destOffset = ((instruction & 0b1111'0000) >> 4) | ((instruction & 0b1'0000'0000) >> 8);
		Byte srcOffset = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 9);

		Byte* Rd = &cpu->R00 + destOffset;
		Byte* Rr = &cpu->R00 + srcOffset;
		
		Byte R = *Rd + *Rr + cpu->SREG.C;

		StatusFlag::CarryBit3(cpu, R, *Rd, *Rr);
		StatusFlag::NVSignedTest(cpu);
		StatusFlag::ByteTwosComplementOverflow(cpu, R, *Rd, *Rr);
		StatusFlag::MSBSet(cpu, R);
		StatusFlag::ByteNullRes(cpu, R);
		StatusFlag::ByteCarryMSB(cpu, R, *Rd, *Rr);
		
		*Rd = R;
	}

	void Handle_ADD(Word instruction, Memory& memory, CPU* cpu)
	{
		Byte destOffset = ((instruction & 0b1111'0000) >> 4) | ((instruction & 0b1'0000'0000) >> 8);
		Byte srcOffset = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 9);
		
		Byte* Rd = &cpu->R00 + destOffset;
		Byte* Rr = &cpu->R00 + srcOffset;
		
		Byte R = *Rd + *Rr;
		
		StatusFlag::CarryBit3(cpu, R, *Rd, *Rr);
		StatusFlag::NVSignedTest(cpu);
		StatusFlag::ByteTwosComplementOverflow(cpu, R, *Rd, *Rr);
		StatusFlag::MSBSet(cpu, R);
		StatusFlag::ByteNullRes(cpu, R);
		StatusFlag::ByteCarryMSB(cpu, R, *Rd, *Rr);
		
		*Rd = R;
	}

	void Handle_ADIW(Word instruction, Memory& memory, CPU* cpu)
	{
		Byte offset = ((instruction & 0b11'0000) >> 4); // 2 bits describing the offset from R24.
		Byte immediate = instruction & 0b1111 | ((instruction & 0b1100'0000) >> 2); // Value in the range of [0-63]

		Word* Rdh = (Word*)&cpu->R24 + offset * 2;
		Word R = *Rdh + immediate;

		StatusFlag::NVSignedTest(cpu);
		StatusFlag::WordTwosComplementOverflow(cpu, R, *Rdh);
		StatusFlag::MSBSet(cpu, (R & 0xFF00) >> 8);
		StatusFlag::WordNullRes(cpu, R);
		StatusFlag::WordCarryMSB(cpu, R, *Rdh);
		
		*Rdh = R;

		// Might have to do something like this
		/*
		Byte* target = &cpu->R24 + dd * 2;
		*target = immediate & 0xFF;
		*(target - 1) = (immediate & 0xFF00) >> 8;
		*/
	}
	
	void Handle_AND(Word instruction, Memory& memory, CPU* cpu)
	{
		Byte destOffset = ((instruction & 0b1111'0000) >> 4) | ((instruction & 0b1'0000'0000) >> 8);
		Byte srcOffset = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 9);

		Byte* Rd = &cpu->R00 + destOffset;
		Byte* Rr = &cpu->R00 + srcOffset;
		
		Byte R = *Rd & *Rr;
	
		StatusFlag::NVSignedTest(cpu);
		cpu->SREG.V = 0;
		StatusFlag::MSBSet(cpu, R);
		StatusFlag::ByteNullRes(cpu, R);
	
		*Rd = R;
	}
	
	void Handle_ANDI(Word instruction, Memory& memory, CPU* cpu)
	{
		Byte destOffset = (instruction & 0b1111'0000) >> 4;
		Byte immediate = (instruction & 0xF) | ((instruction & 0xF00) >> 4);
		
		Byte* Rd = &cpu->R16 + destOffset;
		Byte R = *Rd & immediate;
		
		StatusFlag::NVSignedTest(cpu);
		cpu->SREG.V = 0;
		StatusFlag::MSBSet(cpu, R);
		StatusFlag::ByteNullRes(cpu, R);
		
		*Rd = R;
	}
	
	/*
	void Handle_CBR(uint32_t& cycles, Memory& memory, CPU* cpu)
	{
		cpu->R = cpu->Rd & ~cpu->K;

		StatusFlag::NVSignedTest(cpu);
		cpu->SREG.V = 0;
		StatusFlag::IsResByteMSBSet(cpu);
		StatusFlag::IsResByteNull(cpu);
	
		cpu->Rd = cpu->R;
	}
	*/

	/*
	void Handle_CLR(uint32_t& cycles, Memory& memory, CPU* cpu)
	{
		cpu->R = cpu->Rd ^ cpu->Rd;

		cpu->SREG.S = 0;
		cpu->SREG.V = 0;
		cpu->SREG.N = 0;
		cpu->SREG.Z = 1;

		cpu->Rd = cpu->R;
	}
	*/

	/*
	void Handle_COM(uint32_t& cycles, Memory& memory, CPU* cpu)
	{
		cpu->R = ~cpu->Rd;
	
		StatusFlag::NVSignedTest(cpu);
		cpu->SREG.V = 0;
		StatusFlag::IsResByteMSBSet(cpu);
		StatusFlag::IsResByteNull(cpu);
		cpu->SREG.C = 1;
	
		cpu->Rd = cpu->R;
	}
	*/
}
