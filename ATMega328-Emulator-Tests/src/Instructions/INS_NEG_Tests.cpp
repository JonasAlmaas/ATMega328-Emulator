#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_NEG)
{
	CPU cpuCopy = cpu;

	cpu.R11 = 0xF0;

	// neg r11 ; Take two’s complement of r11
	constexpr Word instruction =
		Instruction::NEG
		| 0b1011'0000; // Rd = R11

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // NEG takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R11, 0x10);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);
	
	EXPECT_FALSE(cpu.IO.SREG.H);
	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_FALSE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);
	EXPECT_TRUE(cpu.IO.SREG.C);
}
