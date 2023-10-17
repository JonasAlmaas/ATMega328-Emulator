#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_SBCI)
{
	CPU cpuCopy = cpu;

	cpu.R22 = 0xF1;

	//sbci r22,0x0F ; Subtract 0x0F from r22
	constexpr Word instruction =
		Instruction::SBCI
		| 0b110'0000 // Rd = R22
		| 0b1111;    // K = 0x0F

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // SBCI takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R22, 0xF1 - 0x0F);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);

	EXPECT_TRUE(cpu.IO.SREG.H);
	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_TRUE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);
	EXPECT_FALSE(cpu.IO.SREG.C);
}

TEST_F(ATMega328, Test_INS_SBCI_Carry)
{
	CPU cpuCopy = cpu;

	cpu.R18 = 0x9;
	cpu.IO.SREG.C = 1;

	// sbci r18,0x2 ; Subtract 0x2 from r18 with carry high byte
	constexpr Word instruction =
		Instruction::SBCI
		| 0b10'0000 // Rd = R18
		| 0b10; // K = 0x2 (2)

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // SBCI takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R18, 0x6);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);

	EXPECT_FALSE(cpu.IO.SREG.I);
	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_FALSE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);
	EXPECT_FALSE(cpu.IO.SREG.C);
}
