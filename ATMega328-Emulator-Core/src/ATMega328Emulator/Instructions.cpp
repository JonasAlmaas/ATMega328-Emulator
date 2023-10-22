#include "ATMega328Emulator/Instructions.h"

namespace ATMega328Emulator {
	
	namespace Instruction {
	
		namespace StatusFlag {

			namespace H {
				
				// Set if there was a carry from bit 3; cleared otherwise.
				static inline void CarryBit3(CPU* cpu, Byte R, Byte Rd, Byte Rr)
				{
					cpu->IO.SREG.H = (
						((Rd & 0x8) & (Rr & 0x8)) |
						((Rr & 0x8) & (~R & 0x8)) |
						((~R & 0x8) & (Rd & 0x8))
					) >> 3;
				}
				
				// Set if there was a borrow from bit 3; cleared otherwise.
				static inline void BorrowBit3(CPU* cpu, Byte R, Byte Rd, Byte Rr)
				{
					cpu->IO.SREG.H = (
						((~Rd & 0x8) & (Rr & 0x8)) |
						((Rr & 0x8) & (R & 0x8)) |
						((R & 0x8) & (~Rd & 0x8))
					) >> 3;
				}
			}

			namespace S {
				
				// N XOR V, for signed tests.
				static inline void SignedTest(CPU* cpu)
				{
					cpu->IO.SREG.S = cpu->IO.SREG.N ^ cpu->IO.SREG.V;
				}
				
			}

			namespace V {

				static inline void Set(CPU* cpu, Byte value)
				{
					cpu->IO.SREG.V = value;
				}
				
				// Set if two's complement overflow resulted from the operation; cleared otherwise.
				static inline void ByteAddTwosComplementOverflow(CPU* cpu, Byte R, Byte Rd, Byte Rr)
				{
					cpu->IO.SREG.V = (
						((Rd & 0x80) & (Rr & 0x80) & (~R & 0x80)) |
						((~Rd & 0x80) & (~Rr & 0x80) & (R & 0x80))
					) >> 7;
				}

				static inline void WordAddTwosComplementOverflow(CPU* cpu, Word R, Word Rdh)
				{
					cpu->IO.SREG.V = ((~Rdh & 0x80) >> 7) & ((R & 0x8000) >> 15);
				}

				// Set if two’s complement overflow resulted from the operation; cleared otherwise.
				static inline void ByteSubTwosComplementOverflow(CPU* cpu, Byte R, Byte Rd, Byte Rr)
				{
					cpu->IO.SREG.V = (
						((Rd & 0x80) & (~Rr & 0x80) & (~R & 0x80)) |
						((~Rd & 0x80) & (Rr & 0x80) & (R & 0x80))
					) >> 7;
				}

				static inline void NCXOR(CPU* cpu)
				{
					cpu->IO.SREG.V = cpu->IO.SREG.N ^ cpu->IO.SREG.C;
				}
				
			}

			namespace N {

				static inline void Set(CPU* cpu, Byte value)
				{
					cpu->IO.SREG.N = value;
				}
				
				// Set if Most Significant Bit(MSB) of the result is set; cleared otherwise.
				static inline void MSBSet(CPU* cpu, Byte R)
				{
					cpu->IO.SREG.N = (R & 0x80) >> 7;
				}
				
			}

			namespace Z {
				
				// Set if Zero; cleared otherwise.
				static inline void ByteZeroRes(CPU* cpu, Byte R)
				{
					cpu->IO.SREG.Z = R == 0;
				}

				// Set if Zero; cleared otherwise.
				static inline void WordZeroRes(CPU* cpu, Word R)
				{
					cpu->IO.SREG.Z = R == 0;
				}

				// Previous value remains unchanged when the result is zero; cleared otherwise.
				static inline void ByteZeroResCarry(CPU* cpu, Byte R)
				{
					cpu->IO.SREG.Z = (R == 0) & cpu->IO.SREG.Z;
				}
				
			}

			namespace C {

				static inline void Set(CPU* cpu, Byte value)
				{
					cpu->IO.SREG.C = value;
				}

				static inline void NotNull(CPU* cpu, Byte R)
				{
					cpu->IO.SREG.C = R != 0x0;
				}

