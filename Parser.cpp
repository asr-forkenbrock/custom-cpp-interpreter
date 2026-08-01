/*
Parser.cpp

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

#include "Parser.h"

        ActionNode* PARSER::NumberNode(int num){
            ActionNode* num1 = new ActionNode();  
            num1->Action = Number; 
            num1->NUMBER = num; 
            return num1; 
        }

        ActionNode* PARSER::StringNode(std::string str){
            ActionNode* str1 = new ActionNode();  
            str1->Action = String; 
            str1->STRING = str; 
            return str1; 
        }

        ActionNode* PARSER::NumberAdditionNode(ActionNode* num1, ActionNode* num2){
            ActionNode* add1 = new ActionNode();  
            add1->Action = Add; 
            add1->Params.push_back(num1);
            add1->Params.push_back(num2);
            return add1; 
        }

        ActionNode* PARSER::NumberModuloNode(ActionNode* num1, ActionNode* num2){
            ActionNode* add1 = new ActionNode();  
            add1->Action = Mod; 
            add1->Params.push_back(num1);
            add1->Params.push_back(num2);
            return add1; 
        }

        ActionNode* PARSER::NumberSubtractionNode(ActionNode* num1, ActionNode* num2){
            ActionNode* add1 = new ActionNode();  
            add1->Action = Sub; 
            add1->Params.push_back(num1);
            add1->Params.push_back(num2);
            return add1; 
        }

        ActionNode* PARSER::NumberMultiplyNode(ActionNode* num1, ActionNode* num2){
            ActionNode* add1 = new ActionNode();  
            add1->Action = Mul; 
            add1->Params.push_back(num1);
            add1->Params.push_back(num2);
            return add1; 
        }

        ActionNode* PARSER::NumberDivisionNode(ActionNode* num1, ActionNode* num2){
            ActionNode* add1 = new ActionNode();  
            add1->Action = Div; 
            add1->Params.push_back(num1);
            add1->Params.push_back(num2);
            return add1; 
        }

        ActionNode* PARSER::VariableNode(std::string name){
            ActionNode* var = new ActionNode();  
            var->Action = Variable; 
            var->VARIABLE_NAME = name; 
            return var; 
        }

        ActionNode* PARSER::PrintNode(ActionNode* input){
            ActionNode* Data = new ActionNode(); 
            Data->Action = Print; 
            Data->Params.push_back(input);   
            return Data; 
        }

        ActionNode* PARSER::AssignmentNode(ActionNode* var, ActionNode* input){
            ActionNode* Data = new ActionNode();  
            Data->Action = Assignment; 
            Data->Params.push_back(var);
            Data->Params.push_back(input);
            return Data; 
        }


        ActionNode* PARSER::IfNode(ActionNode* input, ActionNode* body){
            ActionNode* Data = new ActionNode(); 
            Data->Action = If; 
            Data->Params.push_back(input);   
            Data->Params.push_back(body);   
            return Data; 
        }

        ActionNode* PARSER::IfElseNode(ActionNode* input, ActionNode* body, ActionNode* Else){
            ActionNode* Data = new ActionNode(); 
            Data->Action = If; 
            Data->Params.push_back(input);   
            Data->Params.push_back(body);   
            Data->Params.push_back(Else);   
            return Data; 
        }

        ActionNode* PARSER::WhileNode(ActionNode* input, ActionNode* body){
            ActionNode* Data = new ActionNode(); 
            Data->Action = While; 
            Data->Params.push_back(input);   
            Data->Params.push_back(body);   
            return Data; 
        }

        ActionNode* PARSER::CompareNode(ACTION act, ActionNode* val1, ActionNode* val2){
            ActionNode* cmp = new ActionNode();  
            cmp->Action = act; 
            cmp->Params.push_back(val1);
            cmp->Params.push_back(val2);
            return cmp; 
        }

        ActionNode* PARSER::ConditionNode(ActionNode* input){
            ActionNode* Data = new ActionNode(); 
            Data->Action = Cond; 
            Data->Params.push_back(input);   
            return Data; 
        }


        ActionNode* PARSER::FuncNode(std::string name){
            ActionNode* Data = new ActionNode(); 
            Data->Action = FunctionCall; 
            Data->VARIABLE_NAME = name; 
            return Data; 
        }


        ActionNode* PARSER::BuiltInFuncNode(int params){
            ActionNode* Data = new ActionNode(); 
            Data->Action = BuiltInFunction; 
            ActionNode* PARAMETERNODE = new ActionNode(); 
            PARAMETERNODE->Action = Parameters; 
            Data->Params.push_back(PARAMETERNODE); 
            for(int i=0; i<params; i++){
                PARAMETERNODE->Params.push_back(VariableNode(ARG_NAMES[i])); 
            }
            return Data; 
        }

        ActionNode* PARSER::Parse_Token_Number(int token_index){
            ActionNode* num1 = nullptr; 
            if(ProgramTokens[token_index].type == Number){ 
                if(parser_debug){ std::cout << "NUMBER FOUND \n"; }
                num1 = NumberNode(std::atoi(ProgramTokens[token_index].value.c_str())); 
            }
            if(ProgramTokens[token_index].type == Bool){ 
                if(parser_debug){ std::cout << "BOOLEAN FOUND \n"; }
                num1 = NumberNode(std::atoi(ProgramTokens[token_index].value.c_str())); 
            }
            return num1; 
        }


        ActionNode* PARSER::Parse_Token_Variable(int &token_index){
            ActionNode* var1 = nullptr; 

            if(Functions.find(ProgramTokens[token_index].value) != Functions.end()){
                if(parser_debug){ std::cout << "Function call in var found (PARSE TOEKN VAR): " << ProgramTokens[token_index].value << "\n"; }
                if(parser_debug){ std::cout << "next: " << ProgramTokens[token_index+1].value << "\n"; }
                if(parser_debug){ std::cout << "next: " << ProgramTokens[token_index+2].value << "\n"; }

                    var1 = FuncNode(ProgramTokens[token_index].value); 
                    
                    if(ProgramTokens[token_index+2].type != Syntax_Right_Paren){
                        if(parser_debug){ std::cout << "Function has parameters!! \n"; }
                        //function error starts here 
                        token_index+=2; 

                        int tmp_end = token_index; int depth = 0;
                        while(true){
                            if(ProgramTokens[tmp_end].type == Syntax_Left_Paren){ depth++; }
                            else if(ProgramTokens[tmp_end].type == Syntax_Right_Paren){ if(depth == 0){ break; } depth--; }
                            else if(ProgramTokens[tmp_end].type == Syntax_Comma && depth == 0){ break; } tmp_end++;
                        }
                        
                        //int tmp_end = token_index; 
                        //while(ProgramTokens[tmp_end].type != Syntax_Comma && ProgramTokens[tmp_end].type != Syntax_Right_Paren){ tmp_end++; } 
                        var1->Params.push_back(ParseExpression(token_index, tmp_end));
                        token_index = tmp_end; 
                        if(ProgramTokens[token_index].type == Syntax_Comma){
                            

                            while(ProgramTokens[token_index].type == Syntax_Comma){
                                token_index++; 
                                
                                int tmp_end = token_index; int depth = 0;
                                while(true){
                                    if(ProgramTokens[tmp_end].type == Syntax_Left_Paren){ depth++; }
                                    else if(ProgramTokens[tmp_end].type == Syntax_Right_Paren){ if(depth == 0){ break; } depth--; }
                                    else if(ProgramTokens[tmp_end].type == Syntax_Comma && depth == 0){ break; } tmp_end++;
                                }
                                //int tmp_end = token_index; 
                                //while(ProgramTokens[tmp_end].type != Syntax_Comma && ProgramTokens[tmp_end].type != Syntax_Right_Paren){ tmp_end++; } 
                                var1->Params.push_back(ParseExpression(token_index, tmp_end));
                                token_index = tmp_end; 
                            }   
                        }else{
                            token_index++; 
                        }
                    }else{
                        token_index+=2;
                    }
                    //token_index++; //for right paren


            }else if(ProgramTokens[token_index].type == Variable){
                if(parser_debug){ std::cout << "VAR FOUND \n"; }
                var1 = VariableNode(ProgramTokens[token_index].value); 
            }
            return var1; 
        }

        ActionNode* PARSER::Parse_Token_String(int token_index){
            ActionNode* str1 = nullptr; 
            if(ProgramTokens[token_index].type == String){
                if(parser_debug){ std::cout << "STRING FOUND \n"; }
                str1 = StringNode(ProgramTokens[token_index].value); 
            }
            return str1; 
        }


        int PARSER::FindNewLine(int index){
            int max_index = ProgramTokens.size(); 
            while(index < max_index){
                if(ProgramTokens[index].type == Syntax_NewLine){ break; }else{ index++; }
            }
            return index; 
        }



        int PARSER::FindRightParen_FromBack(int start_index, int end_index){ 
            int BraceDepth = 1; 
            int End_body_index = start_index+1; 
            //if you see ( add +1
            //if you see ) subtract -1   -   but if BraceDepth==1 when you find it then thats the end_point 
            while(true){
                if(ProgramTokens[End_body_index].type == Syntax_Left_Paren){
                    BraceDepth++; 
                }
                if(ProgramTokens[End_body_index].type == Syntax_Right_Paren){
                    if(BraceDepth == 1){
                        break; 
                    }
                    BraceDepth--; 
                }
                End_body_index++; 
            }
            

            return End_body_index; 
        }


        ActionNode* PARSER::ParsePrimaryInput(int &token_index, int end_index){
            ActionNode* val = nullptr; 

            if(ProgramTokens[token_index].type == Syntax_Left_Paren){
                int end = FindRightParen_FromBack(token_index, end_index); 
                //std::cout << "CHECK: " << ProgramTokens[token_index+1].value << "  " << ProgramTokens[token_index+1].type << "\n"; 
                val = ParseExpression(token_index+1, end-1); 
                token_index = end+1; 
            }

            if(val != nullptr){ return val; }

            val = Parse_Token_Number(token_index); 
            if(val != nullptr){ token_index++; return val; }
            val = Parse_Token_Variable(token_index); 
            if(val != nullptr){ token_index++; return val; }
            val = Parse_Token_String(token_index); 
            if(val != nullptr){ token_index++; return val; }else{
                PERR = {1, "Expression Error", "Number or Identifier or String", token_index, ProgramTokens[token_index].value}; 
                return nullptr; 
            }
        }


        ACTION PARSER::ParseBinaryOperator(int &token_index){
            ACTION exp_action = None; 
                if(ProgramTokens[token_index].type == Add){ exp_action = Add; token_index++; }
            else if(ProgramTokens[token_index].type == Sub){ exp_action = Sub; token_index++; }
            else if(ProgramTokens[token_index].type == Mul){ exp_action = Mul; token_index++; }
            else if(ProgramTokens[token_index].type == Div){ exp_action = Div; token_index++; }
            else if(ProgramTokens[token_index].type == Mod){ exp_action = Mod; token_index++; }
            else if(ProgramTokens[token_index].type == Logic_And){ exp_action = Logic_And; token_index++; }
            else if(ProgramTokens[token_index].type == Logic_Or){ exp_action = Logic_Or; token_index++; }
            else if(ProgramTokens[token_index].type == Cmp_NotEqual){ exp_action = Cmp_NotEqual; token_index++; }
            else if(ProgramTokens[token_index].type == Cmp_Equal){ exp_action = Cmp_Equal; token_index++; }
            else if(ProgramTokens[token_index].type == Cmp_GE){ exp_action = Cmp_GE; token_index++; }
            else if(ProgramTokens[token_index].type == Cmp_LE){ exp_action = Cmp_LE; token_index++; }
            else if(ProgramTokens[token_index].type == Cmp_GT){ exp_action = Cmp_GT; token_index++; }
            else if(ProgramTokens[token_index].type == Cmp_LT){ exp_action = Cmp_LT; token_index++; }
            else{ PERR = {1, "Expression Error", "Expected Operator", token_index, ProgramTokens[token_index].value}; }
            return exp_action; 
        }


        ActionNode* PARSER::ParseExpression(int start_index, int end_index){
            ActionNode* num1 = nullptr;
            ActionNode* num2 = nullptr;
            ActionNode* exp1 = nullptr; 
            ACTION exp_action = None; 

            num1 = ParsePrimaryInput(start_index, end_index); 

            //std::cout << "START: " << start_index << "   END: " << end_index << "\n"; 
            
            if(start_index >= end_index){
                return num1; 
            }

            exp_action = ParseBinaryOperator(start_index); 
            if(exp_action == None){ return nullptr; }


            num2 = ParsePrimaryInput(start_index, end_index); 

                if(exp_action == Add){ exp1 = NumberAdditionNode(num1, num2); exp_action = None; }
            else if(exp_action == Sub){ exp1 = NumberSubtractionNode(num1, num2); exp_action = None; }
            else if(exp_action == Mul){ exp1 = NumberMultiplyNode(num1, num2); exp_action = None; }
            else if(exp_action == Div){ exp1 = NumberDivisionNode(num1, num2); exp_action = None; }
            else if(exp_action == Mod){ exp1 = NumberModuloNode(num1, num2); exp_action = None; }
            else if(exp_action == Logic_And){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
            else if(exp_action == Logic_Or){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
            else if(exp_action == Cmp_NotEqual){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
            else if(exp_action == Cmp_Equal){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
            else if(exp_action == Cmp_GE){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
            else if(exp_action == Cmp_LE){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
            else if(exp_action == Cmp_GT){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
            else if(exp_action == Cmp_LT){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }


            while(start_index < end_index){
                exp_action = ParseBinaryOperator(start_index); 
                if(exp_action == None){ return nullptr; }

                num1 = exp1; 

                num2 = ParsePrimaryInput(start_index, end_index); 

                    if(exp_action == Add){ exp1 = NumberAdditionNode(num1, num2); exp_action = None; }
                else if(exp_action == Sub){ exp1 = NumberSubtractionNode(num1, num2); exp_action = None; }
                else if(exp_action == Mul){ exp1 = NumberMultiplyNode(num1, num2); exp_action = None; }
                else if(exp_action == Div){ exp1 = NumberDivisionNode(num1, num2); exp_action = None; }
                else if(exp_action == Mod){ exp1 = NumberModuloNode(num1, num2); exp_action = None; }
                else if(exp_action == Logic_And){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
                else if(exp_action == Logic_Or){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
                else if(exp_action == Cmp_NotEqual){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
                else if(exp_action == Cmp_Equal){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
                else if(exp_action == Cmp_GE){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
                else if(exp_action == Cmp_LE){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
                else if(exp_action == Cmp_GT){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
                else if(exp_action == Cmp_LT){ exp1 = CompareNode(exp_action, num1, num2); exp_action = None; }
            }

            return exp1; 
        }



        void PARSER::ParseLetStatement(ActionNode* &data, int &token_index){
            int end_index = FindNewLine(token_index); 
            ActionNode* var1 = Parse_Token_Variable(token_index); token_index++; 
            ActionNode* exp1; 

            if(parser_debug){ std::cout << "          before: [" << token_index << "] -  type: " << ProgramTokens[token_index].type << "  -  value: " << ProgramTokens[token_index].value << "\n"; }
            
            if(ProgramTokens[token_index].type == Equal){ 
                token_index++; 
                exp1 = ParseExpression(token_index, end_index); 
            }else{
                PERR = {1, "No def", "variable must be set", token_index, ProgramTokens[token_index].value}; return; 
            }

            data = AssignmentNode(var1, exp1); 
            token_index = end_index + 1; 

            if(parser_debug){ std::cout << "next: [" << token_index << "] -  type: " << ProgramTokens[token_index].type << "  -  value: " << ProgramTokens[token_index].value << "\n"; }
        }



        void PARSER::ParsePrintStatement(ActionNode* &data, int &token_index){
            int end_index = FindNewLine(token_index) - 1; //for right_paren
            token_index++; //for left_paren

            ActionNode* exp1 = ParseExpression(token_index, end_index); 

            data = PrintNode(exp1); 
            token_index = end_index + 2; 

            if(parser_debug){ std::cout << "next: [" << token_index << "] -  type: " << ProgramTokens[token_index].type << "  -  value: " << ProgramTokens[token_index].value << "\n"; }
        }

        void PARSER::ParseWhileStatement(ActionNode* &data, int &token_index){
            int end_index = token_index; 
            while(true){
                if(ProgramTokens[end_index].type == Syntax_Left_Brace){ break; }else{ end_index++; }
            }

            token_index++; 

            for(int i=token_index; i<end_index-1; i++){
                if(parser_debug){ std::cout << "type: " << ProgramTokens[i].type << "  -  value: " << ProgramTokens[i].value << "\n"; }
            }

            ActionNode* cond = ConditionNode(ParseExpression(token_index-1, end_index)); 
            
            token_index = end_index + 2; 


            int BraceDepth = 1; 
            int End_body_index = token_index; 
            //if you see { add +1
            //if you see } subtract -1   -   but if BraceDepth==1 when you find it then thats the end_point 
            while(true){
                if(ProgramTokens[End_body_index].type == Syntax_Left_Brace){
                    BraceDepth++; 
                }
                if(ProgramTokens[End_body_index].type == Syntax_Right_Brace){
                    if(BraceDepth == 1){
                        break; 
                    }
                    BraceDepth--; 
                }
                End_body_index++; 
            }

            for(int i=token_index; i<End_body_index; i++){
                if(parser_debug){ std::cout << "BODY  -  type: " << ProgramTokens[i].type << "  -  value: " << ProgramTokens[i].value << "\n"; }
            }

            ActionNode* body = new ActionNode(); 
            body->Action = Body; 

            while(token_index < End_body_index-1){
                ActionNode* bodyNode; 
                ParseInnerStatement(bodyNode, token_index); 
                if(bodyNode != nullptr){ body->Params.push_back(bodyNode); }
            }

            data = WhileNode(cond, body); 

            token_index = End_body_index + 2; 
            if(parser_debug){ std::cout << "next: [" << token_index << "] -  type: " << ProgramTokens[token_index].type << "  -  value: " << ProgramTokens[token_index].value << "\n"; }
        }

        void PARSER::ParseIfStatement(ActionNode* &data, int &token_index){
            int end_index = token_index; 
            while(true){
                if(ProgramTokens[end_index].type == Syntax_Left_Brace){ break; }else{ end_index++; }
            }

            token_index++; 

            for(int i=token_index; i<end_index-1; i++){
                if(parser_debug){ std::cout << "type: " << ProgramTokens[i].type << "  -  value: " << ProgramTokens[i].value << "\n"; }
            }

            ActionNode* cond = ConditionNode(ParseExpression(token_index-1, end_index)); 
            
            token_index = end_index + 2; 


            int BraceDepth = 1; 
            int End_body_index = token_index; 
            //if you see { add +1
            //if you see } subtract -1   -   but if BraceDepth==1 when you find it then thats the end_point 
            while(true){
                if(ProgramTokens[End_body_index].type == Syntax_Left_Brace){
                    BraceDepth++; 
                }
                if(ProgramTokens[End_body_index].type == Syntax_Right_Brace){
                    if(BraceDepth == 1){
                        break; 
                    }
                    BraceDepth--; 
                }
                End_body_index++; 
            }

            for(int i=token_index; i<End_body_index; i++){
                if(parser_debug){ std::cout << "BODY  -  type: " << ProgramTokens[i].type << "  -  value: " << ProgramTokens[i].value << "\n"; }
            }

            ActionNode* body = new ActionNode(); 
            body->Action = Body; 

            while(token_index < End_body_index-1){
                ActionNode* bodyNode; 
                ParseInnerStatement(bodyNode, token_index); 
                if(bodyNode != nullptr){ body->Params.push_back(bodyNode); }
            }

            token_index = End_body_index + 2; 
            

            if(ProgramTokens[token_index-1].type == Else){
                if(parser_debug){ std::cout <<  "ELSE FOUND " << "\n"; }

                token_index+=2; 
                End_body_index = token_index; 
                
                while(true){
                    if(ProgramTokens[End_body_index].type == Syntax_Left_Brace){
                        BraceDepth++; 
                    }
                    if(ProgramTokens[End_body_index].type == Syntax_Right_Brace){
                        if(BraceDepth == 1){
                            break; 
                        }
                        BraceDepth--; 
                    }
                    End_body_index++; 
                }

                for(int i=token_index; i<End_body_index; i++){
                    if(parser_debug){ std::cout << "ELSE  -  type: " << ProgramTokens[i].type << "  -  value: " << ProgramTokens[i].value << "\n"; }
                }

                ActionNode* ELSE = new ActionNode(); 
                ELSE->Action = Else; 

                while(token_index < End_body_index-1){
                    ActionNode* elseNode; 
                    ParseInnerStatement(elseNode, token_index); 
                    if(elseNode != nullptr){ ELSE->Params.push_back(elseNode); }
                }

                token_index = End_body_index + 2; 


                data = IfElseNode(cond, body, ELSE); 
                if(parser_debug){ std::cout << "after if-else end brace - next: [" << token_index << "] -  type: " << ProgramTokens[token_index].type << "  -  value: " << ProgramTokens[token_index].value << "\n"; }
            }else{
                data = IfNode(cond, body); 
                if(parser_debug){ std::cout << "after if end brace - next: [" << token_index << "] -  type: " << ProgramTokens[token_index].type << "  -  value: " << ProgramTokens[token_index].value << "\n"; }
            }
        }




        void PARSER::ParseFunctionStatement(int &token_index){ 
            if(parser_debug){ std::cout << "Function Definiton Found! \n"; }
            std::string func_name = ProgramTokens[token_index].value; 
            if(parser_debug){ std::cout << "Function Name: " << func_name << " \n"; }
            Functions[func_name] = nullptr;     

            ActionNode* PARAMETERNODE = new ActionNode(); 
            PARAMETERNODE->Action = Parameters; 

            ActionNode* BODYNODE = new ActionNode(); 
            BODYNODE->Action = Body; 

            ActionNode* FUNCTIONNODE = new ActionNode(); 
            FUNCTIONNODE->Action = Function; 
            FUNCTIONNODE->Params.push_back(PARAMETERNODE); 
            FUNCTIONNODE->Params.push_back(BODYNODE); 

            token_index+=2; 

            if(ProgramTokens[token_index].type != Syntax_Right_Paren){ 
                ActionNode* PARANODE = new ActionNode(); 
                PARANODE->Action = Variable; 
                PARANODE->VARIABLE_NAME = ProgramTokens[token_index++].value; 
                PARAMETERNODE->Params.push_back(PARANODE);

                while(ProgramTokens[token_index].type == Syntax_Comma){
                    token_index++; 
                    ActionNode* PARANODE = new ActionNode(); 
                    PARANODE->Action = Variable; 
                    PARANODE->VARIABLE_NAME = ProgramTokens[token_index++].value; 
                    PARAMETERNODE->Params.push_back(PARANODE);
                }   
            }
            token_index+=2;    
            

            int End_body_index = token_index; 
            int BraceDepth = 1; 
                
                while(true){
                    if(ProgramTokens[End_body_index].type == Syntax_Left_Brace){ BraceDepth++; }
                    if(ProgramTokens[End_body_index].type == Syntax_Right_Brace){ if(BraceDepth == 1){ break; } BraceDepth--; }
                    End_body_index++; 
                }
            token_index++; 

            for(int i=token_index; i<End_body_index; i++){
                if(parser_debug){ std::cout << "FUNCTION BODY  -  -  -  -  type: " << ProgramTokens[i].type << "  -  value: " << ProgramTokens[i].value << "\n"; }
            }


            while(token_index < End_body_index-1){
                ActionNode* Node; 
                ParseInnerStatement(Node, token_index); 
                if(Node != nullptr){ BODYNODE->Params.push_back(Node); }
            }


            token_index = End_body_index + 2; 

            if(parser_debug){ std::cout << "next - type: " << ProgramTokens[token_index].type << "  -  value: " << ProgramTokens[token_index].value << "\n"; }

            Functions[func_name] = FUNCTIONNODE; 
        }


        void PARSER::parseReturnStatement(ActionNode* &data, int &token_index){
            ActionNode* Data = new ActionNode(); 
            Data->Action = Return; 
            if(ProgramTokens[token_index+1].type != Syntax_NewLine){

                int end_index = FindNewLine(token_index); //for right_paren
                token_index++; //for left_paren
                if(parser_debug){ std::cout << "                                                  " << token_index << ", " << end_index << "\n"; 
                std::cout << "before - type: " << ProgramTokens[token_index].type << "  -  value: " << ProgramTokens[token_index].value << "\n"; 
                std::cout << "next - type: " << ProgramTokens[end_index].type << "  -  value: " << ProgramTokens[end_index].value << "\n"; }

                ActionNode* exp1 = ParseExpression(token_index, end_index); 
                Data->Params.push_back(exp1); 
                token_index = end_index; 
            }else{
                token_index++; 
                token_index++;         
            }

            data = Data; 

        }


        void PARSER::parseContinueStatement(ActionNode* &data, int &token_index){
            ActionNode* Data = new ActionNode(); 
            Data->Action = Continue; 
            data = Data; 
            token_index+=2; 
        }

        void PARSER::parseBreakStatement(ActionNode* &data, int &token_index){
            ActionNode* Data = new ActionNode(); 
            Data->Action = Break; 
            data = Data; 
            token_index+=2; 
        }


        
        void PARSER::ParseInnerStatement(ActionNode* &Data, int &token_index){

            if(ProgramTokens[token_index].type == Assignment){
                token_index++; 
                ParseLetStatement(Data, token_index); 
            }else if(ProgramTokens[token_index].type == Print){
                token_index++; 
                ParsePrintStatement(Data, token_index); 
            }else if(ProgramTokens[token_index].type == If){
                token_index++; 
                ParseIfStatement(Data, token_index); 
            }else if(ProgramTokens[token_index].type == While){
                token_index++; 
                ParseWhileStatement(Data, token_index); 
            }else if(ProgramTokens[token_index].type == Continue){
                parseContinueStatement(Data, token_index); 
            }else if(ProgramTokens[token_index].type == Break){
                parseBreakStatement(Data, token_index); 


            }else if(ProgramTokens[token_index].type == Return){
                parseReturnStatement(Data, token_index); 


            }else if(ProgramTokens[token_index].type == Function){
                token_index++; 
                ParseFunctionStatement(token_index); 
                func_found=true; 
                Data=nullptr; 


            }else if(ProgramTokens[token_index].type == Variable){
                if(Functions.find(ProgramTokens[token_index].value) != Functions.end()){
                    if(parser_debug){ std::cout << "Function call in var found: " << ProgramTokens[token_index].value << "\n"; }
                    Data = FuncNode(ProgramTokens[token_index].value); 

                    token_index++; //for left paren
                    
                    if(ProgramTokens[token_index+1].type != Syntax_Right_Paren){
                        if(parser_debug){ std::cout << "Function has parameters! \n"; }
                        token_index++; 
                        if(parser_debug){ std::cout << "$$$$ " << ProgramTokens[token_index].type << " $$$$ \n"; }
                        int tmp_end = token_index; 
                        while(ProgramTokens[tmp_end].type != Syntax_Comma && ProgramTokens[tmp_end].type != Syntax_Right_Paren){ tmp_end++; } 
                        Data->Params.push_back(ParseExpression(token_index, tmp_end));
                        if(ProgramTokens[token_index+1].type == Syntax_Comma){
                            token_index++; 

                            while(ProgramTokens[token_index].type == Syntax_Comma){
                                token_index++; 
                                int tmp_end = token_index; 
                                while(ProgramTokens[tmp_end].type != Syntax_Comma && ProgramTokens[tmp_end].type != Syntax_Right_Paren){ tmp_end++; } 
                                Data->Params.push_back(ParseExpression(token_index, tmp_end));
                            }   
                        }
                    }
                    token_index++; //for right paren
                }else{
                    ParseLetStatement(Data, token_index); 
                }
                token_index+=2; 

            }else if(ProgramTokens[token_index].type == EndProgram){
                Data=nullptr; 
            }else{
                PERR = {1, "Unknown Statement", "valid token", token_index, ProgramTokens[token_index].value};
                return; 
            }

        }



        void PARSER::ParseStatement(MainNodeAST* &current, int &token_index){
            ParseInnerStatement(current->Data, token_index); 
            if(func_found){ return; }
            if(current->Data == nullptr){ current = nullptr; }
        }




        void PARSER::Build_Built_ins(){
            Functions["_sqrt_"] = BuiltInFuncNode(1); 
            Functions["_len_"] = BuiltInFuncNode(1); 
            Functions["_abs_"] = BuiltInFuncNode(1); 
            Functions["_str_input_"] = BuiltInFuncNode(0); 
            Functions["_num_input_"] = BuiltInFuncNode(0); 
            Functions["_min_"] = BuiltInFuncNode(2); 
            Functions["_max_"] = BuiltInFuncNode(2); 
            Functions["_pow_"] = BuiltInFuncNode(2); 
            Functions["_rand_"] = BuiltInFuncNode(2); 
            Functions["_upper_"] = BuiltInFuncNode(1); 
            Functions["_lower_"] = BuiltInFuncNode(1); 
            Functions["_substr_"] = BuiltInFuncNode(3); 
            Functions["_find_"] = BuiltInFuncNode(2); 
            Functions["_to_string_"] = BuiltInFuncNode(1); 
            Functions["_to_number_"] = BuiltInFuncNode(1); 
        }



        void PARSER::ParseProgram(){
            if(!LEXER_ERROR_DETECTED){ 
                Build_Built_ins(); 
                
                root = new MainNodeAST();  
                MainNodeAST* current = root;
                
                token_to_ast_index = 0; 

                int program_tokens = ProgramTokens.size(); 

                while(token_to_ast_index < program_tokens && !PERR.PARSER_ERROR_DETECTED){
                    ParseStatement(current, token_to_ast_index); 

                    if(func_found){ func_found = false; continue; }

                    if(ProgramTokens[token_to_ast_index].type != EndProgram){
                        current->next = new MainNodeAST(); current = current->next; 
                    }else{
                        token_to_ast_index = program_tokens; 
                    }
                }

                if(PERR.PARSER_ERROR_DETECTED){
                    std::cerr << "\n\n ******** Parser has detected a fault! ******** \n |\n"; 
                    std::cerr << " | Error: " << PERR.PARSER_ERROR_DEFINITION << "\n"; 
                    std::cerr << " | Expected: " << PERR.PARSER_ERROR_EXPECTED << "\n"; 
                    std::cerr << " | Token #: " << PERR.PARSER_ERROR_TOKEN << "\n"; 
                    std::cerr << " | Token: " << PERR.PARSER_ERROR_FOUND << "\n | \n"; 
                    std::cerr << " ********************************************** \n\n"; 
                }
            }
        }

