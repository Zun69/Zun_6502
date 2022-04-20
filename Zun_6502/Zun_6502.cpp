// Zun_6502.cpp : This file contains the 'main' function. Program execution begins and ends there. https://web.archive.org/web/20210803072351/http://www.obelisk.me.uk/6502/registers.html
//

#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char; //16 bits
using Word = unsigned short; //8 bits

struct Memory
{
    static constexpr unsigned int MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialise() //intiialise all memory to 0
    {
        for (unsigned int i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0; 
        }
    }

    Byte operator[](unsigned int Address) const //read 1 byte
    {
        return Data[Address]; //return byte
    }

    Byte& operator[](unsigned int Address)  //write 1 byte
    {
        return Data[Address]; //return byte
    }
};

struct CPU
{
    Word PC; //program counter, 16 bit register, points at next instruction to be executed
    Byte SP; //stack pointer, 8 bit register

    Byte A, X, Y; //registers

    Byte C : 1; //status flag, carry
    Byte Z : 1; //status flag, zero
    Byte I : 1; //status flag, interrupt disable
    Byte D : 1; //status flag, decimal mode
    Byte B : 1; //status flag, break command
    Byte O : 1; //status flag, overflow
    Byte N : 1; //status flag, negative

    void Reset(Memory& memory) //bootup sequence for 6502 cpu in the Commodore 64
    {
        PC = 0xFFFC;
        SP = 0x0100; //first stack access happens at address 0100
        C = Z = I = D = B = O = N = 0; //clear decimal flag, and all flags
        memory.Initialise();
    }

    //opcodes
    static constexpr Byte
        INS_LDA_IM = 0xA9; //load accumulator, immediate addressing mode instruction

    Byte FetchByte(unsigned int& cycles, Memory& memory)
    {
        Byte Data = memory[PC]; //fetch instruction from memory address that program counter is pointing to
        PC++; //increment program counter
        cycles--;
        return Data; //return fetched instruction, and decrement cpu cycle
    }

    void Execute(unsigned int cycles, Memory& memory) //excute cpu instruction
    {
        while (cycles > 0)
        {
            Byte instruction = FetchByte(cycles, memory); //fetch instruction
            
            switch (instruction)
            {
            case INS_LDA_IM:
                Byte value = FetchByte(cycles, memory);
                A = value;
                Z = (A == 0); //set Z flag if A register = 0
                N = (A & 0b10000000) > 0; //set N flag if bit 7 of A is set
                break;
            }
            
                
        }
    }
};

int main()
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_LDA_IM;
    memory[0xFFFD] = 0x42;

    cpu.Execute(2, memory);
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
