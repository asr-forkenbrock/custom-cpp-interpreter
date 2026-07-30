#include "Interpreter.h"

//  clear && g++ main.cpp Lexer.cpp Parser.cpp Interpreter.cpp -o asr-envi && ./asr-envi

int main(){ 
    VirtualMachine VM; 
    VM.RunProgram("Test.asr");
    return 0; 
}

