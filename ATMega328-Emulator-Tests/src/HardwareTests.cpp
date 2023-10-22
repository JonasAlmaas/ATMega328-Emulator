#include <gtest/gtest.h>

#include "ATMega328Emulator/CPU.h"

using namespace ATMega328Emulator;

TEST(HardwareTests, CPUMemoryLayout)
{
	// https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf

	// 8.3
	EXPECT_EQ(offsetof(CPU, CPU::R00), 0x0);
	EXPECT_EQ(offsetof(CPU, CPU::R31), 0x1F);

	EXPECT_EQ(offsetof(CPU, CPU::IO), 0x20);
	EXPECT_EQ(sizeof CPU::IO, 0x40);
	
	EXPECT_EQ(offsetof(CPU, CPU::EXIO), 0x60);
	EXPECT_EQ(sizeof CPU::EXIO, 0xA0);

	EXPECT_EQ(offsetof(CPU, CPU::SRAM), 0x100);
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
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::TIFR1), 0x16);  // 16.11.9 - 0x36
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::TIFR2), 0x17);  // 18.11.7 - 0x37
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_18), 0x18);    //         - 0x38
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_19), 0x19);    //         - 0x39
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_1A), 0x1A);    //         - 0x3A
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PCIFR), 0x1B);  // 13.2.5  - 0x3B
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::EIFR), 0x1C);   // 13.2.3  - 0x3C
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::EIMSK), 0x1D);  // 13.2.2  - 0x3D
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::GPIOR0), 0x1E); // 8.6.6   - 0x3E
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::EECR), 0x1F);   // 8.6.3   - 0x3F
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::EEDR), 0x20);   // 8.6.2   - 0x40
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::EEARL), 0x21);  // 8.6.1   - 0x41
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::EEARH), 0x22);  // 8.6.1   - 0x42
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::GTCCR), 0x23);  // 17.4.1  - 0x43
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::TCCR0A), 0x24); // 15.9.1  - 0x44
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::TCCR0B), 0x25); // 15.9.2  - 0x45
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::TCNT0), 0x26);  // 15.9.3  - 0x46
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::OCR0A), 0x27);  // 15.9.4  - 0x47
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::OCR0B), 0x28);  // 15.9.5  - 0x48
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_29), 0x29);    //         - 0x49
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::GPIOR1), 0x2A); // 8.6.5   - 0x4A
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::GPIOR2), 0x2B); // 8.6.4   - 0x4B
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::SPCR), 0x2C);   // 19.5.1  - 0x4C
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::SPSR), 0x2D);   // 19.5.2  - 0x4D
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::SPDR), 0x2E);   // 19.5.3  - 0x4E
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_2F), 0x2F);    //         - 0x4F
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::ACSR), 0x30);   // 23.3.2  - 0x50
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_31), 0x31);    //         - 0x51
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_32), 0x32);    //         - 0x52
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::SMCR), 0x33);   // 10.11.1 - 0x53
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::MCUSR), 0x34);  // 11.9.1  - 0x54
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::MCUCR), 0x35);  // 10.11.2 - 0x55
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_36), 0x36);    //         - 0x56
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::SPMCSR), 0x37); // 26.3.1  - 0x57
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_38), 0x38);    //         - 0x58
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_39), 0x39);    //         - 0x59
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_3A), 0x3A);    //         - 0x5A
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_3B), 0x3B);    //         - 0x5B
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::_3C), 0x3C);    //         - 0x5C
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::SP), 0x3D);     // 7.5.1   - 0x5D
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::SPL), 0x3D);    // 7.5.1   - 0x5D
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::SPH), 0x3E);    // 7.5.1   - 0x5E
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::SREG), 0x3F);   // 7.3.1   - 0x5F
}

