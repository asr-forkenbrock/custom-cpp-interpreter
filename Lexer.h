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
