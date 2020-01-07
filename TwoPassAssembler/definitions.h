#ifndef __EXCEPTIONS__H
#define __EXCEPTIONS__H 1
#include <vector>
#include <string.h>

#define EXCEPTION_INVALID_ARGUMENTS 1
#define EXCEPTION_WRONG_INPUT_FILE 2

typedef std::vector<std::string> ParsedLine;
typedef std::vector<ParsedLine> ParsedFile;


#define EI_NIDENT 16
typedef unsigned char Elf32_Half[2];
typedef unsigned char Elf32_Word[4];
typedef unsigned char Elf32_Addr[4];
typedef unsigned char Elf32_Off[4];
typedef unsigned char Elf32_Byte;

enum Scope{SCOPE_LOCAL, SCOPE_GLOBAL};

enum WordType{LABEL, DIRECTIVE, INSTRUCTION};
enum DirectiveType { SECTION, TEXT, DATA, BSS, END, BYTE, WORD, ALIGN, SKIP, GLOBAL, EXTERN };
enum InstructionType {};

typedef struct {
	Elf32_Word name;
	Elf32_Addr value;
	Elf32_Word size;
	Elf32_Byte type : 4, binding : 4;
	Elf32_Byte reserved;
	Elf32_Byte section;
} Elf_Symbol;


#endif