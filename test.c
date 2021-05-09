#include <stdio.h>
#include <stdlib.h>

void main(){
    char c[1000];
    scanf("%s", &c);
    printf("%s", c);
    rewind(stdin);
    printf("hey");
    scanf("%s",&c);
    printf("%s", c);
    
}