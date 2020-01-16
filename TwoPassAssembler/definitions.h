#ifndef __DEFINITIONS__H
#define __DEFINITIONS__H 1
#include <vector>
#include <string>

#define EXCEPTION_INVALID_ARGUMENTS 1
#define EXCEPTION_WRONG_INPUT_FILE 2

#define CPU_WORD 2
#define SP 6
#define PC 7
#define PSW 0xF

typedef std::vector<std::string> ParsedLine;
typedef std::vector<ParsedLine> ParsedFile;

enum Scope{SCOPE_LOCAL, SCOPE_GLOBAL};

enum RelocationType{ R_386_8, R_386_16 };

enum WordType { LABEL, DIRECTIVE, INSTRUCTION };
enum DirectiveType { SECTION, TEXT, DATA, BSS, END, BYTE, WORD, ALIGN, SKIP, GLOBAL, EXTERN, EQU };
enum InstructionType {
	HALT = 1, XCHG, INT, MOV, ADD, SUB, MUL, DIV, CMP, NOT, AND, OR, XOR,
	TEST, SHL, SHR, PUSH, POP, JMP, JEQ, JNE, JGT, CALL, RET, IRET
};

enum AddressType { IMMED, IMMED_SYM, ABS, ABS_SYM, REGINDPOM, REGINDPOM_SYM, REGDIR, PCREL_SYM, REGIND };
enum AddressMode { A_IMMED = 0, A_REGDIR, A_REGIND, A_REGINDPOM_B, A_REGINDPOM_W, A_MEMDIR };


#endif