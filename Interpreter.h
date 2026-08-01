/*
Interpreter.h

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

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Parser.h"

struct Values {
    ACTION type = None;  
    int Number = 0; 
    bool Boolean = false; 
    std::string String = "";
    std::string Variable = "";  
}; 
struct RUNTIME_ERROR {
    bool ERROR_DETECTED = false; 
    std::string ERROR_DEFINITION; 
    std::string ERROR_EXPECTED; 
    int ERROR_TOKEN; 
    std::string ERROR_FOUND; 
}; 

inline std::random_device rd;
inline std::mt19937 gen(rd());
inline std::unordered_map<std::string, Values (*)()> BuiltInTable; 
inline Values return_value; 
inline std::stack<std::unordered_map<std::string, Values>> Prog_Variables;
inline RUNTIME_ERROR RunErr; 


class VirtualMachine {
    private: 
        std::string ACTIONS_STR[50] = {
            "None", 
            "Assignment", 
            "Print", 
            "If", 
            "Condition", 
            "Body", 
            "Else", 
            "While", 
            "Function", 
            "Parameters", 
            "Function Call", 
            "Return",  
            "Break", 
            "Continue", 
            
            "Number", 
            "String", 
            "Variable", 
            "Bool", 

            "Add", 
            "Sub",
            "Mul", 
            "Div",
            "Mod", 
            
            "Equal", 

            "Logic_And",
            "Logic_Or",

            "Cmp_NotEqual", 
            "Cmp_Equal",
            "Cmp_GE",
            "Cmp_LE",
            "Cmp_GT", 
            "Cmp_LT", 

            "Syntax_Left_Paren", 
            "Syntax_Right_Paren", 
            "Syntax_Left_Brace", 
            "Syntax_Right_Brace", 
            "Syntax_Left_Bracket",
            "Syntax_Right_Bracket",
            "Syntax_Comma", 
            "Syntax_NewLine", 

            "BuiltInFunction", 

            "EndProgram", 
            "Eof"
        };

        bool break_called = false; 
        bool continue_called = false; 
        bool return_called = false; 

        void help_display_ast(int depth);
        void RecurseDisplayAST(ActionNode* root, int &depth, int param);
        void DisplayAST(); 
        Values EvaluateExpression(ActionNode* input);
        void assignment_interpreter(ActionNode* input);
        void print_interpreter(ActionNode* input);
        void return_interpreter(ActionNode* input);
        void if_interpreter(ActionNode* input);
        void while_interpreter(ActionNode* input);
        void ExecuteFunctionCall(ActionNode* input);
        void ExecuteStatement(ActionNode* input);
        void ExecuteProgramAST();

        void RecurseRemoveAST(ActionNode* root, int &depth, int param); 
        void RemoveAST(); 

    public: 
        void RunProgram(std::string name);
}; 

#endif
