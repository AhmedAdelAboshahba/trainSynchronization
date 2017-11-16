#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
	
	//initialize the mutex
	pthread_mutex_init(&station->pc_mutex, NULL);
	
	//initialize conditions
	pthread_cond_init(&station->trainIsLoading,NULL);
	pthread_cond_init(&station->trainReady,NULL);
	
	//no waiting passengers at the begining
	station->waitingPassengers =0;
	station->freeSeats =0;
	station->passengerFoundPlace=0;
}

void
station_load_train(struct station *station, int count)
{
	//acquire lock
	pthread_mutex_lock(&station->pc_mutex);
	
	//set number of free seats
	station->freeSeats = count;
	
	//wait untill one of the conditions occurs
	if(station->freeSeats >0 && station->waitingPassengers>0){
		
		//Notify all passengers that the train arrived
		pthread_cond_broadcast(&station->trainIsLoading);
		
		//Wait until train is ready to move
		pthread_cond_wait(&station->trainReady,&station->pc_mutex);
	}
	
        //to be reused by other trains
	station->freeSeats =0;
	station->passengerFoundPlace=0;
	
	//free lock
	pthread_mutex_unlock(&station->pc_mutex);
	
}

void
station_wait_for_train(struct station *station)
{
	
    
	//acquire lock
	pthread_mutex_lock(&station->pc_mutex);

	//increment number of passengers
	station->waitingPassengers++;
	
	//wait until find an empty seat
	while(station->freeSeats==0)   pthread_cond_wait(&station->trainIsLoading,&station->pc_mutex);
	
	//one more passenger found a place
	station->passengerFoundPlace++;
	
	//decrease number of free seats
	station->freeSeats--;
	
	//decrease number of waiting passengers
	station->waitingPassengers--;
		
        //free lock
	pthread_mutex_unlock(&station->pc_mutex);
	
}

void
station_on_board(struct station *station)
{
	//acquire lock
	pthread_mutex_lock(&station->pc_mutex);
	
	//decrease number of passengers found place to know the last passenger
	station->passengerFoundPlace--;
	
	//wake up the train to notify that the train is ready to move
	if(station->passengerFoundPlace == 0 && (station->freeSeats == 0 || station->waitingPassengers == 0))    pthread_cond_signal(&station->trainReady);
	
	//free lock
	pthread_mutex_unlock(&station->pc_mutex);
	
}
