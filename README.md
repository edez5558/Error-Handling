# Error Handling

*Read this in other language: [English](README.md), [Spanish](README.es.md)*
## C
In C, there is no such thing as an error handler. Most functions return the error code and in other cases, a pointer is passed to modify it and notify the error.

``` c
#define TEST_ERROR 5

typedef int ERROR_CODE;

ERROR_CODE foo(int arguments,int* returned){
    if(returned != NULL){
        *returned = 0;
    }
    
    return TEST_ERROR;
}

int bar(int arguments,ERROR_CODE* err_code){
    if(err_code != NULL){
        *err_code = TEST_ERROR;
    }
    
    return 0;
}

```

Let's see a simple example of how we would handle errors in C, the example consists of opening a file.

``` c
#include <stdio.h>

int main(int argc, char* argv[]){
    FILE* file = fopen("file.txt","r");

    if(file == NULL){
        fprintf(stderr,"Ha ocurrido un error al abrir el archivo");
    }

    return 0;
}
```
As you may have noticed in this example, only an error is notified, but not which error it is.

To get a little more information, we can use an error number, most standard functions give us this number through the global variable **errno** defined in **<errno.h>**.


``` c
#include <stdio.h>
#include <errno.h>

extern int errno;

int main(int argc, char* argv[]){
    errno = 0;
    FILE* file = fopen("file.txt","r");

    if(file == NULL){
        fprintf(stderr,"Codigo: %d\n",errno);
    }

    return 0;
}
```

```
Codigo: 2
```

When the file does not exist and we try to open it with the **fopen()** function, the value of errno will change to 2.

Although we have more information about the error, a person still cannot understand it, for this we have the **perror(char\*)** function defined in **<stdio.h>** and **strerror(int)** defined in **<string.h>**.

**perror(char\*)** prints the string passed as a parameter, adding ": " plus the textual representation of the code.

**strerror(int)** returns a string of the textual representation of the code.

``` c
#include <stdio.h>
#include <string.h>
#include <errno.h>

extern int errno;

int main(int argc, char* argv[]){
    errno = 0;
    FILE* file = fopen("file.txt","r");

    if(file == NULL){
        perror("error");
        fprintf(stderr,"--- %s ---\n",strerror(errno));
    }

    return 0;
}
```

```
error: No such file or directory
--- No such file or directory ---
```

We can recreate this same error handling for our own functions.

### Example

Let's create a small example of how to create this style of error handler, we will make a divide function that returns an error when the divisor is 0.

```c
int divide(int a,int b){
    return a/b;
}

int main(int argc,char* argv[]){
    int result = divide(5,0);
}

```
At this point, we have a simple function without an error handler, now we will start modifying our code to add the error handler, in this case, we will use the "format" of modifying the error code through a pointer.


```c
typedef unsigned int ERROR_CODE;

int divide(int a,int b,ERROR_CODE* err_code){
    return a/b;
}

int main(int argc,char* argv[]){
    ERROR_CODE err_code;
    int result = divide(5,0,&err_code);
}

```

We also define our codes in sequence, in this case, we will only have two since it is a simple example.

```c
#define NO_ERROR 0
#define DIVIDER_ZERO 1 
```

Now within the function, we define in what cases each type of error will be notified.

```c
int divide(int a,int b,ERROR_CODE* err_code){
    if(b == 0){
        if(err_code) *err_code = DIVIDER_ZERO;

        return -1;
    }

    if(err_code) *err_code = NO_ERROR;

    return a/b;
}
```

At this point, we already have a function that has an error handler, now we can also textually represent the code as the strerror function.

We start by defining an array with all codes with their textual representation.

```c
const char* list_of_error[] = {
    [NO_ERROR] = "No error",
    [DIVIDER_ZERO] = "The divider is zero"
};
```

Now we create a function that will transform the code into a textual representation.

To do this, we only return what we have in the array, verifying that we do not exceed the size of the array.

```c
int n_err = sizeof(list_of_error)/sizeof(char*);

const char* get_string_error(ERROR_CODE err_code){
    if(err_code >= 0 && err_code < n_err)
        return list_of_error[err_code];
    else
        return "Not found error code.";
}
```

And now we can use it to find out what fails in our code.

```c
int main(int argc,char* argv[]){
    ERROR_CODE code;

    int result = divide(5,0,&code);

    if(code != NO_ERROR){
        fprintf(stderr,"Error: %s\n",get_string_error(code));
        return -1;
    }

    return 0;
}
```

```
Error: The divider is zero
```

Done, you now have an error handler.