#include "Interpreter.h"


///////////////////////////////////////////////////////////////////////////////////////////
/////// Built-in functions ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


Values builtin_sqrt(){
    int value = std::sqrt(Prog_Variables.top()["_arg1_"].Number); 
    return {Number, value, false, "", ""};
}

Values builtin_len(){ 
    int value = Prog_Variables.top()["_arg1_"].String.length(); 
    return {Number, value, false, "", ""};
}

Values builtin_abs(){
    int value = std::abs(Prog_Variables.top()["_arg1_"].Number); 
    return {Number, value, false, "", ""};
}

Values builtin_input_int(){
    int value; 
    std::cin >> value;
    return {Number, value, false, "", ""};
}

Values builtin_input_string(){
    std::string value; 
    std::cin >> value;
    return {String, 0, false, value, ""};
}

Values builtin_min(){
    int a = Prog_Variables.top()["_arg1_"].Number; 
    int b = Prog_Variables.top()["_arg2_"].Number; 
    int value = std::min(a, b); 
    return {Number, value, false, "", ""};
}

Values builtin_max(){
    int a = Prog_Variables.top()["_arg1_"].Number; 
    int b = Prog_Variables.top()["_arg2_"].Number; 
    int value = std::max(a, b); 
    return {Number, value, false, "", ""};
}

Values builtin_pow(){
    int a = Prog_Variables.top()["_arg1_"].Number; 
    int b = Prog_Variables.top()["_arg2_"].Number; 
    int value = static_cast<int>(std::pow(a, b));
    return {Number, value, false, "", ""};
}

Values builtin_rand(){
    int a = Prog_Variables.top()["_arg1_"].Number; 
    int b = Prog_Variables.top()["_arg2_"].Number; 
    if(a > b)
        std::swap(a,b);
    std::uniform_int_distribution<int> distrib(a, b);
    int value = distrib(gen); 
    return {Number, value, false, "", ""};
}

Values builtin_upper(){
    std::string str = Prog_Variables.top()["_arg1_"].String; 
    for (char &c : str) {
        c = std::toupper(static_cast<unsigned char>(c));
    }
    return {String, 0, false, str, ""};
}

Values builtin_lower(){
    std::string str = Prog_Variables.top()["_arg1_"].String; 
    for (char &c : str) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return {String, 0, false, str, ""};
}

Values builtin_substr(){
    std::string str = Prog_Variables.top()["_arg1_"].String; 
    int start = Prog_Variables.top()["_arg2_"].Number; 
    int length = Prog_Variables.top()["_arg3_"].Number; 
    int check = start + length; 

    if(start >= str.length())
        return {String,0,false,"",""};

    if(start + length > str.length())
        length = str.length() - start;

    std::string value = str.substr(start, length); 
    return {String, 0, false, value, ""};
}

Values builtin_find(){ 
    int value = Prog_Variables.top()["_arg1_"].String.find(Prog_Variables.top()["_arg2_"].String); 
    if(value == std::string::npos){
        return {Number, -1, false, "", ""};
    }
    return {Number, value, false, "", ""};
}

Values builtin_to_string(){
    int num = Prog_Variables.top()["_arg1_"].Number; 
    std::string value = std::to_string(num); 
    return {String, 0, false, value, ""};
}

Values builtin_to_number(){ 
    std::string str = Prog_Variables.top()["_arg1_"].String;
    try{
        int value = std::stoi(str);
        return {Number, value, false, "", ""};
    }
    catch(...){
        RunErr = {
            true,
            "Invalid numeric conversion",
            "A numeric string",
            0,
            str
        };
        return {};
    }
}


///////////////////////////////////////////////////////////////////////////////////////////
/////// Built-in functions ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

void VirtualMachine::help_display_ast(int depth){
    std::cout << "│"; 
    for(int i=0; i<depth; i++){
        if(i>0){ std::cout << "│"; }
        std::cout << "   "; 
    }
}

