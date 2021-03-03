//Kraisima Maittrechitt 6088060 sec 3
// Wattanachai Santipaiboon 6088163 sec 3

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <assert.h>
// libarary 

pthread_mutex_t mutaex; 

sem_t barberReady;   
sem_t accessWRSeats;
sem_t custReady;

// variable 
int numberOfFreeWRSeats; // number of free seats
int totalOfSeats; // total of seats
int* buffer; // buffer for not free chair
int currentOrder;
int counter;
int countcus =0;
int customerLEAVE;
long keep[];
pthread_t tid;         // set pthread_t = 
pthread_attr_t attr;

#define BARBER_SIZE 3 // 

void *barber(void *bar);
void *customer(void *cus);

void initializeData()
{
	pthread_mutex_init(&mutaex,NULL);
	sem_init(&barberReady,0,0);
	sem_init(&accessWRSeats,0,1);
	sem_init(&custReady,0,0);
	pthread_attr_init(&attr);
	currentOrder = 0;
	counter = 0;
}
void initializeData2()
{
	pthread_mutex_init(&mutaex,NULL);
	sem_init(&barberReady,0,0);
	sem_init(&accessWRSeats,0,1);
	sem_init(&custReady,0,0);
	
	currentOrder = 0;
	counter = 0;
}
int add_customer(int seatItem)
{
	int x = 0;
	if(numberOfFreeWRSeats > 0)
	{
		numberOfFreeWRSeats--;
		x = counter % totalOfSeats;
		buffer[x] =seatItem;
		counter++;
		
	}
	else
	{
		return 0;
	}
}
int remove_customer(int* seatItem)
{
	int x;
	if(numberOfFreeWRSeats<totalOfSeats)
	{
		numberOfFreeWRSeats++;
		x = currentOrder % totalOfSeats;
		*seatItem = buffer[x];
		currentOrder++;
		
	}
	else
	{
		return 0;
	}
}
/////////////// END customer's function and starting P///////////////////
void *barber(void *bar)
{
	int item;
	long tid;
	tid = (long) bar;
        
	while(1)
	{
		printf("Barber:%ld\t| Wait Customer \n",tid);
		sem_wait(&custReady);
		sem_wait(&accessWRSeats);
		pthread_mutex_lock(&mutaex);

		if(remove_customer(&item))
		{
			printf("Customer:%d\t| HairCut With Barber:%ld\n",item,tid);
	        sleep(1);
		}
		else
		{
			printf("customer removeable fail!\n");
		}
		pthread_mutex_unlock(&mutaex);
		sem_post(&barberReady);
		sem_post(&accessWRSeats);
		// cut hair 
		sleep(1);
                
	}
}
void* customer(void* cust)
{
	long tid;
	
	
	tid = (long) cust;
	// this part add delay time of any customer for coming shop
	sleep(1);
	sem_wait(&accessWRSeats);
	pthread_mutex_lock(&mutaex);
	if(add_customer(tid))
	{
		pthread_mutex_unlock(&mutaex);
		sem_post(&custReady);
		sem_post(&accessWRSeats);
		printf("Customer: %ld\t| Waiting for Queue\n",tid);
		sem_wait(&barberReady);
	}
	else
	{
		pthread_mutex_unlock(&mutaex);
		sem_post(&accessWRSeats);
		printf("Customer: %ld\t| Leave\n",tid);
	        keep[customerLEAVE]=tid;
		customerLEAVE++;

	}
}
void* customerleave(void* cust)
{
	long tid;
	
	  
	
	tid = (long) cust;

	// this part add delay time of any customer for coming shop
	sleep(5);
	sem_wait(&accessWRSeats);
	pthread_mutex_lock(&mutaex);
	if(add_customer(tid))
	{
                
		pthread_mutex_unlock(&mutaex);
		sem_post(&custReady);
		sem_post(&accessWRSeats);
		printf("Customer: %ld\t| Waiting for Queue\n",tid);
		
		sem_wait(&barberReady);
		
	}
	else
	{
		pthread_mutex_unlock(&mutaex);
		sem_post(&accessWRSeats);
		printf("Customer: %ld\t| Leave\n",tid);
	        keep[customerLEAVE]=tid;
		customerLEAVE++;

	}
}
int main(int argc, char* argv[])
{
	int i;
	int customer_number;
	customer_number = atoi(argv[1]);
	totalOfSeats = atoi(argv[2]);
        int c =0;
	numberOfFreeWRSeats = totalOfSeats;
	buffer = (int*)malloc(totalOfSeats*sizeof(int));
	// Initialize the program
	initializeData();
	// barber threads
	for(i=1;i<= BARBER_SIZE ;i++)
	{
		pthread_create(&tid,&attr,barber,(void*) i);
	}
	// customer thread // emtry chair
	for ( i=1;i<= customer_number;i++)
	{
		pthread_create(&tid,&attr,customer,(void*) i );
	}
	sleep(10);
	if( customerLEAVE>0)
	{
	  printf("---- Customer who leave,they came ----\n");
	}
	
	
	for(i=0;i<customerLEAVE;i++)
	{
                
		pthread_create(&tid,&attr,customerleave,(keep[i]));
	}
      

	// sleep to specified time in milliseconds
	sleep(15);
        for ( i=0;i< customerLEAVE; i++)
	{
	   
	}

	
	printf("-------------- Finish --------------\n");
	exit (0);
}
	
	
	
	
	
	


