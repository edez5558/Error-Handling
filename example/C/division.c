#include <stdio.h>

#define NO_ERROR 0
#define DIVIDER_ZERO 1 

typedef unsigned int ERROR_CODE;

const char* list_of_error[] = {
    [NO_ERROR] = "No error",
    [DIVIDER_ZERO] = "The divider is zero"
};

int n_err = sizeof(list_of_error)/sizeof(char*);

const char* get_string_error(ERROR_CODE err_code){
    if(err_code >= 0 && err_code < n_err)
        return list_of_error[err_code];
    else
        return "Not found error code.";
}

int divide(int a,int b,ERROR_CODE* err_code){
    if(b == 0){
        if(err_code) *err_code= DIVIDER_ZERO;

        return -1;
    }

    if(err_code) *err_code = NO_ERROR;

    return a/b;
}

int main(int argc,char* argv[]){
    ERROR_CODE code;

    int result = divide(5,0,&code);

    if(code != NO_ERROR){
        fprintf(stderr,"Error: %s\n",get_string_error(code));
        return -1;
    }

    return 0;
}