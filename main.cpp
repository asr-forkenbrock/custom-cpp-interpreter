#include "Interpreter.h"

int main(int argc, char* argv[]) {
    if(argc > 1){
        std::string name = argv[1]; 
        VirtualMachine VM; 
        VM.RunProgram(name);
    }else{
        VirtualMachine VM; 
        VM.RunProgram("Demos/17_full_demo.asr");
    }
    return 0; 
}
