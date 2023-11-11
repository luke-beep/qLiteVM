namespace qLiteVM;

internal class Program
{
    #region Variables
    private const int MEMORY_MAX = 1 << 16;

    private static readonly ushort[] memory = new ushort[MEMORY_MAX];
    private static readonly ushort[] reg = new ushort[(int)Register.R_COUNT];

    private static bool running = true;
    #endregion

    private static void Main(string[] args)
    {
        if (args.Length < 1)
        {
            Console.WriteLine("qLiteM [image-file1] ...");
            Environment.Exit(2);
        }

        foreach (var t in args)
        {
            if (ReadImage(t)) continue;
            Console.WriteLine("Failed to load image: {0}", t);
            Environment.Exit(1);
        }

        reg[(int)Register.R_COND] = (ushort)Flags.ZRO;
        reg[(int)Register.R_PC] = 0x3000;


        while (running)
        {
            var instr = MemRead(reg[(int)Register.R_PC]++);
            var op = (ushort)(instr >> 12);

            switch (op)
            {
                case (ushort)Opcode.ADD:
                    Add(instr);
                    break;
                case (ushort)Opcode.AND:
                    And(instr);
                    break;
                case (ushort)Opcode.NOT:
                    Not(instr);
                    break;
                case (ushort)Opcode.BR:
                    Branch(instr);
                    break;
                case (ushort)Opcode.JMP:
                    Jump(instr);
                    break;
                case (ushort)Opcode.JSR:
                    JSR(instr);
                    break;
                case (ushort)Opcode.LD:
                    Load(instr);
                    break;
                case (ushort)Opcode.LDI:
                    LoadRegister(instr);
                    break;
                case (ushort)Opcode.LDR:
                    LDR(instr);
                    break;
                case (ushort)Opcode.LEA:
                    LoadEffectiveAddress(instr);
                    break;
                case (ushort)Opcode.ST:
                    Store(instr);
                    break;
                case (ushort)Opcode.STI:
                    StoreRegister(instr);
                    break;
                case (ushort)Opcode.STR:
                    STR(instr);
                    break;
                case (ushort)Opcode.TRAP:
                    HandleTrap(instr);
                    break;
                case (ushort)Opcode.RES:
                    Reserved(instr);
                    break;
                case (ushort)Opcode.RTI:
                    Reserved(instr);
                    break;
                default:
                    Reserved(instr);
                    break;
            }
        }
    }

    #region Image Loading
    private static bool ReadImage(string imagePath)
    {
        try
        {
            using var reader = new BinaryReader(File.Open(imagePath, FileMode.Open));
            var origin = Swap16(reader.ReadUInt16());
            Console.WriteLine($"Image origin: {origin}");

            var count = 0;
            while (reader.BaseStream.Position < reader.BaseStream.Length)
            {
                memory[origin] = Swap16(reader.ReadUInt16());
                Console.WriteLine($"Loaded word at address {origin:X4}: {memory[origin]:X4}");
                origin++;
                count++;
            }

            Console.WriteLine($"Loaded {count} words into memory.");
            return true;
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error reading image file: {ex.Message}");
            return false;
        }
    }

    private static ushort Swap16(ushort x)
    {
        return (ushort)((x << 8) | (x >> 8));
    }
    #endregion

    #region Memory Functions
    private static void MemWrite(ushort address, ushort val)
    {
        memory[address] = val;
    }

    private static ushort MemRead(ushort address)
    {
        if (address != (int)MemoryMappedRegister.MR_KBSR) return memory[address];
        if (Console.KeyAvailable)
        {
            memory[(ushort)MemoryMappedRegister.MR_KBSR] = 1 << 15;
            memory[(ushort)MemoryMappedRegister.MR_KBDR] = Console.ReadKey(true).KeyChar;
        }
        else
        {
            memory[(ushort)MemoryMappedRegister.MR_KBSR] = 0;
        }

        return memory[address];
    }
    #endregion

