#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_CLR)
{
	CPU cpuCopy = cpu;

	cpu.R18 = 0xF1;
	
	//clr r18 ; Clear r18
	constexpr Word instruction =
		Instructions::CLR
		| 0b1'0010; // Rd = R18

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // ADD takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R18, 0x0);

	EXPECT_EQ(cpu.SREG.I, cpuCopy.SREG.I);
	EXPECT_EQ(cpu.SREG.T, cpuCopy.SREG.T);
	EXPECT_EQ(cpu.SREG.H, cpuCopy.SREG.H);
	
	EXPECT_FALSE(cpu.SREG.S);
	EXPECT_FALSE(cpu.SREG.V);
	EXPECT_FALSE(cpu.SREG.N);
	EXPECT_TRUE(cpu.SREG.Z);

	EXPECT_EQ(cpu.SREG.C, cpuCopy.SREG.C);
}