TEST(HardwareTests, CPU_ExternalIORegisters)
{
	// https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf

	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::WDTCSR), 0x0);  // 11.9.2  - 0x60
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::CLKPR), 0x01);  // 9.12.2  - 0x61
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_02), 0x02);    //         - 0x62
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_03), 0x03);    //         - 0x63
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::PRR), 0x04);    // 10.11.3 - 0x64
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_05), 0x05);    //         - 0x65
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::OSCCAL), 0x06); // 9.12.1  - 0x66
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_07), 0x07);    //         - 0x67
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::PCICR), 0x08);  // 13.2.4  - 0x68
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::EICRA), 0x09);  // 12.2.1  - 0x69
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_0A), 0x0A);    //         - 0x6A
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::PCMSK0), 0x0B); // 13.2.8  - 0x6B
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::PCMSK1), 0x0C); // 13.2.7  - 0x6C
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::PCMSK2), 0x0D); // 13.2.6  - 0x6D
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TIMSK0), 0x0E); // 15.9.6  - 0x6E
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TIMSK1), 0x0F); // 16.11.8 - 0x6F
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TIMSK2), 0x10); // 18.11.6 - 0x70
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_11), 0x11);    //         - 0x71
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_12), 0x12);    //         - 0x72
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_13), 0x13);    //         - 0x73
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_14), 0x14);    //         - 0x74
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_15), 0x15);    //         - 0x75
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_16), 0x16);    //         - 0x76
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_17), 0x17);    //         - 0x77
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::ADC), 0x18);    // 24.9.3  - 0x78
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::ADCL), 0x18);   // 24.9.3  - 0x78
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::ADCH), 0x19);   // 24.9.3  - 0x79
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::ADCSRA), 0x1A); // 24.9.2  - 0x7A
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::ADCSRB), 0x1B); // 23.3.1  - 0x7B
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::ADMUX), 0x1C);  // 24.9.1  - 0x7C
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_1D), 0x1D);    //         - 0x7D
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::DIDR0), 0x1E);  // 24.9.5  - 0x7E
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::DIDR1), 0x1F);  // 23.3.3  - 0x7F
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TCCR1A), 0x20); // 16.11.1 - 0x80
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TCCR1B), 0x21); // 16.11.2 - 0x81
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TCCR1C), 0x22); // 16.11.3 - 0x82
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_23), 0x23);    //         - 0x83
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TCNT1), 0x24);  // 16.11.5 - 0x84
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TCNT1L), 0x24); // 16.11.5 - 0x84
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TCNT1H), 0x25); // 16.11.5 - 0x85
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::ICR1), 0x26);   // 16.11.7 - 0x86
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::ICR1L), 0x26);  // 16.11.7 - 0x86
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::ICR1H), 0x27);  // 16.11.7 - 0x87
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::OCR1A), 0x28);  // 16.11.5 - 0x88
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::OCR1AL), 0x28); // 16.11.5 - 0x88
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::OCR1AH), 0x29); // 16.11.5 - 0x89
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::OCR1B), 0x2A);  // 16.11.6 - 0x8A
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::OCR1BL), 0x2A); // 16.11.6 - 0x8A
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::OCR1BH), 0x2B); // 16.11.6 - 0x8B

	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TCCR2A), 0x50); // 18.11.1 - 0xB0
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TCCR2B), 0x51); // 18.11.2 - 0xB1
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TCNT2), 0x52);  // 18.11.3 - 0xB2
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::OCR2A), 0x53);  // 18.11.4 - 0xB3
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::OCR2B), 0x54);  // 18.11.5 - 0xB4
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_55), 0x55);    //         - 0xB5
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::ASSR), 0x56);   // 18.11.8 - 0xB6
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_57), 0x57);    //         - 0xB7
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TWBR), 0x58);   // 22.9.1  - 0xB8
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TWSR), 0x59);   // 22.9.3  - 0xB9
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TWAR), 0x5A);   // 22.9.5  - 0xBA
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TWDR), 0x5B);   // 22.9.4  - 0xBB
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TWCR), 0x5C);   // 22.9.2  - 0xBC
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::TWAMR), 0x5D);  // 22.9.6  - 0xBD
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_5E), 0x5E);    //         - 0xBE
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_5F), 0x5F);    //         - 0xBF
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::UCSR0A), 0x60); // 22.11.2 - 0xC0
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::UCSR0B), 0x61); // 22.11.3 - 0xC1
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::UCSR0C), 0x62); // 22.11.4 - 0xC2
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::_63), 0x63);    //         - 0xC3
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::UBRR0), 0x64);  // 20.11.5 - 0xC4
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::UBRR0L), 0x64); // 20.11.5 - 0xC4
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::UBRR0H), 0x65); // 20.11.5 - 0xC5
	EXPECT_EQ(offsetof(CPU::ExternalIORegisters, CPU::ExternalIORegisters::UDR0), 0x66);   // 20.11.1 - 0xC6
}
