#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_COM)
{
	CPU cpuCopy = cpu;

	cpu.R04 = 0xF1;
	
	// com r4 ; Take one’s complement of r4
	constexpr Word instruction =
		Instructions::COM
		| 0b100'0000; // Rd = R4

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // COM takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R04, 0x0E);

	EXPECT_EQ(cpu.SREG.I, cpuCopy.SREG.I);
	EXPECT_EQ(cpu.SREG.T, cpuCopy.SREG.T);
	EXPECT_EQ(cpu.SREG.H, cpuCopy.SREG.H);

	EXPECT_FALSE(cpu.SREG.S);
	EXPECT_FALSE(cpu.SREG.V);
	EXPECT_FALSE(cpu.SREG.N);
	EXPECT_FALSE(cpu.SREG.Z);

	EXPECT_TRUE(cpu.SREG.C);
}