				// Set if there was a carry from the Most Significant Bit(MSB) of the result; cleared otherwise.
				static inline void ByteCarryMSB(CPU* cpu, Byte R, Byte Rd, Byte Rr)
				{
					cpu->IO.SREG.C = (
						((Rd & 0x80) & (Rr & 0x80)) |
						((Rr & 0x80) & (~R & 0x80)) |
						((~R & 0x80) & (Rd & 0x80))
					) >> 7;
				}

				// Set if there was a carry from the Most Significant Bit(MSB) of the result; cleared otherwise.
				static inline void WordCarryMSB(CPU* cpu, Word R, Word Rd)
				{
					cpu->IO.SREG.C = ((~R & 0x8000) >> 15) & ((Rd & 0x80) >> 7);
				}

				// Set if the absolute value of the contents of Rr plus previous carry is larger than the absolute value of the Rd; cleared otherwise.
				static inline void ByteGreater(CPU* cpu, Byte R, Byte Rd, Byte Rr)
				{
					cpu->IO.SREG.C = (
						((~Rd & 0x80) & (Rr & 0x80)) |
						((Rr & 0x80) & (R & 0x80)) |
						((R & 0x80) & (~Rd & 0x80))
					) >> 7;
				}

				static inline void MSBSet(CPU* cpu, Byte R)
				{
					cpu->IO.SREG.C = (R & 0x80) >> 7;
				}

				static inline void LSBSet(CPU* cpu, Byte R)
				{
					cpu->IO.SREG.C = R & 0x01;
				}
			}
		}
		
		void Handle_ADC(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);
			
			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;
		
			Byte R = *Rd + *Rr + cpu->IO.SREG.C;

			StatusFlag::H::CarryBit3(cpu, R, *Rd, *Rr);
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::ByteAddTwosComplementOverflow(cpu, R, *Rd, *Rr);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);
			StatusFlag::C::ByteCarryMSB(cpu, R, *Rd, *Rr);
		
