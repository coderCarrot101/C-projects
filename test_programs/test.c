#include <stdio.h>
#include <stdlib.h>

void getname(char *c) {

    while (*c != '\n') {// c++; is problem i think
        *c = getchar();
        if (*c != '\n') {
            c++;
        }
    }
}

int main() {
    char array[20];
    char *a = array;
    printf("\nplease type your name\n");
    getname(a);

    printf("\nhello %s\n", array);
    
}