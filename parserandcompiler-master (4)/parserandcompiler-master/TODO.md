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