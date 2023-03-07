#include <stdio.>
#include <unistd.h>

int main(){
	int f = fork();
	if (f==0){
		execl("ej1", (char*)NULL);
	}
	else {
		printf("%d\n", (int)getpid());
		execl("ej1", (char*)NULL);
	}
	return(0);
}