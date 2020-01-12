#include "Regexes.h"
#include "definitions.h"

using namespace std;

// Types
regex Regexes::LABEL("^[a-zA-z_]+[a-zA-z0-9_]*:.*");
regex Regexes::DIRECTIVE("^\\.(section|text|data|bss|end|extern|global|equ|byte|word|align|skip).*");
regex Regexes::INSTRUCTION("[a-zA-z]+.*"); // check for label before instruction

// Directives
regex Regexes::SECTION("^\\.section");
regex Regexes::TEXT("^\\.text");
regex Regexes::DATA("^\\.data");
regex Regexes::BSS("^\\.bss");
regex Regexes::END("^\\.end");
regex Regexes::BYTE("^\\.byte.*");
regex Regexes::WORD("^\\.word.*");
regex Regexes::ALIGN("^\\.align.*");
regex Regexes::SKIP("^\\.skip.*");
regex Regexes::GLOBAL("^\\.global");
regex Regexes::EXTERN("^\\.extern");
regex Regexes::EQU("^\\.equ.*");

// Instructions
regex Regexes::HALT("halt");
regex Regexes::XCHG("xchg(b|w)?");
regex Regexes::INT("int(b|w)?");
regex Regexes::MOV("mov(b|w)?");
regex Regexes::ADD("add(b|w)?");
regex Regexes::SUB("sub(b|w)?");
regex Regexes::MUL("mul(b|w)?");
regex Regexes::DIV("div(b|w)?");
regex Regexes::CMP("cmp(b|w)?");
regex Regexes::NOT("not(b|w)?");
regex Regexes::AND("and(b|w)?");
regex Regexes::OR("or(b|w)?");
regex Regexes::XOR("xor(b|w)?");
regex Regexes::TEST("test(b|w)?");
regex Regexes::SHL("shl(b|w)?");
regex Regexes::SHR("shr(b|w)?");
regex Regexes::PUSH("push(b|w)?");
regex Regexes::POP("pop(b|w)?");
regex Regexes::JMP("jmp(b|w)?");
regex Regexes::JEQ("jeq(b|w)?");
regex Regexes::JNE("jne(b|w)?");
regex Regexes::JGT("jgt(b|w)?");
regex Regexes::CALL("call(b|w)?");
regex Regexes::RET("ret");
regex Regexes::IRET("iret?");

// Address modes
regex Regexes::IMMED("-?\\d.*");
regex Regexes::REGDIR("(r\\d(l|h)?|pc|sp)");
regex Regexes::REGINDPOM("(r\\d(l|h)?|pc|sp)\\[-?\\d+\\]");
regex Regexes::REGINDPOM_SYM("(r\\d(l|h)?|pc|sp)\\[-?[a-zA-Z_]+[a-zA-Z_0-9]*\\]");
regex Regexes::IMMED_SYM("&[a-zA-Z_]+[a-zA-Z_0-9]*");
regex Regexes::PCREL_SYM("\\$[a-zA-Z_]+[a-zA-Z_0-9]*");
regex Regexes::ABS("\\*\\d.*");
regex Regexes::ABS_SYM("[a-zA-Z_]+[a-zA-Z_0-9]*");
regex Regexes::REGIND("\\[(r\\d(l|h)?|pc|sp)\\]");

// Symbol
regex Regexes::SYMBOL("[a-zA-Z_]+[a-zA-Z_0-9]*");

// Extracting operands
regex Regexes::REGINDPOM_OFFSET("(r\\d(l|h)?|pc|sp)\\[(-?\\d+|[a-zA-Z0-9_]+)\\]");
regex Regexes::IMMED_OPERAND("(-?\\d.*|&[a-zA-Z_]+[a-zA-Z_0-9]*)");
regex Regexes::MEMDIR_OPERAND("(\\$[a-zA-Z_]+[a-zA-Z_0-9]*|[a-zA-Z_]+[a-zA-Z_0-9]*|\\*-?\\d.*)");

Regexes::Regexes()
{
}


Regexes::~Regexes()
{
}