    #region Instructions
    private static void Add(ushort instruction)
    {
        var r0 = (ushort)((instruction >> 9) & 0x7);
        var r1 = (ushort)((instruction >> 6) & 0x7);
        var immFlag = (ushort)((instruction >> 5) & 0x1);


        if (immFlag != 0)
        {
            var imm5 = SignExtend((ushort)(instruction & 0x1F), 5);
            reg[r0] = (ushort)(reg[r1] + imm5);
        }
        else
        {
            var r2 = (ushort)(instruction & 0x7);
            reg[r0] = (ushort)(reg[r1] + reg[r2]);
        }

        UpdateFlags(r0);
    }

    private static void And(ushort instruction)
    {
        var r0 = (ushort)((instruction >> 9) & 0x7);
        var r1 = (ushort)((instruction >> 6) & 0x7);
        var immFlag = (ushort)((instruction >> 5) & 0x1);

        if (immFlag != 0)
        {
            var imm5 = SignExtend((ushort)(instruction & 0x1F), 5);
            reg[r0] = (ushort)(reg[r1] & imm5);
        }
        else
        {
            var r2 = (ushort)(instruction & 0x7);
            reg[r0] = (ushort)(reg[r1] & reg[r2]);
        }

        UpdateFlags(r0);
    }

    private static void Not(ushort instruction)
    {
        var r0 = (ushort)((instruction >> 9) & 0x7);
        var r1 = (ushort)((instruction >> 6) & 0x7);

        reg[r0] = (ushort)~reg[r1];
        UpdateFlags(r0);
    }

    private static void Branch(ushort instruction)
    {
        var condFlag = (ushort)((instruction >> 9) & 0x7);
        var pcOffset = SignExtend((ushort)(instruction & 0x1FF), 9);

        if ((condFlag & reg[(int)Register.R_COND]) != 0) reg[(int)Register.R_PC] += pcOffset;
    }

    private static void Jump(ushort instruction)
    {
        var r1 = (ushort)((instruction >> 6) & 0x7);

        reg[(int)Register.R_PC] = reg[r1];
    }

    private static void JSR(ushort instruction)
    {
        var longFlag = (ushort)((instruction >> 11) & 1);

        reg[(int)Register.R_R7] = reg[(int)Register.R_PC];

        if (longFlag != 0)
        {
            var longPcOffset = SignExtend((ushort)(instruction & 0x7FF), 11);

            reg[(int)Register.R_PC] += longPcOffset;
        }
        else
        {
            var r1 = (ushort)((instruction >> 6) & 0x7);
            reg[(int)Register.R_PC] = reg[r1];
        }
    }

    private static void Load(ushort instruction)
    {
        var r0 = (ushort)((instruction >> 9) & 0x7);

        var pcOffset = SignExtend((ushort)(instruction & 0x1FF), 9);

        reg[r0] = MemRead((ushort)(reg[(int)Register.R_PC] + pcOffset));
        UpdateFlags(r0);
    }

    private static void LoadRegister(ushort instruction)
    {
        var r0 = (ushort)((instruction >> 9) & 0x7);

        var pcOffset = SignExtend((ushort)(instruction & 0x1FF), 9);

        var address = MemRead((ushort)(reg[(int)Register.R_PC] + pcOffset));

        reg[r0] = MemRead(address);
        UpdateFlags(r0);
    }

    private static void LDR(ushort instruction)
    {
        var r0 = (ushort)((instruction >> 9) & 0x7);

        var r1 = (ushort)((instruction >> 6) & 0x7);

        var pcOffset = SignExtend((ushort)(instruction & 0x3F), 6);

        reg[r0] = MemRead((ushort)(reg[r1] + pcOffset));
        UpdateFlags(r0);
    }

    private static void LoadEffectiveAddress(ushort instruction)
    {
        var r0 = (ushort)((instruction >> 9) & 0x7);
        var pcOffset = SignExtend((ushort)(instruction & 0x1FF), 9);
        reg[r0] = (ushort)(reg[(int)Register.R_PC] + pcOffset);
    }

    private static void Store(ushort instruction)
    {
        var r0 = (ushort)((instruction >> 9) & 0x7);

        var pcOffset = SignExtend((ushort)(instruction & 0x1FF), 9);
        MemWrite((ushort)(reg[(int)Register.R_PC] + pcOffset), reg[r0]);
    }

