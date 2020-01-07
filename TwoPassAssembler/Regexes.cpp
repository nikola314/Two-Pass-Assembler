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

// TODO: Instructions

Regexes::Regexes()
{
}


Regexes::~Regexes()
{
}