void VirtualMachine::RecurseDisplayAST(ActionNode* root, int &depth, int param){
        int action_num = root->Action; 
        int params_size = root->Params.size(); 
        int varname_size = root->VARIABLE_NAME.length(); 
        int string_size = root->STRING.length(); 

        help_display_ast(depth); std::cout << "├──" << ACTIONS_STR[action_num] << "\n";  

        if(varname_size != 0){
            help_display_ast(depth+1); std::cout << "├──" << root->VARIABLE_NAME << "\n";  
        }else if(string_size != 0){
            help_display_ast(depth+1); std::cout << "├──" << root->STRING << "\n";  
        }else if(params_size == 0){
            help_display_ast(depth+1); std::cout << "├──" << root->NUMBER << "\n";  
        }
        if(params_size != 0){
            depth++; 
            for(int r=0; r<params_size; r++){
                RecurseDisplayAST(root->Params[r], depth, r); 
            }
            depth--; 
        }
}

void VirtualMachine::DisplayAST(){
    if(!PERR.PARSER_ERROR_DETECTED && !LEXER_ERROR_DETECTED){
        int depth = 0; 
        MainNodeAST* current = root; 
        std::cout << "\n###############################################\n\n"; 
        std::cout << "Program\n│\n"; 
        
        while(current != nullptr){
            int action_num = current->Data->Action; 
            int params_size = current->Data->Params.size(); 
            int varname_size = current->Data->VARIABLE_NAME.length(); 
            int string_size = current->Data->STRING.length(); 

            help_display_ast(depth); std::cout << "───" << ACTIONS_STR[action_num] << "\n";  

            if(varname_size != 0){
                help_display_ast(depth+1); std::cout << "├──" << current->Data->VARIABLE_NAME << "\n";  
            }else if(string_size != 0){
                help_display_ast(depth+1); std::cout << "├──" << current->Data->STRING << "\n";  
            }else if(params_size == 0){
                help_display_ast(depth+1); std::cout << "├──" << current->Data->NUMBER << "\n";  
            }      
            if(params_size != 0){
                depth++; 
                for(int r=0; r<params_size; r++){ RecurseDisplayAST(current->Data->Params[r], depth, r); }
                depth--; 
            }      
            std::cout << "|\n"; 
            current = current -> next; 
        }
        std::cout << "END_PROG\n\n";

        for (const auto& [key, value] : Functions) {
            std::cout << "'" << key << "'\n";
            depth++; 
            RecurseDisplayAST(value, depth, 0);
            depth--; 
        }

        std::cout << "\n\n###############################################\n"; 
    }
}

