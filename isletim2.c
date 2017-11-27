
#include "var.c"



 
 

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