			*Rd = R;
		}

		void Handle_ADD(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);
			
			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;
		
			Byte R = *Rd + *Rr;
		
			StatusFlag::H::CarryBit3(cpu, R, *Rd, *Rr);
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::ByteAddTwosComplementOverflow(cpu, R, *Rd, *Rr);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);
			StatusFlag::C::ByteCarryMSB(cpu, R, *Rd, *Rr);

			*Rd = R;
		}

		void Handle_ADIW(Word instruction, int& cycles, CPU* cpu)
		{
			Byte d = (instruction & 0b11'0000) >> 4; // 2 bits describing the offset from R24 in steps of 2.
			Byte K = instruction & 0b1111 | ((instruction & 0b1100'0000) >> 2); // Value in the range of [0-63]

			Word* Rdh = ((Word*)&cpu->R24) + d;
			Word R = *Rdh + K;

			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::WordAddTwosComplementOverflow(cpu, R, *Rdh);
			StatusFlag::N::MSBSet(cpu, (R & 0xFF00) >> 8);
			StatusFlag::Z::WordZeroRes(cpu, R);
			StatusFlag::C::WordCarryMSB(cpu, R, *Rdh);

			*Rdh = R;
			--cycles;
		}

		void Handle_AND(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);

			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;

			Byte R = *Rd & *Rr;

			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::Set(cpu, 0);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);

			*Rd = R;
		}

		void Handle_ANDI(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1111'0000) >> 4;
			Byte K = (instruction & 0xF) | ((instruction & 0xF00) >> 4);

			Byte* Rd = &cpu->R16 + d;
			Byte R = *Rd & K;

			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::Set(cpu, 0);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);

			*Rd = R;
		}

		void Handle_ASR(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte R = (*Rd >> 1) | (*Rd & 0x80);

			StatusFlag::S::SignedTest(cpu);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);
			StatusFlag::C::LSBSet(cpu, *Rd);
			StatusFlag::V::NCXOR(cpu); // Has to be done at the end

			*Rd = R;
		}

		void Handle_BCLR(Word instruction, CPU* cpu)
		{
			Byte s = (instruction & 0b111'0000) >> 4;
			*(Byte*)&cpu->IO.SREG &= ~(1 << s);
		}

		void Handle_BLD(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte b = instruction & 0b111;

			Byte* Rd = &cpu->R00 + d;
			Byte R = *Rd | cpu->IO.SREG.T << b;

			*Rd = R;
		}

		void Handle_BRBC(Word instruction, int& cycles, CPU* cpu)
		{
			char k = ((instruction & 0b1'1111'1000) >> 3) | ((instruction & 0b10'0000'0000) >> 2);
			Byte s = instruction & 0b111;
			
			if (((*(Byte*)&cpu->IO.SREG) & (1 << s)) == 0) { // Bit in register is cleared
				cpu->PC += k;
				--cycles;
			}
		}

		void Handle_BRBS(Word instruction, int& cycles, CPU* cpu)
		{
			char k = ((instruction & 0b1'1111'1000) >> 3) | ((instruction & 0b10'0000'0000) >> 2);
			Byte s = instruction & 0b111;

			if ((*(Byte*)&cpu->IO.SREG) & (1 << s)) { // Bit in register is set
				cpu->PC += k;
				--cycles;
			}
		}

		void Handle_BSET(Word instruction, CPU* cpu)
		{
			Byte s = (instruction & 0b111'0000) >> 4;
			*(Byte*)&cpu->IO.SREG |= 1 << s;
		}

		void Handle_BST(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte b = instruction & 0b111;

			Byte* Rd = &cpu->R00 + d;
			cpu->IO.SREG.T = (*Rd >> b) & 0b1;
		}

		void Handle_CBI(Word instruction, CPU* cpu)
		{
			Byte A = (instruction & 0b1111'1000) >> 3;
			Byte b = instruction & 0b111;

			*((Byte*)&cpu->IO + A) &= ~(1 << b);
		}
		
		void Handle_COM(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte R = ~*Rd;
			
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::Set(cpu, 0);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);
			StatusFlag::C::Set(cpu, 1);
			
			*Rd = R;
		}

		void Handle_CP(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);

			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;

			Byte R = *Rd - *Rr;

			StatusFlag::H::BorrowBit3(cpu, R, *Rd, *Rr);
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::ByteSubTwosComplementOverflow(cpu, R, *Rd, *Rr);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);
			StatusFlag::C::ByteGreater(cpu, R, *Rd, *Rr);
		}

		void Handle_CPC(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);

			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;

			Byte R = *Rd - *Rr - cpu->IO.SREG.C;

			StatusFlag::H::BorrowBit3(cpu, R, *Rd, *Rr);
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::ByteSubTwosComplementOverflow(cpu, R, *Rd, *Rr);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);
			StatusFlag::C::ByteGreater(cpu, R, *Rd, *Rr);
		}

		void Handle_CPI(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1111'0000) >> 4;
			Byte K = (instruction & 0b1111) | ((instruction & 0b1111'0000'0000) >> 4);
			
			Byte* Rd = &cpu->R00 + d;

			Byte R = *Rd - K;

			StatusFlag::H::BorrowBit3(cpu, R, *Rd, K);
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::ByteSubTwosComplementOverflow(cpu, R, *Rd, K);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);
			StatusFlag::C::ByteGreater(cpu, R, *Rd, K);
		}

		void Handle_CPSE(Word instruction, int& cycles, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);

			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;

			if (*Rd == *Rr) {
				++cpu->PC;
				--cycles;
			}
		}

		void Handle_DEC(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte R = *Rd - 1;

			StatusFlag::S::SignedTest(cpu);
			cpu->IO.SREG.V = R == 0x7F;
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);

			*Rd = R;
		}

		void Handle_EOR(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);

			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;

			Byte R = *Rd ^ *Rr;

			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::Set(cpu, 0);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);

			*Rd = R;
		}

		void Handle_FMUL(Word instruction, int& cycles, CPU* cpu)
		{
			Byte d = (instruction & 0b111'0000) >> 4;
			Byte r = instruction & 0b111;

			Byte* Rd = &cpu->R16 + d;
			Byte* Rr = &cpu->R16 + r;

			Word R = *Rd * *Rr;

			StatusFlag::C::MSBSet(cpu, *(Byte*)&R); // Has to happen before the shift

			R = R << 1;
			
			StatusFlag::Z::WordZeroRes(cpu, R);

			*(Word*)&cpu->R00 = R;
			
			--cycles;
		}

		void Handle_FMULS(Word instruction, int& cycles, CPU* cpu)
		{
			Byte d = (instruction & 0b111'0000) >> 4;
			Byte r = instruction & 0b111;

			char* Rd = (char*)&cpu->R16 + d;
			char* Rr = (char*)&cpu->R16 + r;

			short R = *Rd * *Rr;

			StatusFlag::C::MSBSet(cpu, *(Byte*)&R); // Has to happen before the shift

			R = R << 1;

			StatusFlag::Z::WordZeroRes(cpu, R);

			*(short*)&cpu->R00 = R;

			--cycles;
		}

		void Handle_FMULSU(Word instruction, int& cycles, CPU* cpu)
		{
			Byte d = (instruction & 0b111'0000) >> 4;
			Byte r = instruction & 0b111;

			char* Rd = (char*)&cpu->R16 + d;
			Byte* Rr = &cpu->R16 + r;

			short R = *Rd * *Rr;

			StatusFlag::C::MSBSet(cpu, *(Byte*)&R); // Has to happen before the shift

			R = R << 1;

			StatusFlag::Z::WordZeroRes(cpu, R);

			*(short*)&cpu->R00 = R;

			--cycles;
		}

		void Handle_INC(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte R = *Rd + 1;

			StatusFlag::S::SignedTest(cpu);
			cpu->IO.SREG.V = R == 0x80;
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);

			*Rd = R;
		}

		void Handle_LAC(Word instruction, int& cycles, CPU* cpu)
		{
			Byte r = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rr = &cpu->R00 + r;
			Byte* DS = cpu->SRAM + cpu->Z;

			*DS = (~*Rr) & *DS;
			*Rr = *DS;

			--cycles;
		}

		void Handle_LAS(Word instruction, int& cycles, CPU* cpu)
		{
			Byte r = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rr = &cpu->R00 + r;
			Byte* DS = cpu->SRAM + cpu->Z;

			*DS = *Rr | *DS;
			*Rr = *DS;

			--cycles;
		}

		void Handle_LAT(Word instruction, int& cycles, CPU* cpu)
		{
			Byte r = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rr = &cpu->R00 + r;
			Byte* DS = cpu->SRAM + cpu->Z;

			*DS = *Rr ^ *DS;
			*Rr = *DS;

			--cycles;
		}

		void Handle_LDI(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1111'0000) >> 4;
			Byte K = (instruction & 0b1111) | ((instruction & 0b1111'0000'0000) >> 4);

			Byte* Rd = &cpu->R16 + d;

			*Rd = K;
		}

		void Handle_LDS(Word instruction, int& cycles, CPU* cpu, Memory& memory)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Word k = cpu->FetchWord(cycles, memory);

			Byte* Rd = &cpu->R00 + d;
			Byte* DS = cpu->SRAM + k;

			*Rd = *DS;
		}

		void Handle_LSR(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte R = *Rd >> 1;

			StatusFlag::S::SignedTest(cpu);
			StatusFlag::N::Set(cpu, 0);
			StatusFlag::Z::ByteZeroRes(cpu, R);
			StatusFlag::C::LSBSet(cpu, *Rd);
			StatusFlag::V::NCXOR(cpu); // Has to be done at the end

			*Rd = R;
		}

		void Handle_MOV(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);

			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;

			*Rd = *Rr;
		}

		void Handle_MOVW(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1111'0000) >> 4;
			Byte r = instruction & 0b1111;

			Word* Rd = (Word*)&cpu->R00 + d;
			Word* Rr = (Word*)&cpu->R00 + r;

			*Rd = *Rr;
		}

		void Handle_MUL(Word instruction, int& cycles, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);

			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;
			Word R = *Rd * *Rr;

			StatusFlag::C::MSBSet(cpu, R >> 8);
			StatusFlag::Z::WordZeroRes(cpu, R);

			*(Word*)&cpu->R00 = R;
			--cycles;
		}

		void Handle_MULS(Word instruction, int& cycles, CPU* cpu)
		{
			Byte d = (instruction & 0b1111'0000) >> 4;
			Byte r = instruction & 0b1111;
			
			char* Rd = (char*)&cpu->R00 + d;
			char* Rr = (char*)&cpu->R00 + r;
			
			short R = *Rd * *Rr;

			StatusFlag::C::MSBSet(cpu, R >> 8);
			StatusFlag::Z::WordZeroRes(cpu, R);

			*(short*)&cpu->R00 = R;
		}

		void Handle_MULSU(Word instruction, int& cycles, CPU* cpu)
		{
			Byte d = (instruction & 0b111'0000) >> 4;
			Byte r = instruction & 0b111;

			char* Rd = (char*)cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;

			short R = *Rd * *Rr;

			StatusFlag::C::MSBSet(cpu, R >> 8);
			StatusFlag::Z::WordZeroRes(cpu, R);

			*(short*)&cpu->R00 = R;
		}
		
		void Handle_NEG(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte R = (~*Rd) + 1;
			
			StatusFlag::H::BorrowBit3(cpu, R, *Rd, 0);
			StatusFlag::S::SignedTest(cpu);
			cpu->IO.SREG.V = R == 0x80;
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);
			StatusFlag::C::NotNull(cpu, R);

			*Rd = R;
		}

		void Handle_OR(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);
			
			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;
			
			Byte R = *Rd | *Rr;

			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::Set(cpu, 0);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);

			*Rd = R;
		}

		void Handle_ORI(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1111'0000) >> 4;
			Byte K = (instruction & 0b1111) | (instruction & 0b1111'0000'0000) >> 4;

			Byte* Rd = &cpu->R16 + d;
			Byte R = *Rd | K;

			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::Set(cpu, 0);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);
			
			*Rd = R;
		}

		void Handle_OUT(Word instruction, CPU* cpu)
		{
			Byte A = (instruction & 0b1111) | ((instruction & 0b110'0000'0000) >> 5);
			Byte r = (instruction & 0b1111'0000) >> 4;

			Byte* IO = (Byte*)&cpu->IO + A;
			Byte* Rr = &cpu->R00 + r;

			*IO = *Rr;
		}

		void Handle_SBC(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);

			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;

			Byte R = *Rd - *Rr - cpu->IO.SREG.C;

			StatusFlag::H::BorrowBit3(cpu, R, *Rd, *Rr);
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::ByteSubTwosComplementOverflow(cpu, R, *Rd, *Rr);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroResCarry(cpu, R);
			StatusFlag::C::ByteGreater(cpu, R, *Rd, *Rr);

			*Rd = R;
		}

		void Handle_SBCI(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1111'0000) >> 4;
			Byte K = (instruction & 0b1111) | (instruction & 0b1111'0000'0000) >> 4;

			Byte* Rd = &cpu->R16 + d;
			Byte R = *Rd - K - cpu->IO.SREG.C;

			StatusFlag::H::BorrowBit3(cpu, R, *Rd, K);
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::ByteSubTwosComplementOverflow(cpu, R, *Rd, K);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroResCarry(cpu, R);
			StatusFlag::C::ByteGreater(cpu, R, *Rd, K);

			*Rd = R;
		}

		void Handle_SBI(Word instruction, CPU* cpu)
		{
			Byte A = (instruction & 0b1111'1000) >> 3;
			Byte b = instruction & 0b111;

			Byte* reg = (Byte*)&cpu->IO + A;
			*reg |= (1 << b);
		}

		void Handle_SUBI(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1111'0000) >> 4;
			Byte K = (instruction & 0b1111) | (instruction & 0b1111'0000'0000) >> 4;

			Byte* Rd = &cpu->R16 + d;
			Byte R = *Rd - K;

			StatusFlag::H::BorrowBit3(cpu, R, *Rd, K);
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::ByteSubTwosComplementOverflow(cpu, R, *Rd, K);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);
			StatusFlag::C::ByteGreater(cpu, R, *Rd, K);

			*Rd = R;
		}

		void Handle_SUB(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);

			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;

			Byte R = *Rd - *Rr;

			StatusFlag::H::BorrowBit3(cpu, R, *Rd, *Rr);
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::ByteSubTwosComplementOverflow(cpu, R, *Rd, *Rr);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteZeroRes(cpu, R);
			StatusFlag::C::ByteGreater(cpu, R, *Rd, *Rr);

			*Rd = R;
		}
		
	}
}
