
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>


bool Swap(int * array,int index1,int index2)
{
	bool result = 0;
		if(array[index1]>array[index2])
		{
			int tmp = array[index2];
			array[index2]=array[index1];
			array[index1]=tmp;
			result=1;
		}
		
//printf("Comparing %d with %d,Swap %d\n",array[index1],array[index2],result);
		exit(  result);

}


int main( int num_arg, char **args )
{
	int sharmem_key = getuid();
	int arrlen = 45;
srand(time(NULL))	;
	int sharmem_id = shmget(sharmem_key,arrlen*sizeof(int),0644|IPC_CREAT);
	int *sharmem=(int*)shmat(sharmem_id,NULL,0);
	
for(int j = 0;j<arrlen;j++)
{
sharmem[j] = rand()%(arrlen+1);
}	




for(int j = 0;j<arrlen;j++)
{
printf("%d,",sharmem[j]);
}	

printf("\n");
		
	bool oddphase =  0;
	while(1)
{
	
int changes=0;	
	int pid = -1;
	int i;
	for ( i = oddphase; i+1 < arrlen; i+=2 )
	{
		pid = fork();
		if ( pid == 0 ) break;
	}

	//printf( "Ja jsem proces PID %d\n", getpid() );
	//getchar();

	if ( pid )
	{ // kod rodice
		int  status=-1;
		//printf( "I am parent with PID %d\n", getpid() );
		int wpid = -1;
		while ( ( wpid = wait( &status ) ) > 0 )
		{
			//printf( "Process %d ended with status %d\n", wpid,WEXITSTATUS(status) );
			changes+=status;

		}


		//getchar();
		//printf( "Parent finish...\n" );
if(oddphase == 0){oddphase = 1;}
else{oddphase=0;}
if(changes == 0){break;}
	}
	else
	{
	Swap(sharmem,i,i+1);
	}


}

for(int j = 0;j<arrlen;j++)
{
printf("%d,",sharmem[j]);
}	

printf("\n");
shmctl(sharmem_id,IPC_RMID,0);

}
