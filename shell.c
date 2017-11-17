#include "stdio.h"
#include "unistd.h"
#include "string.h"



int main(int arg, char *argv[]){
	    char *komutlar[20]={"cat","ls","echo","grep"};
	    char *fonks[20]={"cat","ls"};
	     
	for(int i=0; i<4; i++){

	    for(int j=1 ; j<arg; j++){
			
		if(strcmp(komutlar[i],argv[j])==0) printf("%s,\n", argv[j]);

	    }

	}


return 0;
}
