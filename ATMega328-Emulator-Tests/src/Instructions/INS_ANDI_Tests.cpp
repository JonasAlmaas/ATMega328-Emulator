#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_ANDI)
{
	CPU cpuCopy = cpu;

	cpu.R17 = 0b1010'1010;

	//andi r17,0x0F ; Clear upper nibble of r17
	constexpr Word instruction =
		Instructions::ANDI
		| 0b1'0000 // Rd = R17
		| 0b1111;  // K = 0x0F

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(2, memory); // ADIW takes 2 cycles

	// Assert
	EXPECT_EQ(cpu.R17, 0b0000'1010);
	
	EXPECT_EQ(cpu.SREG.I, cpuCopy.SREG.I);
	EXPECT_EQ(cpu.SREG.T, cpuCopy.SREG.T);
	EXPECT_EQ(cpu.SREG.H, cpuCopy.SREG.H);

	EXPECT_FALSE(cpu.SREG.S);
	EXPECT_FALSE(cpu.SREG.V);
	EXPECT_FALSE(cpu.SREG.N);
	EXPECT_FALSE(cpu.SREG.Z);
	
	EXPECT_EQ(cpu.SREG.C, cpu.SREG.C);
}

TEST_F(ATMega328, Test_INS_ANDI_NullResult)
{
	CPU cpuCopy = cpu;

	cpu.R17 = 0b1010'1010;

	//andi r17,0x0F ; Clear upper nibble of r17
	constexpr Word instruction =
		Instructions::ANDI
		| 0b1'0000 // Rd = R17
		| 0b0;     // K = 0x0

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(2, memory); // ADIW takes 2 cycles

	// Assert
	EXPECT_EQ(cpu.R17, 0);

	EXPECT_EQ(cpu.SREG.I, cpuCopy.SREG.I);
	EXPECT_EQ(cpu.SREG.T, cpuCopy.SREG.T);
	EXPECT_EQ(cpu.SREG.H, cpuCopy.SREG.H);

	EXPECT_FALSE(cpu.SREG.S);
	EXPECT_FALSE(cpu.SREG.V);
	EXPECT_FALSE(cpu.SREG.N);
	EXPECT_TRUE(cpu.SREG.Z);

	EXPECT_EQ(cpu.SREG.C, cpu.SREG.C);
}

TEST_F(ATMega328, Test_INS_ANDI_MSBSet)
{
	CPU cpuCopy = cpu;

	cpu.R17 = 0b1010'1010;

	//andi r17,0x0F ; Clear upper nibble of r17
	constexpr Word instruction =
		Instructions::ANDI
		| 0b1'0000          // Rd = R17
		| 0b1111'0000'1111; // K = 0xFF

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(2, memory); // ADIW takes 2 cycles

	// Assert
	EXPECT_EQ(cpu.R17, 0b1010'1010);

	EXPECT_EQ(cpu.SREG.I, cpuCopy.SREG.I);
	EXPECT_EQ(cpu.SREG.T, cpuCopy.SREG.T);
	EXPECT_EQ(cpu.SREG.H, cpuCopy.SREG.H);

	EXPECT_FALSE(cpu.SREG.S);
	EXPECT_FALSE(cpu.SREG.V);
	EXPECT_TRUE(cpu.SREG.N);
	EXPECT_FALSE(cpu.SREG.Z);

	EXPECT_EQ(cpu.SREG.C, cpu.SREG.C);
}
