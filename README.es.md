# Manejador de errores
## C
En C no se tiene un manejador de errores como tal, la mayoria de las funciones retornan el codigo error en otras ocasiones se pasa un puntero para modificarlo y notificar el error.

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

Vamos a ver un simple ejemplo de como manejariamos los errores en C, el ejemplo consiste en abrir un archivo.

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
Como habras notado en este ejemplo solo se notifica que existe un error, pero no se especifica cual es el error.

Para obtener un poco mas de informacion podemos usar un numero de error, la mayoria de funciones estandar nos dan este numero mediante la variable global **errno** definida en **<errno.h>**.

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

Cuando el archivo no existe y lo intentamos abrir con la funcion **fopen()** el valor de errno cambiara a 2.

Aunque tenemos mas informacion acerca del error, todavia una persona no puede entenderlo para esto estan las funciones **perror(char\* )** definida en **<stdio.h>** y **strerror(int)** definida en **<string.h>**.

**perror(char\*)** nos imprime la string pasada como parametro agregando ": " mas la representacion textural del codigo.

**strerror(int)** nos devuelve una string de la representacion textual del codigo.

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
Podemos recrear este mismo manejo de errores para nuestras propias funciones.

### Ejemplo

Vamos a crear un pequeño ejemplo de como crear este estilo de manejador de errores, haremos una funcion de dividir que nos regrese un error cuando el divisor es 0.

```c
int divide(int a,int b){
    return a/b;
}

int main(int argc,char* argv[]){
    int result = divide(5,0);
}

```

En este momento tenemos una simple funcion sin un manejador de errores, ahora empezaremos a modificar nuestro codigo para agregar el manejador de errores en este caso usare el "formato" de modificar el codigo de error mediante un puntero.

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

Tambien definimos nuestros codigos en secuencia, este caso unicamente tendremos dos ya que es un ejemplo sencillo.

```c
#define NO_ERROR 0
#define DIVIDER_ZERO 1 
```

Ahora dentro de la funcion definimos en que casos se notificaran cada tipo de error.

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

En este momento ya tenemos una funcion que tiene un manejador de errores, ahora tambien podemos representar textualmente el codigo como la funcion **strerror**.

Empezamos definiendo un array con todos los codigos con su representacion textual.

```c
const char* list_of_error[] = {
    [NO_ERROR] = "No error",
    [DIVIDER_ZERO] = "The divider is zero"
};
```

Ahora creamos una funcion que nos transformara del codigo a la representacion textual.

Para esto unicamente retornamos lo que tenemos en el array, verificando que no se salga del tamaño del array.

```c
int n_err = sizeof(list_of_error)/sizeof(char*);

const char* get_string_error(ERROR_CODE err_code){
    if(err_code >= 0 && err_code < n_err)
        return list_of_error[err_code];
    else
        return "Not found error code.";
}
```

Ya podemos usarlo para obtener que falla en nuestro codigo.

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

Listo ya tienes un manejador de errores.
