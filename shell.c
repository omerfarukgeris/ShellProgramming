
#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "unistd.h"
#include "string.h"
#include "fcntl.h"
int digit_command=5;
char *commands[]={"","ls","cat","echo","file","sleep","id","\0"};

char *operators[]={"<",">","|",";"};
char *list[20];
const char *quit="quit";
const char *s=">";
const char *b="<";
const char *amb="&";

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
int cntrl_param();
int run_command(char *cmd,char *params[],int);
int write_file(char *cmd,char *params[],char *, char *);
int read_file(char *cmd,char *params[],char *, char *);

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


int cntrl_param(){
	char *cmd;
	cmd=malloc(sizeof(char*));
	char *param[20];
	*param=malloc(sizeof(char*)*20);
	char *param2[20];
	*param2=malloc(sizeof(char*)*20);
	char *opr[20];
	int flag=0;
	
	int k=0,l=0,m=0;
	int digit_opr=0;

	for (int i = 0; i < digit_input; i++)
	{	
		for (int j = 0; j < sizeof(commands)/(sizeof(char *)) ; j++)
		{	
			if(strcmp(list[i],quit)==0){
				 
				exit(0);
			}
			else if (strcmp(list[i],commands[j])==0)
			{
				cmd=commands[j];
			}else if(strcmp(operators[j],list[i])==0){
				opr[m++]=operators[j];
				digit_opr++; 
			
				break;
				
			}else if(strcmp(param[k-1],list[i])!=0 && strcmp(list[i],s)!=0 && strcmp(list[i],b)!=0 && strcmp(list[i],amb)!=0) {
				if(digit_opr<1){
					param[k++]=list[i];
				}else{
					param2[l++]=list[i];
				}
			}
			if(strcmp(list[i],amb)==0) {
				flag=1; 
		
			}


		}
	}
	
	param[k]=NULL;
	if(digit_opr>0){
		if (*opr[0]=='>')
		{
			write_file(cmd,param,*opr,*param2);
		}else if(*opr[0]=='<'){
			read_file(cmd,param,opr[0],param2[0]);
		}else if(flag==1){

				run_command(cmd,param,flag);
				flag=0;
		}
		
	}else{

		run_command(cmd,param,flag);
	}	
}
	


int run_command(char *cmd, char *param[],int flag){
	
	 
	
	struct Child *child;
	 
	child=malloc(sizeof(struct Child*));
	Children[digit_child++]=child;
	
	if((child->pid=fork())<0){
		fflush(stdin);
		printf("ERROR _child process failed\n");
        exit(1); 
	}
	else if(child->pid==0){
			
	
		if(execvp(cmd,param)<0){

			printf("ERROR _exec failed-%s-  %d\n",cmd,execvp(cmd,param));
			
			exit(1);
		}else{
				exit(0);
		}

 	
	}else{

		if(flag==0){
			pid=wait(pid,&status);
			int exstat = WEXITSTATUS(status);
			//printf("%d-%d-%d",pid,status,exstat);	

		}
		
		

	}
			
		
		
}
	 

	
int write_file(char *cmd, char *param[],char *opr,char * file){
	  
	
 
	struct Child *child;
	int fd;
	int p[2];
	pipe(p);
	child=malloc(sizeof(struct Child*));
	Children[digit_child++]=child;
	

	
			
	
	if((child->pid=fork())<0){
		printf("\e[31m\e[1mERROR _child process failed\n\e[1m");
        exit(1);	
	}else if(child->pid==0){
		 close(1);
	
			if(fd=open(file,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR)<0){
				perror("Error File open\n");
				exit(EXIT_FAILURE);
			}
			dup2(fd,0);
			
			
			
		if(execvp(cmd,param)<0){
			
			printf("\e[1m\e[32mERROR _exec failed\n\e[0m");
			exit(1);
		}
		close(0);
			

	}else{
 
		
		pid=wait(&status);
		int exstat = WEXITSTATUS(status);
		//printf("%d-%d-%d",pid,status,exstat);	
	}
}
	
int read_file(char *cmd, char *param[],char *opr,char * file){
	 
	struct Child *child;

	int fd ,pid;
	

	child=malloc(sizeof(struct Child*));
	Children[digit_child++]=child;
	

	
			
	
	if((child->pid=fork())<0){
		printf("\e[31m\e[1mERROR _child process failed\n\e[1m");
        exit(1);	
	}else if(child->pid==0){
		 close(0);
		
			if(fd=open(file,O_RDONLY)<0){
				perror("Error File open\n");
				exit(EXIT_FAILURE);
			}
			dup2(fd,0);
			
			
			 
		if(execvp(cmd,param)<0){
			
			printf("\e[1m\e[32mERROR _exec failed\n\e[0m");

			exit(1);
		}
			

	}else{
			
		pid=wait(&status);
		int exstat = WEXITSTATUS(status);

		//printf("%d-%d-%d",pid,status,exstat);	

	}
	
	



}
 
 

int main(int argc, char *arg[]){

	
	*list=malloc(sizeof(char*)*20);
	printf("\e[1m\e[32mshell>\e[0m");
	while(1){

		 
		digit_input=0;
	
	
		char input[50];
		
		 


	 	fflush(stdin);
		fgets(input, 50, stdin);
			
		 
			
		cntrl(input);

		cntrl_param();
			
		
	



		printf("\e[1m\e[32mshell>\e[0m");
		
	}
	 
	
return 0;
}

