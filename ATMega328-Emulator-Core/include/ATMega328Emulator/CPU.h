#pragma once

#include <cstdint>

#include "ATMega328Emulator/Types.h"
#include "ATMega328Emulator/Memory.h"

// Rd - Destination (and source) register in the Register File
// Rr - Source register in the Register File
// R - Result after instruction is executed

// Rd and Rr are registers in the range R0-R31
// Rdh and Rrh are registers in the range R16-R31(high half)
// Rdq and Rrq are registers in the range R16-R23 (one quarter of the register file)
// Rp is a register pair R25:R24, R27:R26 (X), R29:R28 (Y) or R31:R30 (Z)

// Rd - Destination (and source) register in the Register File
// Rr - Source register in the Register File
// R - Result after instruction is executed
// K - Constant data
// k - Constant address
// b - Bit in the Register File or I/O Register (3-bit)
// s - Bit in the Status Register (3-bit)

// A - I/O location address
// q - Displacement for direct addressing (6-bit)
// UU - Unsigned * Unsigned operands
// SS - Signed * Signed operands
// SU - Signed * Unsigned operands

namespace ATMega328Emulator {
	
	class CPU
	{
	public:
		static constexpr uint32_t FREQUENCY = 20'000'000; // 20MHz
		static constexpr uint16_t PROGRAM_MEMORY_SIZE = 32 * 1024; // 32KB
		static constexpr uint16_t FLASH_SIZE = 32 * 1024; // 32KB
		static constexpr uint16_t SRAM_SIZE = 2 * 1024; // 2KB
		static constexpr uint16_t EEPROM_SIZE = 1024; // 1KB
		
	public:
		void Reset(Memory& memory);

		// Fetches a Word from memory.
		// Takes 1 cycles.
		// Increments the program counter by 1.
		inline Word FetchWord(int& cycles, Memory& memory)
		{
			// 6502 is little endian
			Word address = PC * 2;

			Byte lo = memory[address];
			Byte hi = memory[address + 1];
		
			++PC;
			--cycles;
			
			return lo | (hi << 8);
		}
		
		void Execute(int cycles, Memory& memory);

	public:
		// General purpose registers
		Byte R00, R01;
		Byte R02, R03;
		Byte R04, R05;
		Byte R06, R07;
		Byte R08, R09;
		Byte R10, R11;
		Byte R12, R13;
		Byte R14, R15;
		Byte R16, R17;
		Byte R18, R19;
		Byte R20, R21;
		Byte R22, R23;
		Byte R24, R25;

		#pragma pack(1)
		union {
			struct { Byte R26, R27; };
			Word X; // Indirect Address Register
		};
		#pragma pack()

		#pragma pack(1)
		union {
			struct { Byte R28, R29; };
			Word Y; // Indirect Address Register
		};
		#pragma pack()
		
		#pragma pack(1)
		union {
			struct { Byte R30, R31; };
			Word Z; // Indirect Address Register
		};
		#pragma pack()

		// General Purpose I/O Registers
		// https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf
		struct IORegisters {
			Byte _00;
			Byte _01;
			Byte _02;

			// Ports as General Digital I/O
			Byte PINB;   // 14.4.4  - 0x23 - Port B Input Pins Address (Read Only)
			Byte DDRB;   // 14.4.3  - 0x24 - Port B Data Direction Register (Read Write)
			Byte PORTB;  // 14.4.2  - 0x25 - Port B Data Register (Read Write)
			Byte PINC;   // 14.4.7  - 0x26 - Port C Input Pins Address (Read Only)
			Byte DDRC;   // 14.4.6  - 0x27 - Port C Data Direction Register (Read Write)
			Byte PORTC;  // 14.4.5  - 0x28 - Port C Data Register (Read Write)
			Byte PIND;   // 14.4.10 - 0x29 - Port D Input Pins Address (Read Only)
			Byte DDRD;   // 14.4.9  - 0x2A - Port D Data Direction Register (Read Write)
			Byte PORTD;  // 14.4.8  - 0x2B - Port D Data Register (Read Write)
			
			Byte _0C;
			Byte _0D;
			Byte _0E;
			Byte _0F;
			Byte _10;
			Byte _11;
			Byte _12;
			Byte _13;
			Byte _14;

			Byte TIFR0;  // 15.9.7  - 0x35 - Timer/Counter 0 Interrupt Flag Register
			Byte TIFR1;  // 16.11.9 - 0x36 - Timer/Counter 1 Interrupt Flag Register
			Byte TIFR2;  // 18.11.7 - 0x37 - Timer/Counter 2 Interrupt Flag Register

			Byte _18;
			Byte _19;
			Byte _1A;

			Byte PCIFR;  // 13.2.5  - 0x3B - Pin Change Interrupt Flag Register
			Byte EIFR;   // 13.2.3  - 0x3C - External Interrupt Flag Register
			Byte EIMSK;  // 13.2.2  - 0x3D - External Interrupt Mask Register

			Byte GPIOR0; // 8.6.6   - 0x3E - General Purpose I/O Register 0

