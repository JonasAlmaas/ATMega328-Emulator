#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_EOR)
{
	CPU cpuCopy = cpu;

	cpu.R00 = 0xF1;
	cpu.R22 = 0x1F;

	constexpr Byte a = 0xF1 ^ 0x1F;
	constexpr Byte b = 0b1 ^ 0b10;

	// eor r0,r22 ; Bitwise exclusive or between r0 and r22
	constexpr Word instruction =
		Instruction::EOR
		| 0b0             // Rd = R0
		| 0b10'0000'0110; // Rr = R22

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // EOR takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R00, 0xEE);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);
	EXPECT_EQ(cpu.IO.SREG.H, cpuCopy.IO.SREG.H);

	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_TRUE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);
	
	EXPECT_EQ(cpu.IO.SREG.C, cpuCopy.IO.SREG.C);
}

TEST_F(ATMega328, Test_INS_EOR_CLR)
{
	CPU cpuCopy = cpu;

	cpu.R04 = 0xF1;
	
	// eor r4,r4 ; Clear r4. Same as "clr r4"
	constexpr Word instruction =
		Instruction::EOR
		| 0b100'0000 // Rd = R4
		| 0b100;     // Rr = R4

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // EOR takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R04, 0x0);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);
	EXPECT_EQ(cpu.IO.SREG.H, cpuCopy.IO.SREG.H);
	
	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_FALSE(cpu.IO.SREG.N);
	EXPECT_TRUE(cpu.IO.SREG.Z);

	EXPECT_EQ(cpu.IO.SREG.C, cpuCopy.IO.SREG.C);
}