    private static void StoreRegister(ushort instruction)
    {
        var r0 = (ushort)((instruction >> 9) & 0x7);

        var pcOffset = SignExtend((ushort)(instruction & 0x1FF), 9);

        var address = MemRead((ushort)(reg[(int)Register.R_PC] + pcOffset));
        MemWrite(address, reg[r0]);
    }

    private static void STR(ushort instruction)
    {
        var r0 = (ushort)((instruction >> 9) & 0x7);

        var r1 = (ushort)((instruction >> 6) & 0x7);

        var pcOffset = SignExtend((ushort)(instruction & 0x3F), 6);

        MemWrite((ushort)(reg[r1] + pcOffset), reg[r0]);
    }

    private static void HandleTrap(ushort instruction)
    {
        reg[(int)Register.R_R7] = (ushort)(reg[(int)Register.R_PC] + 1);
        switch (instruction & 0xFF)
        {
            case (ushort)TrapVector.TRAP_GETC:
                reg[(int)Register.R_R0] = Console.ReadKey(true).KeyChar;
                UpdateFlags((ushort)Register.R_R0);
                break;
            case (ushort)TrapVector.TRAP_OUT:
                Console.Write((char)reg[(int)Register.R_R0]);
                break;
            case (ushort)TrapVector.TRAP_PUTS:
            {
                var address = reg[(int)Register.R_R0];
                var character = (char)MemRead(address);
                while (character != 0)
                {
                    Console.Write(character);
                    address++;
                    character = (char)MemRead(address);
                }

                Console.WriteLine();
                break;
            }
            case (ushort)TrapVector.TRAP_IN:
            {
                Console.Write("Enter a character: ");
                var c = Console.ReadKey(true).KeyChar;
                Console.WriteLine();
                reg[(int)Register.R_R0] = c;
                UpdateFlags((ushort)Register.R_R0);
                break;
            }
            case (ushort)TrapVector.TRAP_PUTSP:
            {
                var address = reg[(int)Register.R_R0];
                var character = (char)MemRead(address);
                while (character != 0)
                {
                    var char1 = (byte)(character & 0xFF);
                    Console.Write((char)char1);
                    var char2 = (byte)(character >> 8);
                    if (char2 != 0) Console.Write((char)char2);
                    address++;
                    character = (char)MemRead(address);
                }

                Console.WriteLine();
                break;
            }
            case (ushort)TrapVector.TRAP_HALT:
                Console.WriteLine("HALT");
                running = false;
                break;
        }
    }

    private static void Reserved(ushort instruction)
    {
        throw new Exception("Bad opcode");
    }
    #endregion

    #region Instruction Helpers
    private static void UpdateFlags(ushort r)
    {
        if (reg[r] == 0)
            reg[(int)Register.R_COND] = (ushort)Flags.ZRO;
        else if (reg[r] >> 15 != 0)
            reg[(int)Register.R_COND] = (ushort)Flags.NEG;
        else
            reg[(int)Register.R_COND] = (ushort)Flags.POS;
    }

    private static ushort SignExtend(ushort x, int bitCount)
    {
        if (((x >> (bitCount - 1)) & 1) != 0) x |= (ushort)(0xFFFF << bitCount);
        return x;
    }
    #endregion

    #region Registers
    public enum Register
    {
        R_R0 = 0,
        R_R1,
        R_R2,
        R_R3,
        R_R4,
        R_R5,
        R_R6,
        R_R7,
        R_PC, 
        R_COND,
        R_COUNT
    }

    public enum TrapVector
    {
        TRAP_GETC = 0x20,
        TRAP_OUT = 0x21,
        TRAP_PUTS = 0x22, 
        TRAP_IN = 0x23, 
        TRAP_PUTSP = 0x24,
        TRAP_HALT = 0x25
    }

    public enum Flags
    {
        POS = 1 << 0,
        ZRO = 1 << 1,
        NEG = 1 << 2
    }

    public enum Opcode
    {
        BR = 0,
        ADD,
        LD, 
        ST, 
        JSR, 
        AND, 
        LDR, 
        STR, 
        RTI, 
        NOT, 
        LDI, 
        STI, 
        JMP, 
        RES, 
        LEA, 
        TRAP 
    }


    public enum MemoryMappedRegister
    {
        MR_KBSR = 0xFE00,
        MR_KBDR = 0xFE02 
    }
    #endregion
}