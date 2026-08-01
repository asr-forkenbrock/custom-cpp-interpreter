/*
Lexer.h

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

#ifndef LEXER_H
#define LEXER_H

#include "config.h"



enum ACTION {
    None, 
    Assignment, 
    Print, 
    If, 
    Cond, 
    Body, 
    Else, 
    While, 
    Function, 
    Parameters, 
    FunctionCall, 
    Return, 
    Break, 
    Continue, 
    Number, 
    String, 
    Variable, 
    Bool, 
    Add, 
    Sub,
    Mul, 
    Div,
    Mod, 
    Equal, 
    Logic_And,
    Logic_Or,
    Cmp_NotEqual, 
    Cmp_Equal,
    Cmp_GE,
    Cmp_LE,
    Cmp_GT, 
    Cmp_LT, 
    Syntax_Left_Paren, 
    Syntax_Right_Paren, 
    Syntax_Left_Brace, 
    Syntax_Right_Brace, 
    Syntax_Left_Bracket,
    Syntax_Right_Bracket,
    Syntax_Comma, 
    Syntax_NewLine, 
    BuiltInFunction, 
    EndProgram, 
    Eof
}; 

struct Token {
    ACTION type; 
    std::string value=""; 
}; 

inline bool LEXER_ERROR_DETECTED = false; 
inline std::vector<Token> ProgramTokens; 

        
class LEXER {
    private: 
        std::string string_tmp; 
        std::vector<std::string> ProgramLines; 
        bool string_found = false; 
        std::string output; 
        ACTION outputType; 

    public: 
        void LoadProgram(std::string filename); 
        bool CheckKeyWord(int line, int start, int end);
        bool CheckNumeric(int line, int start, int end); 
        bool CheckVar(int line, int start, int end);
        bool CheckSyntax(int line, int index); 
        void TokenizeProgram(); 
    }; 


#endif
