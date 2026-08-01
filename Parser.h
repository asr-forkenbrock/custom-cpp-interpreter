/*
Parser.h

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

#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"

struct ActionNode {
    ACTION Action; 
    std::vector<ActionNode*> Params; 
    int NUMBER = 0.00; 
    float FNUMBER = 0.00; 
    bool BOOL = false; 
    std::string STRING = ""; 
    std::string VARIABLE_NAME = ""; 
}; 

struct MainNodeAST {
    ActionNode* Data; 
    MainNodeAST* next; 
}; 
inline MainNodeAST* root; 

inline std::unordered_map<std::string, ActionNode*> Functions; 

struct PARSER_ERROR {
    bool PARSER_ERROR_DETECTED = false; 
    std::string PARSER_ERROR_DEFINITION; 
    std::string PARSER_ERROR_EXPECTED; 
    int PARSER_ERROR_TOKEN; 
    std::string PARSER_ERROR_FOUND; 
}; 
inline PARSER_ERROR PERR;


class PARSER {
    private: 
        bool func_found = false; 
        std::string ARG_NAMES[8] = { "_arg1_", "_arg2_", "_arg3_", "_arg4_", "_arg5_", "_arg6_", "_arg7_", "_arg8_" }; 
        int token_to_ast_index = 0; 
        ActionNode* NumberNode(int num);
        ActionNode* StringNode(std::string str);
        ActionNode* NumberAdditionNode(ActionNode* num1, ActionNode* num2);
        ActionNode* NumberModuloNode(ActionNode* num1, ActionNode* num2);
        ActionNode* NumberSubtractionNode(ActionNode* num1, ActionNode* num2);
        ActionNode* NumberMultiplyNode(ActionNode* num1, ActionNode* num2);
        ActionNode* NumberDivisionNode(ActionNode* num1, ActionNode* num2);
        ActionNode* VariableNode(std::string name);
        ActionNode* PrintNode(ActionNode* input);
        ActionNode* AssignmentNode(ActionNode* var, ActionNode* input);
        ActionNode* IfNode(ActionNode* input, ActionNode* body);
        ActionNode* IfElseNode(ActionNode* input, ActionNode* body, ActionNode* Else);
        ActionNode* WhileNode(ActionNode* input, ActionNode* body);
        ActionNode* CompareNode(ACTION act, ActionNode* val1, ActionNode* val2); 
        ActionNode* ConditionNode(ActionNode* input); 
        ActionNode* FuncNode(std::string name); 
        ActionNode* BuiltInFuncNode(int params); 

    public: 
        ActionNode* Parse_Token_Number(int token_index); 
        ActionNode* Parse_Token_Variable(int &token_index); 
        ActionNode* Parse_Token_String(int token_index); 
        int FindNewLine(int index); 
        int FindRightParen_FromBack(int start_index, int end_index); 
        ActionNode* ParsePrimaryInput(int &token_index, int end_index); 
        ACTION ParseBinaryOperator(int &token_index); 
        ActionNode* ParseExpression(int start_index, int end_index); 
        void ParseLetStatement(ActionNode* &data, int &token_index); 
        void ParsePrintStatement(ActionNode* &data, int &token_index); 
        void ParseWhileStatement(ActionNode* &data, int &token_index); 
        void ParseIfStatement(ActionNode* &data, int &token_index); 
        void ParseFunctionStatement(int &token_index); 
        void parseReturnStatement(ActionNode* &data, int &token_index); 
        void parseContinueStatement(ActionNode* &data, int &token_index); 
        void parseBreakStatement(ActionNode* &data, int &token_index); 
        void ParseInnerStatement(ActionNode* &Data, int &token_index); 
        void ParseStatement(MainNodeAST* &current, int &token_index); 
        void Build_Built_ins(); 
        void ParseProgram(); 
}; 

#endif
