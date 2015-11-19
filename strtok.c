#include <string.h>
#include <stdio.h>

int main()
{

   FILE *f = NULL;
   //char str[80] = "This is - www.tutorialspoint.com - website";
   char buf[100];
   const char s[2] = " ";
   char *token;

   f = fopen("server.txt", "r");
	
   while(fgets(buf, sizeof(buf), f) != NULL){
   /* get the first token */

      token = strtok(buf, s);
   
   /* walk through other tokens */
      while( token != NULL ) 
      {
         printf( " %s\n", token );
    
         token = strtok(NULL, s);
      }
	token = NULL;	
   }

   fclose(f);
   return(0);
}
