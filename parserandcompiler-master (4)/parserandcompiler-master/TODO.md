TO-DO LIST

- Implement if statements so it registers in the compiler

- add a complete for loop with for( int i = 0; i < x ; i++){
    - use regex to get all letters [a / z] using map 
}
- implement class statements



//if true bool print out string or number
//ex: int x = 1; if{x==1}print("hello") or print(2);


-- need to have boolean value register as a statement ex: 1 < 0;
-- need the if statement to print out the given value ex: print(num);
-- if true execute; if false do nothing 

write down all boolean values and register them somewhere in the code

change the byte code instructions to support boolean values

if statement is already registered with the parser
- focus on the GenerateCodeForFunction and apply boolean

-make suyre boolean knows its boolean 



if statements - check the codition if it is true or false 



LOG:: generate compare instruction COMP_INT_LT, JUMP Instruction JUMP_BY_IF_ZERO, No counter variable 

1 < 2 = true , should be false

7 < 2 = should be true , but false , does not print anything 


compare lessthan instruction 

compare greater than instructioib






int main(){
    int x = 12;
    
    if( 13 <  x){
                                ------------------> result true print out number 100
        printNum(100);
    }
   
;} 


- bytecode loops that are made by hand


- import the insrtuction code to if statment bytecode 

- make a bytecode if statement 







lessathan sighed prints out result > does not












int main() {
    int x = 7;
    if(x > 10) {
        
        printNum(30); ---------------------------------> works : 7 > 10 is false result 0;
    };

   
}

- look at video for if statements and loops what they dont neeed instruction part 4



notes: Load_INT , PUSH_INT, COMP_INT_LT are 3 lines of code together for operating check


1. print strings
- setup print statement 
- store as a char maybe
-utf8_str
-  compiledCode.push_back(Instruction{bytecodeinterpreter::PRINT_STRING, 0, 0}); prints out String Printed and the number 0.
- not int16_t because it registers with registers,stack,back()
- could be registers.stack.back()


2. add all string commands 
- change registers.stack.back();
- add varaible declare for "" to resiter as a string in the function call
- resiters is not the answer it is stack that needs to be changed

- printNum("LOG: TEST"); prints out 3 error maybe
- print is defined and hello world is not
- define hello world to register for the error

3. -1 breaks teh compilier
 - "" breaks the compilrer and prints out 6144

important:::----------------------------------------
 - maybe not numbers its the result varabile statemnt 
- make string commands in instructions and implement them in print statement to test if it passes the test.