			Byte EECR;   // 8.6.3   - 0x3F - EEPROM Control Register
			Byte EEDR;   // 8.6.2   - 0x40 - EEPROM Data Register
			#pragma pack(1)
			union {      // 8.6.1   - 0x41 - EEPROM Address Register
				struct {
					Byte EEARL; // 8.6.1 - 0x41 - EEPROM Address Register Low
					Byte EEARH; // 8.6.1 - 0x42 - EEPROM Address Register High
				};
				Word EEAR;
			};
			#pragma pack()

			Byte GTCCR;  // 17.4.1  - 0x43 - General Timer/Counter Control Register
			Byte TCCR0A; // 15.9.1  - 0x44 - Timer/Counter Control Register A
			Byte TCCR0B; // 15.9.2  - 0x45 - Timer/Counter Control Register B
			Byte TCNT0;  // 15.9.3  - 0x46 - Timer/Counter Register
			Byte OCR0A;  // 15.9.4  - 0x47 - Output Compare Register A
			Byte OCR0B;  // 15.9.5  - 0x48 - Output Compare Register B

			Byte _29;
			
			Byte GPIOR1; // 8.6.5   - 0x4A - General Purpose I/O Register 1
			Byte GPIOR2; // 8.6.4   - 0x4B - General Purpose I/O Register 2
			
			Byte SPCR;   // 19.5.1  - 0x4C - SPI Control Register
			Byte SPSR;   // 19.5.2  - 0x4D - SPI Status Register
			Byte SPDR;   // 19.5.3  - 0x4E - SPI Data Register
			
			Byte _2F;

			Byte ACSR;    // 23.3.2 - 0x50 - Analog Comparator Control and Status Register

			Byte _31;
			Byte _32;

			Byte SMCR;   // 10.11.1 - 0x53 - Sleep Mode Control Register
			
			Byte MCUSR;  // 11.9.1  - 0x54 - MCU Status Register
			Byte MCUCR;  // 10.11.2 - 0x55 - MCU Control Register

			Byte _36;

			Byte SPMCSR; // 26.3.1  - 0x57 - Store Program Memory Control and Status Register

			Byte _38;
			Byte _39;
			Byte _3A;
			Byte _3B;
			Byte _3C;

			#pragma pack(1)
			union {       // 7.5.1  - 0x5D - Stack Pointer
				struct {
					Byte SPL; //      0x5D - Stack Pointer Low
					Byte SPH; //      0x5E - Stack Pointer High
				};
				Word SP;
			};
			#pragma pack()

			struct StatusRegister {
				Byte C : 1; // Carry Flag
				Byte Z : 1; // Zero Flag
				Byte N : 1; // Negative Flag
				Byte V : 1; // Two's complement overflow indicator
				Byte S : 1; // N ^ V, For signed tests
				Byte H : 1; // Half Carry Flag
				Byte T : 1; // Transfer bit used by BLD and BST instructions
				Byte I : 1; // Global Interrupt Enable/Disable Flag
			} SREG;      // 7.3.1   - 0x5F - AVR Status Register
		} IO;

		struct ExternalIORegisters {
			struct WatchdogTimerControlRegister {
				Byte WDP0 : 1; // Watchdog Timer Prescaler Bit 0
				Byte WDP1 : 1; // Watchdog Timer Prescaler Bit 1
				Byte WDP2 : 1; // Watchdog Timer Prescaler Bit 2
				Byte WDE : 1; // Watchdog System Reser Enable
				Byte WDCE : 1; // Watchdog Change Enable
				Byte WDP3 : 1; // Watchdog Timer Prescaler Bit 3
				Byte WDIE : 1; // Watchdog Interrupt Enable
				Byte WDIF : 1; // Watchdog Interrupt Flag
			} WDTCSR;    // 11.9.2  - 0x60 - Watchdog Timer Control Register

			Byte CLKPR;  // 9.12.2  - 0x61 - Clock Prescale Register

			Byte _02;
			Byte _03;

			Byte PRR;    // 10.11.3 - 0x64 - Power Reduction Register

			Byte _05;

			Byte OSCCAL; // 9.12.1  - 0x66 - Oscillator Calibration Register

			Byte _07;

			Byte PCICR;  // 13.2.4  - 0x68 - Pin Change Interrupt Control Register
			Byte EICRA;  // 12.2.1  - 0x69 - External Interrupt Control Register A

			Byte _0A;

			Byte PCMSK0; // 13.2.8  - 0x6B - Pin Change Mask Register 0
			Byte PCMSK1; // 13.2.7  - 0x6C - Pin Change Mask Register 1
			Byte PCMSK2; // 13.2.6  - 0x6D - Pin Change Mask Register 2

			Byte TIMSK0; // 15.9.6  - 0x6E - Timer/Counter 0 Interrupt Mask Register
			Byte TIMSK1; // 16.11.8 - 0x6F - Timer/Counter 1 Interrupt Mask Register
			Byte TIMSK2; // 18.11.6 - 0x70 - Timer/Counter 2 Interrupt Mask Register

