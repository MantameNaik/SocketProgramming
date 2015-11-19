#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main()
{

   FILE *f = NULL;
   char buf[100];
   char temp[100];
   int i =0;
   int j =0;
   int arr[4];
   char *serv;
   int count = 0;
   int cost = 0;
   	f = fopen("serverA.txt", "r");
	memset(buf,0,100);
	while(fgets(buf, sizeof(buf), f) != NULL) /* when fgets reaches EOF, it retuens NULL */
        {
                printf("The line is %s\n",buf);
                memset(temp,0,100);
		count = 0;
                i = 0;
                while(buf[i] != '\n'){  // Reading each line
                        j=0;
                        while(buf[i]!=' ') // reading each string
                        {
			    if(buf[i] == '\0')
                                break;
			    if(buf[i] == '\n')
				break;
                            temp[j]=buf[i];
                            j++;
                            i++;
			}
                        temp[j] = '\0';
			printf("temp value is %s\n", temp);
			if(count == 0){
			      strcpy(serv,temp);
			      printf("serv value is %s\n", serv);
			      count++;
			}
			else{
			    if(strcmp(serv, "serverA")==0){
				cost = atoi(temp); 
				arr[0] = cost;
			    }	
			    if(strcmp(serv, "serverB")==0){
                                cost = atoi(temp);
                                arr[1] = cost;
                            }
			    if(strcmp(serv, "serverC")==0){
                                cost = atoi(temp);
                                arr[2] = cost;
                            }
			    if(strcmp(serv, "serverD")==0){
                                cost = atoi(temp);
                                arr[3] = cost;
                            }	
		       }
		if(buf[i] != '\0' || buf[i] != '\n')
		i++;
		break;
		}
	}
 	printf("The array is %d,%d,%d,%d\n", arr[0],arr[1],arr[2],arr[3]);
	fclose(f);

   return 0;
}