Values VirtualMachine::EvaluateExpression(ActionNode* input){
    Values num_collector = {None, 0, false, "", ""}; 
    if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }

    switch(input->Action){
        case Number: num_collector = {Number, input->NUMBER, false, "", ""}; break; 
        case String: num_collector = {String, 0, false, input->STRING, ""}; break; 
        case Bool: num_collector = {Bool, 0, input->BOOL, "", ""}; break; 

        case Variable: { 
            if(Prog_Variables.top().find(input->VARIABLE_NAME) != Prog_Variables.top().end()){ 
                num_collector = Prog_Variables.top()[input->VARIABLE_NAME]; 
            }else{ 
                RunErr = {1, "Variable not found: "+input->VARIABLE_NAME, "Define variable", 0, ""}; return {None, 0, false, "", ""}; 
            } 
            break; 
        }
        
        case FunctionCall: ExecuteFunctionCall(input); num_collector = return_value; break; 

        case Add: { 
            Values val1 = EvaluateExpression(input->Params[0]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            Values val2 = EvaluateExpression(input->Params[1]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            if(val1.type == Number && val2.type == Number){ num_collector = {Number, val1.Number + val2.Number, false, "", ""}; }
            else if(val1.type == String && val2.type == String){ num_collector = {String, 0, false, val1.String + val2.String, ""}; }
            else{ RunErr = {1, "invalid values for + operation", "Change values", 0, ""}; }
            break; 
        }

        case Sub: { 
            Values val1 = EvaluateExpression(input->Params[0]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            Values val2 = EvaluateExpression(input->Params[1]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            if(val1.type == Number && val2.type == Number){ num_collector = {Number, val1.Number - val2.Number, false, "", ""}; }
            else{ RunErr = {1, "invalid values for - operation", "Change values", 0, ""}; }
            break; 
        }

        case Mul: { 
            Values val1 = EvaluateExpression(input->Params[0]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            Values val2 = EvaluateExpression(input->Params[1]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            if(val1.type == Number && val2.type == Number){ num_collector = {Number, val1.Number * val2.Number, false, "", ""}; }
            else{ RunErr = {1, "invalid values for * operation", "Change values", 0, ""}; }
            break; 
        }
        
        case Div: { 
            Values val1 = EvaluateExpression(input->Params[0]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            Values val2 = EvaluateExpression(input->Params[1]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            if(val1.type == Number && val2.type == Number){ 
                if(val2.Number == 0){ RunErr = {1, "Divide by zero", "Check calculations", 0, ""}; return {None, 0, false, "", ""}; }
                num_collector = {Number, val1.Number / val2.Number, false, "", ""}; 
            }else{ RunErr = {1, "invalid values for / operation", "Change values", 0, ""}; }
            break; 
        }

        case Mod: { 
            Values val1 = EvaluateExpression(input->Params[0]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            Values val2 = EvaluateExpression(input->Params[1]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            if(val1.type == Number && val2.type == Number){ 
                if(val2.Number == 0){ RunErr = {1, "Divide by zero", "Check calculations", 0, ""}; return {None, 0, false, "", ""}; }
                num_collector = {Number, val1.Number % val2.Number, false, "", ""}; 
            }else{ RunErr = {1, "Non-numeric values for modulo operation", "Change values to numerics", 0, ""}; }
            break; 
        }
        
        case Logic_And: { 
            Values val1 = EvaluateExpression(input->Params[0]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            Values val2 = EvaluateExpression(input->Params[1]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            if(val1.type == Number && val2.type == Number){ num_collector = {Bool, 0, bool(val1.Number && val2.Number), "", ""}; }
            else if(val1.type == Bool && val2.type == Bool){ num_collector = {Bool, 0, bool(val1.Boolean && val2.Boolean), "", ""}; }
            else{ RunErr = {1, "Non-numeric values for logical operation", "Change values to numerics", 0, ""}; }
            break; 
        }

        case Logic_Or: { 
            Values val1 = EvaluateExpression(input->Params[0]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            Values val2 = EvaluateExpression(input->Params[1]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            if(val1.type == Number && val2.type == Number){ num_collector = {Bool, 0, bool(val1.Number || val2.Number), "", ""}; }
            else if(val1.type == Bool && val2.type == Bool){ num_collector = {Bool, 0, bool(val1.Boolean || val2.Boolean), "", ""}; }
            else{ RunErr = {1, "Non-numeric values for logical operation", "Change values to numerics", 0, ""}; }
            break; 
        }

        case Cmp_NotEqual: { 
            Values val1 = EvaluateExpression(input->Params[0]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            Values val2 = EvaluateExpression(input->Params[1]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            if(val1.type == Number && val2.type == Number){ num_collector = {Bool, 0, bool(val1.Number != val2.Number), "", ""}; }
            else if(val1.type == String && val2.type == String){ num_collector = {Bool, 0, bool(val1.String != val2.String), "", ""}; }
            else if(val1.type == Bool && val2.type == Bool){ num_collector = {Bool, 0, bool(val1.Boolean != val2.Boolean), "", ""}; }
            else{ RunErr = {1, "invalid values for != operation", "Change values", 0, ""}; }
            break; 
        }

        case Cmp_Equal: { 
            Values val1 = EvaluateExpression(input->Params[0]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            Values val2 = EvaluateExpression(input->Params[1]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            if(val1.type == Number && val2.type == Number){ num_collector = {Bool, 0, bool(val1.Number == val2.Number), "", ""}; }
            else if(val1.type == String && val2.type == String){ num_collector = {Bool, 0, bool(val1.String == val2.String), "", ""}; }
            else if(val1.type == Bool && val2.type == Bool){ num_collector = {Bool, 0, bool(val1.Boolean == val2.Boolean), "", ""}; }
            else{ RunErr = {1, "invalid values for == operation", "Change values", 0, ""}; }
            break; 
        }

        case Cmp_GE: { 
            Values val1 = EvaluateExpression(input->Params[0]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            Values val2 = EvaluateExpression(input->Params[1]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            if(val1.type == Number && val2.type == Number){ num_collector = {Bool, 0, bool(val1.Number >= val2.Number), "", ""}; }
            else{ RunErr = {1, "Non-numeric values for comparison operation", "Change values to numerics", 0, ""}; }
            break; 
        }

        case Cmp_LE: { 
            Values val1 = EvaluateExpression(input->Params[0]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            Values val2 = EvaluateExpression(input->Params[1]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            if(val1.type == Number && val2.type == Number){ num_collector = {Bool, 0, bool(val1.Number <= val2.Number), "", ""}; }
            else{ RunErr = {1, "Non-numeric values for comparison operation", "Change values to numerics", 0, ""}; }
            break; 
        }

        case Cmp_GT: { 
            Values val1 = EvaluateExpression(input->Params[0]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            Values val2 = EvaluateExpression(input->Params[1]);  if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  
            if(val1.type == Number && val2.type == Number){ num_collector = {Bool, 0, bool(val1.Number > val2.Number), "", ""}; }
            else{ RunErr = {1, "Non-numeric values for comparison operation", "Change values to numerics", 0, ""}; }
            break; 
        }

        case Cmp_LT: { 
            Values val1 = EvaluateExpression(input->Params[0]); if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }  Values val2 = EvaluateExpression(input->Params[1]);
            if(val1.type == Number && val2.type == Number){ num_collector = {Bool, 0, bool(val1.Number < val2.Number), "", ""}; }
            else{ RunErr = {1, "Non-numeric values for comparison operation", "Change values to numerics", 0, ""}; }
            break; 
        }
    }
    if(RunErr.ERROR_DETECTED){ return {None, 0, false, "", ""}; }

    return num_collector; 
}



void VirtualMachine::assignment_interpreter(ActionNode* input){
    //Params[0] = name
    //Params[1] = value 
    std::string var_name = input->Params[0]->VARIABLE_NAME; 
    Values var_value = EvaluateExpression(input->Params[1]); 
    if(RunErr.ERROR_DETECTED){ return; }
    Prog_Variables.top()[var_name] = var_value;
}


void VirtualMachine::print_interpreter(ActionNode* input){
    int params_size = input->Params.size(); 
    if(params_size != 0){  
        for(int r=0; r<params_size; r++){ 
            Values val = EvaluateExpression(input->Params[r]); 
            switch(val.type){
                case Number: std::cout << val.Number << "\n"; break; 
                case String: std::cout << val.String << "\n"; break; 
                case Bool: std::cout << (val.Boolean ? "true" : "false") << "\n"; break; 
            }
            if(RunErr.ERROR_DETECTED){ return; }            
        }
    } 
}

void VirtualMachine::return_interpreter(ActionNode* input){
    return_value = {None, 0, false, "", ""}; 
    if(input->Params.size() != 0){
        return_value = EvaluateExpression(input->Params[0]); 
    }    
}

void VirtualMachine::if_interpreter(ActionNode* input){
    //Params[0] = condition - boolean output
    //Params[1] = body
    //Params[2] = possible else 
    if(EvaluateExpression(input->Params[0]->Params[0]).Boolean){ 
        if(RunErr.ERROR_DETECTED){ return; }
        for(int p=0; p<input->Params[1]->Params.size(); p++){
            ExecuteStatement(input->Params[1]->Params[p]); 
        }
    }else{
        if(input->Params.size() == 3 && input->Params[2] != nullptr && input->Params[2]->Action == Else){
            if(RunErr.ERROR_DETECTED){ return; }
            for(int p=0; p<input->Params[2]->Params.size(); p++){
            ExecuteStatement(input->Params[2]->Params[p]); 
            }
        }
    }
}

void VirtualMachine::while_interpreter(ActionNode* input){
    //Params[0] = condition - boolean output
    //Params[1] = body
    cont_while: 
    while(EvaluateExpression(input->Params[0]->Params[0]).Boolean){ 
        if(RunErr.ERROR_DETECTED){ return; }
        for(int p=0; p<input->Params[1]->Params.size(); p++){            
            ExecuteStatement(input->Params[1]->Params[p]); 
            if(break_called){ break_called=false; return; }
            if(continue_called){ continue_called=false; goto cont_while; }
        }
    }
}

void VirtualMachine::ExecuteFunctionCall(ActionNode* input){
    if(Functions.find(input->VARIABLE_NAME) != Functions.end()){
        Prog_Variables.push(Prog_Variables.top()); 
        ActionNode* functionParams = Functions[input->VARIABLE_NAME]->Params[0]; 
        for(int i=0; i<input->Params.size(); i++){ Prog_Variables.top()[functionParams->Params[i]->VARIABLE_NAME] = EvaluateExpression(input->Params[i]); }
        if(Functions[input->VARIABLE_NAME]->Action == BuiltInFunction){
            return_value = BuiltInTable[input->VARIABLE_NAME](); 
        }else{
            ActionNode* functionBlock = Functions[input->VARIABLE_NAME]->Params[1]; 
            for(int p=0; p<functionBlock->Params.size(); p++){ ExecuteStatement(functionBlock->Params[p]); if(return_called){ return_called = false; break; } }
        }
        Prog_Variables.pop(); if(RunErr.ERROR_DETECTED){ return; }
    }else{
        RunErr = {1, "Function not defined", "Define function before calling it", 0, ""}; return;
    }
}


void VirtualMachine::ExecuteStatement(ActionNode* input){
    ACTION action_num = input->Action; 
    switch(action_num){
        case Print:
            print_interpreter(input); 
            break; 
        case Assignment:
            assignment_interpreter(input);
            break; 
        case If:
            if_interpreter(input);
            break; 
        case While:
            while_interpreter(input);
            break; 
        case Break:
            break_called = true; 
            break; 
        case Continue:
            continue_called = true; 
            break; 
        case Return: 
            return_interpreter(input);
            return_called = true; 
            break; 
        case FunctionCall: 
            ExecuteFunctionCall(input); 
            break; 
        default: 
            RunErr = {1, "Unknown instruction in AST", "Check syntax", 0, ""}; 
            break; 
    }
    if(RunErr.ERROR_DETECTED){ return; }
}


void VirtualMachine::ExecuteProgramAST(){ 
    if(!PERR.PARSER_ERROR_DETECTED && !LEXER_ERROR_DETECTED){ 

        BuiltInTable["_sqrt_"] = builtin_sqrt; 
        BuiltInTable["_len_"] = builtin_len; 
        BuiltInTable["_abs_"] = builtin_abs; 
        BuiltInTable["_num_input_"] = builtin_input_int; 
        BuiltInTable["_str_input_"] = builtin_input_string; 
        BuiltInTable["_min_"] = builtin_min; 
        BuiltInTable["_max_"] = builtin_max; 
        BuiltInTable["_pow_"] = builtin_pow; 
        BuiltInTable["_rand_"] = builtin_rand; 
        BuiltInTable["_upper_"] = builtin_upper; 
        BuiltInTable["_lower_"] = builtin_lower; 
        BuiltInTable["_substr_"] = builtin_substr; 
        BuiltInTable["_find_"] = builtin_find; 
        BuiltInTable["_to_string_"] = builtin_to_string; 
        BuiltInTable["_to_number_"] = builtin_to_number; 

        Prog_Variables.push({}); 
        if(parser_debug){ std::cout << "\n\n";  }
        std::cout << "Running...\n\n"; 
        MainNodeAST* current = root; 
        while(current != nullptr){
            ExecuteStatement(current->Data); 
            if(RunErr.ERROR_DETECTED){ break; }
            current = current -> next; 
        }

        if(RunErr.ERROR_DETECTED){
            std::cout << "\nProgram exited with errors\n"; 
            std::cerr << "\n\n ******** Interpreter has detected a fault! ******** \n |\n"; 
            std::cerr << " | Error: " << RunErr.ERROR_DEFINITION << "\n"; 
            std::cerr << " | Expected: " << RunErr.ERROR_EXPECTED << "\n"; 
            std::cerr << " *************************************************** \n\n"; 
        }else{
            std::cout << "\nProgram exited\n\n";
        }

        if(var_debugging){
            std::cout << "Vars: \n"; 
            for (const auto& [key, value] : Prog_Variables.top()) {
                std::cout << "'" << key << "' => " << ACTIONS_STR[value.type] << " : ";
                switch(value.type){
                    case Number: std::cout << value.Number << "\n"; break; 
                    case Bool: std::cout << value.Boolean << "\n"; break; 
                    case String: std::cout << value.String << "\n"; break; 
                    case Variable: std::cout << value.Variable << "\n"; break; 
                }
            }
        }
        Prog_Variables.top().clear(); 
    }
}

void VirtualMachine::RunProgram(std::string name){
    LEXER lex; 
    lex.LoadProgram(name);
    lex.TokenizeProgram();  

    PARSER par; 
    par.ParseProgram(); 

    if(ast_debug){ DisplayAST(); } 
    ExecuteProgramAST(); 
}
