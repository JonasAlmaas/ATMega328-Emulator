#include <gtest/gtest.h>

#include "ATMega328Emulator/CPU.h"

using namespace ATMega328Emulator;

TEST(HardwareTests, CPUMemoryLayout)
{
	EXPECT_EQ(offsetof(CPU, CPU::R00), 0x0);
	EXPECT_EQ(offsetof(CPU, CPU::R31), 0x1F);

	EXPECT_EQ(offsetof(CPU, CPU::IO), 0x20);
	EXPECT_EQ(offsetof(CPU, CPU::EXIO), 0x65);

	//EXPECT_EQ(offsetof(CPU, CPU::SRAM), IDK);
	//EXPECT_EQ(offsetof(CPU, CPU::EEPROM), IDK);
}

TEST(HardwareTests, CPU_IORegisters)
{
	// https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf

	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_00), 0x0);     //         - 0x20
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_01), 0x01);    //         - 0x21
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_02), 0x02);    //         - 0x22
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PINB), 0x03);   // 14.4.4  - 0x23
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::DDRB), 0x04);   // 14.4.3  - 0x24
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PORTB), 0x05);  // 14.4.2  - 0x25
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PINC), 0x06);   // 14.4.7  - 0x26
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::DDRC), 0x07);   // 14.4.6  - 0x27
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PORTC), 0x08);  // 14.4.5  - 0x28
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PIND), 0x09);   // 14.4.10 - 0x29
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::DDRD), 0x0A);   // 14.4.9  - 0x2A
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PORTD), 0x0B);  // 14.4.8  - 0x2B
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_0C), 0x0C);    //         - 0x2C
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_0D), 0x0D);    //         - 0x2D
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_0E), 0x0E);    //         - 0x2E
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_0F), 0x0F);    //         - 0x2F
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_10), 0x10);    //         - 0x30
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_11), 0x11);    //         - 0x31
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_12), 0x12);    //         - 0x32
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_13), 0x13);    //         - 0x33
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_14), 0x14);    //         - 0x34
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::TIFR0), 0x15);  // 15.9.7  - 0x35
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_16), 0x16);    //         - 0x36
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_17), 0x17);    //         - 0x37
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_18), 0x18);    //         - 0x38
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_19), 0x19);    //         - 0x39
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_1A), 0x1A);    //         - 0x3A
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PCIFR), 0x1B);  // 13.2.5  - 0x3B
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::EIFR), 0x1C);   // 13.2.3  - 0x3C
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::SPL), 0x1D);    // 7.5.1   - 0x3D
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::EIMSK), 0x1D);  // 13.2.2  - 0x3D
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::SPH), 0x1E);    // 7.5.1   - 0x3E
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::GPIOR0), 0x1E); // 8.6.6   - 0x3E
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::EECR), 0x1F);   // 8.6.3   - 0x3F
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::EEDR), 0x20);   // 8.6.2   - 0x40
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::EEARL), 0x21);  // 8.6.1   - 0x41
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::EEARH), 0x22);  // 8.6.1   - 0x42
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::GTCCR), 0x23);  // 17.4.1  - 0x43
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::TCCR0A), 0x24); // 15.9.1  - 0x44
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::TCCR0B), 0x25); // 15.9.2  - 0x45
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::TCNT0), 0x26);  // 15.6.3  - 0x46
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::OCR0A), 0x27);  // 15.6.4  - 0x47
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::OCR0B), 0x28);  // 15.6.5  - 0x48
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_29), 0x29);    //         - 0x49
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::GPIOR1), 0x2A); // 8.6.5   - 0x4A
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::GPIOR2), 0x2B); // 8.6.4   - 0x4B
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_2C), 0x2C);    //         - 0x4C
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_2D), 0x2D);    //         - 0x4D
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_2E), 0x2E);    //         - 0x4E
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_2F), 0x2F);    //         - 0x4F
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_30), 0x30);    //         - 0x50
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_31), 0x31);    //         - 0x51
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_32), 0x32);    //         - 0x52
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::SMCR), 0x33);   // 10.11.1 - 0x53
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::MCUSR), 0x34);  // 11.9.1  - 0x54
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::MCUCR), 0x35);  // 10.11.2 - 0x55
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_36), 0x36);    //         - 0x56
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_37), 0x37);    //         - 0x57
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_38), 0x38);    //         - 0x58
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_39), 0x39);    //         - 0x59
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_3A), 0x3A);    //         - 0x5A
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_3B), 0x3B);    //         - 0x5B
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_3C), 0x3C);    //         - 0x5C
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_3D), 0x3D);    //         - 0x5D
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_3E), 0x3E);    //         - 0x5E
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::SREG), 0x3F);   // 7.3.1   - 0x5F
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::WDTCSR), 0x40); // 11.9.2  - 0x60
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::CLKPR), 0x41);  // 9.12.2  - 0x61
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_42), 0x42);    //         - 0x62
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_43), 0x43);    //         - 0x63
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PRR), 0x44);    // 10.11.3 - 0x64
}

TEST(HardwareTests, CPU_ExternalIORegisters)
{
	// https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf

	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_00), 0x0);     //         - 0x65
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::OSCCAL), 0x01); // 9.12.1  - 0x66
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_02), 0x02);    //         - 0x67
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::PCICR), 0x03);  // 13.2.4  - 0x68
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::EICRA), 0x04);  // 12.2.1  - 0x69
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_05), 0x05);    //         - 0x6A
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::PCMSK0), 0x06); // 13.2.8  - 0x6B
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::PCMSK1), 0x07); // 13.2.7  - 0x6C
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::PCMSK2), 0x08); // 13.2.6  - 0x6D
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TIMSK0), 0x09); // 15.9.6  - 0x6E

	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TIMSK2), 0x0B); // 18.11.6 - 0x70

	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TCCR2A), 0x4B); // 18.11.1 - 0xB0
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TCCR2B), 0x4C); // 18.11.2 - 0xB1
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TCNT2), 0x4D);  // 18.11.3 - 0xB2
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::OCR2A), 0x4E);  // 18.11.4 - 0xB3
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::OCR2B), 0x4F);  // 18.11.5 - 0xB4
}
