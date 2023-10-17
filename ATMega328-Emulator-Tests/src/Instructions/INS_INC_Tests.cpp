#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_INC)
{
	CPU cpuCopy = cpu;

	cpu.R04 = 0xF1;

	// inc r4 ; Increment r4
	constexpr Word instruction =
		Instruction::INC
		| 0b100'0000; // Rd = R4

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // INC takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R04, 0xF2);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);
	EXPECT_EQ(cpu.IO.SREG.H, cpuCopy.IO.SREG.H);

	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_TRUE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);

	EXPECT_EQ(cpu.IO.SREG.C, cpuCopy.IO.SREG.C);
}

TEST_F(ATMega328, Test_INS_INC_TwosComplementOverflow)
{
	CPU cpuCopy = cpu;

	cpu.R04 = 0X7F;

	// inc r4 ; Increment r4
	constexpr Word instruction =
		Instruction::INC
		| 0b100'0000; // Rd = R4

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // INC takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R04, 0x80);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);
	EXPECT_EQ(cpu.IO.SREG.H, cpuCopy.IO.SREG.H);

	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_TRUE(cpu.IO.SREG.V);
	EXPECT_TRUE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);

	EXPECT_EQ(cpu.IO.SREG.C, cpuCopy.IO.SREG.C);
}
