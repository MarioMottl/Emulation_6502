#include <stdio.h>
#include <stdlib.h>

namespace m6502
{
    using Byte = unsigned char;
    using Word = unsigned short;
    using u32 = unsigned int;

    class Memory
    {
    private:
        static constexpr u32 MAX_MEM = 1024 * 64;
        Byte Data[MAX_MEM];
    public:
        void Initialise();
        Byte operator[](u32 Address) const;
        Byte& operator[](u32 Address);
    };

    class CPU
    {
    private:
        //Flags
        Byte C : 1;	//0: Carry Flag	
	    Byte Z : 1;	//1: Zero Flag
	    Byte I : 1; //2: Interrupt disable
	    Byte D : 1; //3: Decimal mode
	    Byte B : 1; //4: Break
	    Byte Unused : 1; //5: Unused
	    Byte V : 1; //6: Overflow
	    Byte N : 1; //7: Negative

        Word PC;
        Word SP;

        //Registers
        Byte A, X, Y;


    public:
        static constexpr Byte
            INS_LDA_IM = 0x49;
    
        void WriteWord(u32 &Cycles, Word Value, u32 Address, Memory &mem);
        void WriteByte(u32 &Cycles, Word Value, u32 Address,Memory &mem);
        Byte FetchByte (u32 &Cycles,const Memory &mem);
        Word FetchWord(u32 &Cycles,const Memory &mem);
        Byte ReadByte(u32 &Cycles,Byte Address,const Memory &mem);
        Word ReadWord(u32 &Cycles, Word Address,const Memory &mem);

        void Reset(Memory &mem);
        void Execute(u32 Cycles,Memory &mem);
    };
    
}

using namespace m6502;