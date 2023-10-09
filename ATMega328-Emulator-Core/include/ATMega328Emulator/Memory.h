#pragma once

#include <cstdint>

#include "ATMega328Emulator/Types.h"

namespace ATMega328Emulator {
	
	class Memory
	{
	public:
		static constexpr uint32_t MAX_MEM = 2048;
	
	public:
		Memory();
		~Memory();

		void Initialize();
	public:
		inline Byte operator[](uint32_t address) const
		{
			// TODO: Assert if address is greater than MAX_MEM
			return Data[address];
		}

		inline Byte& operator[](uint32_t address)
		{
			// TODO: Assert if address is greater than MAX_MEM
			return Data[address];
		}

		// Write a Byte to memory
		// Takes 1 cycle
		inline void Write(Byte value, Word address, int& cycles)
		{
			Data[address] = value;
			cycles--;
		}

		// Write a Word to memory.
		// Takes 2 cycles.
		inline void WriteWord(Word value, Word address, int& cycles)
		{
			Write(value & 0xFF, address, cycles);
			Write((value >> 8) & 0xFF, address + 1, cycles);
		}

	public:
		Byte* Data = nullptr;
	
	};

}
