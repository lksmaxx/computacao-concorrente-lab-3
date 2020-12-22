#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"


#define N_THREADS 4

struct
{
	int id;
	long long int n;
	double soma;
	double pos,neg;
}typedef t_arg;


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
void* pi_series_conc(void* arg)
{
	t_arg* args = (t_arg*) arg;
	//printf("t%d criada\n",args->id);
	args->soma = 0;
	for(long long int i = 2 * args->id + 1; i < args->n; i+= 2 * N_THREADS)
	{
		args->soma += 4.0f / i;
	}
	if(args->id % 2)
		args-> soma *= -1;
	//printf("t%d result = %f\n",args->id,args->soma);
	pthread_exit((void*) args);
}
void* pi_series_conc2(void* arg)
{
	t_arg* args = (t_arg*) arg;
	//printf("t%d criada\n",args->id);
	double pos = 0,neg = 0;
	//f(n) = 1/2*n	
	for(long long int i =(args->n/(float)N_THREADS)  * args->id + 1; i < (args->id + 1) * (args->n/(float)N_THREADS); i+= 4)
	{
		pos += 4.0f / i;
		neg -= 4.0f / (i + 2);
	}
	args->soma = pos + neg;
	//printf("t%d result = %f\n",args->id,args->soma);
	pthread_exit((void*) args);
}

int main(int argc,char* argv[])
{
	if(argc < 2)
		error("Argumentos invalidos.",1);
	long long int n = pow(10, (int)atoll(argv[1]));
	if(!n)
		error("Erro ao converter o argumento\n",2);
	
	t_arg t_argv[N_THREADS];
	pthread_t ids[N_THREADS];
	double inicio,fim;
	GET_TIME(inicio);
	double soma_seq = pi_series(n);
	printf("sequencial:\nPI 	= %.15f\nPI real = %.15f\n",soma_seq,M_PI);
	GET_TIME(fim);
	printf("|M_PI - pi|/M_PI = %.15f\n",fabs(M_PI - soma_seq)/M_PI);
	double tempo_seq = fim - inicio;
	printf("tempo sequencial: %f\n",tempo_seq);
	GET_TIME(inicio);
	for(int i = 0; i < N_THREADS; i++)
	{
		t_argv[i].id = i;
		t_argv[i].n = n;
		pthread_create(&ids[i],NULL,(void*)pi_series_conc2,(void*) &t_argv[i]);	
	}
	double soma = 0;
	for(int i = 0; i < N_THREADS; i++)
	{
		t_arg* temp;
		pthread_join(ids[i],(void**) &temp);
		soma += temp->soma;


	}
	GET_TIME(fim);
	double tempo_conc = fim - inicio;
	printf("concorrente:\nPI 	= %.15f\nPI real = %.15f\ntempo concorrente: %f\n",soma,M_PI,tempo_conc);
	printf("|M_PI - pi|/M_PI = %.15f\n",fabs(M_PI - soma)/M_PI);
	printf("Tseq/Tconc: %f\n",tempo_seq/tempo_conc);

	
	return 0;
}
