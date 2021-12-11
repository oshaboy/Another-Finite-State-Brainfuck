#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char * argv[]){
    char * code=malloc(1024);
    {
    int code_size=1024;
    int counter=0;
    int num;
    FILE * f=fopen(argv[1],"rb");
    num=fread(code,1,1024,f);
    while(num==1024){
        code_size+=1024;
        counter+=1024;
        code=realloc(code, code_size);
        num=fread(code+code_size-1024,1,1024,f);

    }
    counter+=num;
    for (int i=code_size; i>=counter; i--){code[i]='\0';}
    }
    {
    int stack_size=1;
    char ** stack=malloc(sizeof(char*));
    char * IP=code;
    uint16_t accumulator;
    bool isprevplus=false;
    int skip_count=0;
    while(*IP){

        switch(*IP){
            case '+':
                if (!skip_count){
                    if (!isprevplus) accumulator=0;
                    accumulator++;
                    isprevplus=true;
                }
                break;
            case '-':
                if(!skip_count&&accumulator) accumulator--;
                isprevplus=false;
                break;
            case ',':   

                if (!skip_count&&!fread(&accumulator,1,1,stdin)) accumulator=0;
                isprevplus=false;
                break;
            case '.':
                if(!skip_count){
                    fwrite(&accumulator,1,1,stdout);
                }
                isprevplus=false;
                break;
            case '[':
                if (skip_count||!accumulator) skip_count++;
                else if (accumulator){
                    stack[stack_size-1]=IP;
                    stack_size++;
                    stack=realloc(stack, stack_size*sizeof(char*));

                }
                isprevplus=false;
                break;
            case ']':
                if (skip_count)skip_count--;
                else if(accumulator){
                    if(!stack_size){fwrite("Unbalanced Brackets\n",20,1,stdout);}
                    IP=stack[stack_size-1-1];
                    stack=realloc(stack, stack_size*sizeof(char*));
                    
                } else{
                    stack_size--;
                    stack=realloc(stack, stack_size*sizeof(char*));
                }
                isprevplus=false;
                break;
        }
        IP++;
    }
    free(code); free(stack);
    }
    return 0;
}