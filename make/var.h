#ifndef var
#define var

#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "unistd.h"
#include "string.h"
#include "fcntl.h"

int digit_command;
char *commands[10];

char *operators[10];
char *list[20];
const char *quit;
const char *s;
const char *b;
const char *amb;

int status;
int digit_input;
pid_t pid;
struct Child{
	pid_t pid;
	int status;

};
struct Child *Children[50];
int digit_child;
void cntrl(char []);
int cntrl_param();
int run_command(char *cmd,char *params[],int);
int write_file(char *cmd,char *params[],char *, char *);
int read_file(char *cmd,char *params[],char *, char *);



int cntrl_param(){
	char *cmd;
	
	char *param[20];
	 
	char *param2[20];
	 
	char *opr[20];
	int flag;
	
	int k,l,m;
	int digit_opr;

	
}
	
	


int run_command(char *cmd, char *param[],int flag){
	
	 
	
	struct Child *child;
	 
	
		
}
	 

	
int write_file(char *cmd, char *param[],char *opr,char * file){
	  
	
	 printf("write");
	struct Child *child;
	int fd;
	int p[2];
	pipe(p);
	child=malloc(sizeof(struct Child*));
	Children[digit_child++]=child;
	

	
			
	
	
}
	
int read_file(char *cmd, char *param[],char *opr,char * file){
	 printf("read");
	struct Child *child;

	int fd ,pid;
	

	

	
	
}
	
	


#endif