#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

void error(char* msg,int error_number)
{
	printf("ERROR NUMBER: %d\n%s\n",error_number,msg);
	exit(error_number);
}

double pi_series(long long n)
{
	double pos = 0,neg = 0;
	for(long long i = 1; i< n; i+= 4)
	{
		pos += 1.0/i;
		neg -= 1.0/(i + 2.0);
	}
	return (pos + neg)*4;
}

int main(int argc,char* argv[])
{
	if(argc < 2)
		error("Argumentos invalidos.",1);
	long long int n = atoll(argv[1]);
	if(!n)
		error("Erro ao converter o argumento\n",2);
	printf("PI 	= %.15f\nPI real = %.15f\n",pi_series(n),M_PI);
	
	
	return 0;
}
