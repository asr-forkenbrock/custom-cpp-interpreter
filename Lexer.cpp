/*
Lexer.cpp

ASR Interpreter - Lexer, Parser, and Interpreter written in C++.
Copyright (C) 2026  Noah Forkenbrock <asr-forkenbrock> 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Lexer.h"

        void LEXER::LoadProgram(std::string filename){
            std::cout << "Loading Program file...\n";
            std::cout << "File: " << filename << "\n";

            std::ifstream inputFile(filename); 
            std::string line;
            if(!inputFile.is_open()){ std::cout << "Unable to located or open file! \n"; return; }

            if(lexer_debug){ std::cout << "== RAW ==================================\n"; }
            while(std::getline(inputFile, line)){ 
                if(line.length() > 0){
                    if(line[0] == '/' && line[1] == '/'){ continue; }
                    if(lexer_debug){ std::cout << line << std::endl; }
                    ProgramLines.push_back(line); 
                }
            }
            if(lexer_debug){ std::cout << "== RAW ==================================\n\n"; }

            inputFile.close(); 
        }


        bool LEXER::CheckKeyWord(int line, int start, int end){
            std::string tmp = ProgramLines[line].substr(start, end-start); 
            bool check = false; 
            if(!string_found && tmp.find('"') == std::string::npos){
                if(tmp.find("let") != std::string::npos){
                    ProgramTokens.push_back(Token{Assignment, "LET"}); check=true; 
                }
                if(tmp.find("end") != std::string::npos){
                    ProgramTokens.push_back(Token{EndProgram, "END_PROGRAM"}); check=true; 
                }
                if(tmp.find("print") != std::string::npos){
                    ProgramTokens.push_back(Token{Print, "PRINT"}); check=true; 
                }
                if(tmp.find("if") != std::string::npos){
                    ProgramTokens.push_back(Token{If, "IF"}); check=true; 
                }
                if(tmp.find("while") != std::string::npos){
                    ProgramTokens.push_back(Token{While, "WHILE"}); check=true; 
                }
                if(tmp.find("else") != std::string::npos){
                    ProgramTokens.push_back(Token{Else, "ELSE"}); check=true; 
                }
                if(tmp.find("break") != std::string::npos){
                    ProgramTokens.push_back(Token{Break, "Break"}); check=true; 
                }
                if(tmp.find("continue") != std::string::npos){
                    ProgramTokens.push_back(Token{Continue, "Continue"}); check=true; 
                }
                if(tmp.find("func") != std::string::npos){
                    ProgramTokens.push_back(Token{Function, "Function"}); check=true; 
                }
                if(tmp.find("return") != std::string::npos){
                    ProgramTokens.push_back(Token{Return, "Return"}); check=true; 
                }
                if(tmp.find("!=") != std::string::npos){ 
                    ProgramTokens.push_back(Token{Cmp_NotEqual, "NOT_EQUAL"}); check=true; return check; 
                }
                if(tmp.find("==") != std::string::npos){
                    ProgramTokens.push_back(Token{Cmp_Equal, "E_CMP"}); check=true; return check; 
                }
                if(tmp.find(">=") != std::string::npos){
                    ProgramTokens.push_back(Token{Cmp_GE, "GE_CMP"}); check=true; return check; 
                }
                if(tmp.find("<=") != std::string::npos){
                    ProgramTokens.push_back(Token{Cmp_LE, "LE_CMP"}); check=true; return check; 
                }
                if(tmp.find("&&") != std::string::npos){
                    ProgramTokens.push_back(Token{Logic_And, "AND"}); check=true; return check; 
                }
                if(tmp.find("||") != std::string::npos){
                    ProgramTokens.push_back(Token{Logic_Or, "OR"}); check=true; return check; 
                }
                if(tmp.find(">") != std::string::npos){
                    ProgramTokens.push_back(Token{Cmp_GT, "GT_CMP"}); check=true; return check; 
                }
                if(tmp.find("<") != std::string::npos){
                    ProgramTokens.push_back(Token{Cmp_LT, "LT_CMP"}); check=true; return check; 
                }
                if(tmp.find("=") != std::string::npos){
                    ProgramTokens.push_back(Token{Equal, "EQUAL"}); check=true; return check; 
                }
                if(tmp.find("+") != std::string::npos){
                    ProgramTokens.push_back(Token{Add, "PLUS"}); check=true; return check; 
                }
                if(tmp.find("-") != std::string::npos){
                    ProgramTokens.push_back(Token{Sub, "MINUS"}); check=true; return check; 
                }
                if(tmp.find("*") != std::string::npos){
                    ProgramTokens.push_back(Token{Mul, "MULTIPLY"}); check=true; return check; 
                }
                if(tmp.find("/") != std::string::npos){
                    ProgramTokens.push_back(Token{Div, "DIVIDE"}); check=true; return check; 
                }
                if(tmp.find("%") != std::string::npos){
                    ProgramTokens.push_back(Token{Mod, "MODULO"}); check=true; return check; 
                }
            }
            return check; 
        }

        bool LEXER::CheckNumeric(int line, int start, int end){
            std::string tmp = ProgramLines[line].substr(start, end-start); 
            bool check = true; 
            if(tmp.length() == 0){
                return false; 
            }
            for(int i=0; i<tmp.length(); i++){
                if(tmp[i] < '0' || tmp[i] > '9'){ 
                    check = false; 
                }
            }
            if(check){
                ProgramTokens.push_back(Token{Number, tmp});
            }
            return check; 
        }

        bool LEXER::CheckVar(int line, int start, int end){
            std::string tmp = ProgramLines[line].substr(start, end-start); 
            bool check = true; 
            if(tmp.length() == 0 || !std::isalpha(tmp[0]) && tmp[0] != '_'){ return false; }

            for(char c : tmp){
                if(!std::isalnum(c) && c != '_'){ return false; }
            }

            if(check){ ProgramTokens.push_back(Token{Variable, "_"+tmp+"_"}); }
            return check; 
        }

        bool LEXER::CheckSyntax(int line, int index){
            output = ""; 
            bool check = false; 
            if(ProgramLines[line][index] == '"'){
                if(string_found){ 
                    string_found=false; 
                    output = string_tmp; 
                    string_tmp=""; 
                    outputType = String; 
                    return true;
                }else{ 
                    string_tmp = ""; 
                    string_found=true; 
                }
            }

            if(!string_found){
                if(ProgramLines[line][index] == ' '){ 
                    check = true; 
                }
                if(ProgramLines[line][index] == '('){
                    outputType = Syntax_Left_Paren; output = "LEFT_PAREN"; check = true; 
                }
                if(ProgramLines[line][index] == ')'){
                    outputType = Syntax_Right_Paren; output = "RIGHT_PAREN"; check = true; 
                }
                if(ProgramLines[line][index] == '{'){
                    outputType = Syntax_Left_Brace; output = "LEFT_BRACE"; check = true; 
                }
                if(ProgramLines[line][index] == '}'){
                    outputType = Syntax_Right_Brace; output = "RIGHT_BRACE"; check = true; 
                }
                if(ProgramLines[line][index] == '['){
                    outputType = Syntax_Left_Bracket; output = "LEFT_BRACKET"; check = true; 
                }
                if(ProgramLines[line][index] == ']'){
                    outputType = Syntax_Right_Bracket; output = "RIGHT_BRACKET"; check = true; 
                }
                if(ProgramLines[line][index] == ','){
                    outputType = Syntax_Comma; output = "COMMA"; check = true; 
                }
                if(ProgramLines[line][index] == '\n' or index == ProgramLines[line].length()){
                    outputType = Syntax_NewLine; output = "____NEWLINE____"; check = true; 
                }

            }else{
                if(ProgramLines[line][index] != '"'){
                    string_tmp += ProgramLines[line][index]; 
                }
            }
            return check; 
        }
        


        void LEXER::TokenizeProgram(){
            for(int i=0; i<ProgramLines.size(); i++){
                int pointer_a = 0; 
                int pointer_b = 0; 
                while(pointer_b <= ProgramLines[i].length()){
                    if(CheckSyntax(i, pointer_b)){
                        if(string_found == false){
                            if(CheckKeyWord(i, pointer_a, pointer_b)){
                            }else if(CheckNumeric(i, pointer_a, pointer_b)){
                            }else{ CheckVar(i, pointer_a, pointer_b); }
                            pointer_a=pointer_b+1; 
                            if(output.length() > 0){ ProgramTokens.push_back(Token{outputType, output}); }
                        }
                    }
                    pointer_b++; 
                }
            }
            ProgramTokens.push_back(Token{Eof, "END_OF_FILE"});

            if(lexer_debug){ 
                std::cout << "== TOKENS ==================================\n"; 
                for(int i=0; i<ProgramTokens.size(); i++){
                    std::cout << "[" << i << "]  Type: " << ProgramTokens[i].type << "  -  Value: " << ProgramTokens[i].value << "\n"; 
                    if(ProgramTokens[i].type == Syntax_NewLine){ std::cout << std::endl; }
                }
                std::cout << "== TOKENS ==================================\n\n"; 
            }
            if(LEXER_ERROR_DETECTED){ std::cerr << "\n\n ******** Lexer has detected a fault! ******** \n\n\n"; }
        }
