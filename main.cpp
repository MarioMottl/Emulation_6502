#include "m6502.hpp"  

void Memory::Initialise()
{
    for (u32 i = 0; i <= MAX_MEM; i++)
    {
        Data[i] = 0;
    }
}

Byte Memory::operator[](u32 Address) const
{
    return Data[Address];
}

Byte& Memory::operator[](u32 Address)
{
    return Data[Address];
}

void Memory::WriteWord(u32 &Cycles, Word Value, u32 Address)
{
    Data[Address] = Value & 0xFF;
    Data[Address + 1] = (Value >> 8);
    Cycles -=2;
}

void CPU::Reset(Memory &mem)
{
    PC = 0xFFFC;
    SP = 0x0100;
    C = Z = I = D= B = V = N = 0;
    A = X = Y = 0;
    mem.Initialise();
}

Byte CPU::FetchByte(u32 &Cycles, Memory &mem)
{
    Byte Data = mem[PC];
    PC ++;
    Cycles --;
    return Data;
}

Byte CPU::ReadByte(u32 &Cycles, Byte Address, Memory &mem)
{
    Byte Data = mem[Address];
    PC ++;
    Cycles --;
    return Data;    
}

void CPU::Execute(u32 Cycles, Memory &mem)
{
    while(Cycles > 0)
    {
        Byte Instruction = FetchByte(Cycles,mem);
        switch (Instruction)
        {
            case INS_LDA_IM:
            {
                Byte Value = FetchByte(Cycles,mem);
                A = Value;
                Z = (A == 0);
                N = (A & 0b10000000) > 0;
            }
            break;
            default:
            {
                printf("No Instruction found");
            }
        }
    }
}

int main()
{
    Memory mem;
    CPU cpu;
    cpu.Reset(mem);

    //Test Memory
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x42;

    cpu.Execute(2, mem);

    return 0;
}