			Byte _11;
			Byte _12;
			Byte _13;
			Byte _14;
			Byte _15;
			Byte _16;
			Byte _17;

			#pragma pack(1)
			union {      // 24.9.3  - 0x78 - ADC Data Register
				struct {
					Byte ADCL; //     0x79 - ADC Data Register Low
					Byte ADCH; //     0x79 - ADC Data Register High
				};
				Word ADC;
			};
			#pragma pack()

			Byte ADCSRA; // 24.9.2  - 0x7A - ADC Control and Status Register A
			Byte ADCSRB; // 23.3.1  - 0x7B - ADC Control and Status Register B

			Byte ADMUX;  // 24.9.1  - 0x7C - ADC Multiplexer Selection Register

			Byte _1D;

			Byte DIDR0;  // 24.9.5  - 0x7E - Digital Input Disable Register 0
			Byte DIDR1;  // 23.3.3  - 0x7F - Digital Input Disable Register 1

			Byte TCCR1A; // 16.11.1 - 0x80 - Timer/Counter 1 Control Register A
			Byte TCCR1B; // 16.11.2 - 0x81 - Timer/Counter 1 Control Register B
			Byte TCCR1C; // 16.11.3 - 0x82 - Timer/Counter 1 Control Register C

			Byte _23;
			
			#pragma pack(1)
			union {      // 16.11.4 - 0x84 - Timer/Counter 1 Register
				struct {
					Byte TCNT1L; // 0x84 - Timer/Counter 1 Register Low
					Byte TCNT1H; // 0x85 - Timer/Counter 1 Register High
				};
				Word TCNT1;
			};
			#pragma pack()

			#pragma pack(1)
			union {      // 16.11.7 - 0x86 - Input Capture Register 1
				struct {
					Byte ICR1L; // 0x86 - Input Capture Register 1 Low
					Byte ICR1H; // 0x87 - Input Capture Register 1 High
				};
				Word ICR1;
			};
			#pragma pack()

			#pragma pack(1)
			union {      // 16.11.5 - 0x88 - Output Compare Register A
				struct {
					Byte OCR1AL; // 0x88 - Output Compare Register A Low
					Byte OCR1AH; // 0x89 - Output Compare Register A High
				};
				Word OCR1A;
			};
			#pragma pack()

			#pragma pack(1)
			union {      // 16.11.6 - 0x8A - Output Compare Register B
				struct {
					Byte OCR1BL; // 0x88 - Output Compare Register B Low
					Byte OCR1BH; // 0x89 - Output Compare Register B High
				};
				Word OCR1B;
			};
			#pragma pack()
			
			Byte _reserved_1[36];

			Byte TCCR2A; // 18.11.1 - 0xB0 - Timer/Counter Control Register A
			Byte TCCR2B; // 18.11.2 - 0xB1 - Timer/Counter Control Register B
			Byte TCNT2;  // 18.11.3 - 0xB2 - Timer/Counter Register
			
			Byte OCR2A;  // 18.11.4 - 0xB3 - Output Compare Register A
			Byte OCR2B;  // 18.11.5 - 0xB4 - Output Compare Register B

			Byte _55;

			Byte ASSR;   // 18.11.8 - 0xB6 - Asynchronous Status Register

			Byte _57;

			Byte TWBR;   // 22.9.1  - 0xB8 - TWI Bit Rate Register
			Byte TWSR;   // 22.9.3  - 0xB9 - TWI Status Register
			Byte TWAR;   // 22.9.5  - 0xBA - TWI (Slave) Address Register
			Byte TWDR;   // 22.9.4  - 0xBB - TWI Data Register
			Byte TWCR;   // 22.9.2  - 0xBC - TWI Control Register
			Byte TWAMR;  // 22.9.6  - 0xBD - TWI (Slave) Address Mask Register

			Byte _5E;
			Byte _5F;

			Byte UCSR0A; // 20.11.2 - 0xC0 - USART Control and Status Register n A
			Byte UCSR0B; // 20.11.3 - 0xC1 - USART Control and Status Register n B
			Byte UCSR0C; // 20.11.4 - 0xC2 - USART Control and Status Register n C

			Byte _63;

			#pragma pack(1)
			union {      // 20.11.5 - 0xC4 - USART Baud Rate Registers
				struct {
					Byte UBRR0L; // 0xC4 - USART Baud Rate Registers Low
					Byte UBRR0H; // 0xC5 - USART Baud Rate Registers High
				};
				Word UBRR0;
			};
			#pragma pack()

			Byte UDR0;   // 20.11.1 - 0xC6 - USART I/O Data Register n

			Byte _reserved_2[57];
		} EXIO;
		
		Byte SRAM[SRAM_SIZE]; // Internal SRAM (Should be at an offset of 0x100)
		
		Byte EEPROM[EEPROM_SIZE]; // Should be elsewhere?

		// Program Counter (Should probably not be here)
		union {
			struct { Byte PCL, PCH; };
			Word PC;
		};

	private:
		bool handleInstruction(Word instruction, int& cycles, Memory& memory);
		
	};
	
}
