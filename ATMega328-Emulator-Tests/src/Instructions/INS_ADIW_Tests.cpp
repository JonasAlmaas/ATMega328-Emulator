#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_ADIW)
{
	CPU cpuCopy = cpu;

	cpu.R24 = 0x12;
	cpu.R25 = 0x34;
	
	// adiw r25:24, 1; Add 6 to r25:r24
	constexpr Word instruction =
		Instruction::ADIW
		| 0x0  // Rd = R24
		| 0x6; // K = 1

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(2, memory); // ADIW takes 2 cycles

	// Assert
	EXPECT_EQ(*(Word*)&cpu.R24, 0x3418);

	EXPECT_EQ(cpu.SREG.I, cpuCopy.SREG.I);
	EXPECT_EQ(cpu.SREG.T, cpuCopy.SREG.T);
	EXPECT_EQ(cpu.SREG.H, cpuCopy.SREG.H);
	
	EXPECT_FALSE(cpu.SREG.S);
	EXPECT_FALSE(cpu.SREG.V);
	EXPECT_FALSE(cpu.SREG.N);
	EXPECT_FALSE(cpu.SREG.Z);
	EXPECT_FALSE(cpu.SREG.C);
}

TEST_F(ATMega328, Test_INS_ADIW_ZPointer)
{
	CPU cpuCopy = cpu;

	cpu.R30 = 0x12;
	cpu.R31 = 0x34;
	
	//constexpr Word a = (0x12 | (0x34 << 8)) + 63;
	//constexpr Word b = 0x3451;

	// adiw ZH:ZL, 63; Add 63 to the Z-pointer(r31:r30)
	constexpr Word instruction =
		Instruction::ADIW
		| 0b11'0000    // Rd = ZH:ZL
		| 0b1100'1111; // K = 63

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(2, memory); // ADIW takes 2 cycles

	// Assert
	EXPECT_EQ(*(Word*)&cpu.R30, 0x3451);

	EXPECT_EQ(cpu.SREG.I, cpuCopy.SREG.I);
	EXPECT_EQ(cpu.SREG.T, cpuCopy.SREG.T);
	EXPECT_EQ(cpu.SREG.H, cpuCopy.SREG.H);

	EXPECT_FALSE(cpu.SREG.S);
	EXPECT_FALSE(cpu.SREG.V);
	EXPECT_FALSE(cpu.SREG.N);
	EXPECT_FALSE(cpu.SREG.Z);
	EXPECT_FALSE(cpu.SREG.C);
}
