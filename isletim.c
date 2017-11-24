

#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "unistd.h"
#include "string.h"
int digit_command=5;
char *commands[]={"","ls","cat","echo","file","\0"};

char *operators[]={"<",">","&","|",";"};
char *list[20];
const char *quit="quit";
const char *s=">";

int status;
int digit_input;
pid_t pid;
struct Child{
	pid_t pid;
	int status;

};
struct Child *Children[50];
int digit_child=0;
void cntrl(char []);
void cntrl_param();
int run_command(char *cmd,char *params[]);


void cntrl( char input[]){	
	
	
	int i=0;
	
	for(int i=0 ;i<strlen(input);i++){
		if(input[i]==' ' || input[i]=='\n'){
			char *tmp;
			tmp=malloc(sizeof(char*)*1);
			strncpy(tmp,input,i);
			list[digit_input++]=tmp;
			//printf("%s\n",list[a-1]);
			strcpy(input,&input[i+1]);
			i=0;
		}
	}	
	
}


void cntrl_param(){
	char *cmd;
	cmd=malloc(sizeof(char*));
	char *param[20];
	*param=malloc(sizeof(char*)*20);
	char *opr[20];
	int k=0,l=0,m=0;

	for (int i = 0; i < digit_input; ++i)
	{	
		for (int j = 0; j < sizeof(commands)/(sizeof(char *)) ; ++j)
		{	
			if(strcmp(list[i],quit)==0){
				exit(0);;
			}
			else if (strcmp(list[i],commands[j])==0)
			{
				cmd=commands[j];
			}else if(strcmp(operators[j],list[i])==0){
				opr[m++]=operators[j];
				printf("%s",operators[j]);
				if(strcmp(operators[j],s)==0){
					printf("as");
					if(strlen(list[i+1])!=0){

					}else{
						printf("Erorr Newline");
					}
				}

				
			}else if(strcmp(param[k-1],list[i])!=0 ){
				param[k++]=list[i];
					printf("aq");
			}


		}
	}
	
	param[k]=NULL;
	run_command(cmd,param);
	
	
}

int run_command(char *cmd, char *param[]){
	 //printf("cmd=%s.param1=%s.param2=%s\n",cmd,param[0],param[1]);
	struct Child *child;
	 
	child=malloc(sizeof(struct Child*));
	Children[digit_child++]=child;
	
	if((child->pid=fork())<0){
		printf("ERROR _child process failed\n");
        exit(1);	
	}else if(child->pid==0){
			
		if(execvp(cmd,param)<0){

			printf("ERROR _exec failed-%s-  %d\n",cmd,execvp(cmd,param));
			exit(1);
		}

	}else{
		while (wait(&status) != child->pid);
	}
	

	



}

int main(int argc, char *arg[]){
	
	*list=malloc(sizeof(char*)*20);
	printf("shell>");
	while(1){
		digit_input=0;
	
	
		char input[50];
		
		
		
		
		fgets(input, 50, stdin);
		
		cntrl(input);
		cntrl_param();
		 
		
		


		printf("shell>");
	
	}
	//for(int i=0 ; i< digit_input;i++) printf("%s\n",list[i]);
	
return 0;
}

