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

void CPU::WriteByte(u32 &Cycles, Word Value, u32 Address,Memory &mem)
{
    mem[Address] = Value;
    Cycles --;
}

void CPU::WriteWord(u32 &Cycles, Word Value, u32 Address, Memory &mem)
{
    mem[Address] = Value &  0xFF;
    mem[Address + 1] = (Value >> 8);
    Cycles --;
}

Byte CPU::FetchByte(u32 &Cycles,const Memory &mem)
{
    Byte Data = mem[PC];
    PC ++;
    Cycles --;
    return Data;
}

Word CPU::FetchWord(u32 &Cycles,const Memory &mem)
{
    Word Data = mem[PC];
    PC ++;
    Data  |= (mem[PC] << 8);
    PC ++;
    Cycles -= 2;
    return Data;
}

Byte CPU::ReadByte(u32 &Cycles, Byte Address,const Memory &mem)
{
    Byte Data = mem[Address];
    PC ++;
    Cycles --;
    return Data;    
}

Word CPU::ReadWord(u32 &Cycles, Word Address,const Memory &mem)
{
    Byte LoByte = ReadByte(Cycles, Address, mem);
    Byte HiByte = ReadByte(Cycles, Address + 1, mem);
    return LoByte | (HiByte << 8);
}

Word CPU::SPtoAddress() const
{
    return 0x100 | SP;
}

void CPU::PushBytetoStack(u32 Cycles, Byte Value, Memory &mem)
{
    WriteByte(Cycles, Value, SPtoAddress(),mem);
    SP--;
}

void CPU::PushWordtoStack(u32 Cycles, Word Value, Memory &mem)
{
    PushBytetoStack(Cycles, Value >>8, mem);
    PushBytetoStack(Cycles, (Value & 0xFF), mem);
}

void CPU::PushPCtoStack(u32 Cycles, Memory &mem)
{
    PushWordtoStack(Cycles, PC, mem);
}

void CPU::PushPCminusonetoStack(u32 Cycles, Memory &mem)
{
    PushWordtoStack(Cycles, PC - 1, mem);   
}

void CPU::PushPCplusonetoStack(u32 Cycles, Memory &mem)
{
    PushWordtoStack(Cycles, PC + 1, mem); 
}

Byte CPU::PopByteFromStack(u32 Cycles, Memory &mem)
{
    SP++;
    Cycles --;
    const Word SPW = SPtoAddress();
    Byte Value = mem[SPW];
    Cycles --;
    return Value;
}

Word CPU::PopWordFromStack(u32 Cycles, Memory &mem)
{
    Word Value = ReadWord(Cycles, SPtoAddress(), mem);
    SP += 2;
    Cycles --;
    return Value;
}

void CPU::Reset(Memory &mem)
{
    PC = 0xFFFC;
    SP = 0x0100;
    Flag.C = Flag.Z = Flag.I = Flag.D= Flag.B = Flag.V = Flag.N = 0;
    A = X = Y = 0;
    mem.Initialise();
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
                Flag.Z = (A == 0);
                Flag.N = (A & 0b10000000) > 0;
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