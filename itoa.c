#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
    int port = 22000596;
    char s[20];
  //  itoa(port,s,10);
    sprintf(s,"%d",port);
    printf("string is %s\n", s);
return 0;
	

}
