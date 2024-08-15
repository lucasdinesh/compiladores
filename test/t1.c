#include <stdio.h>

int a = 8;
int b[] = {1,2,3};
int main()
{
    if(a==8){
        printf("OK");
    }

    b[1] = b[1] + 5;
}