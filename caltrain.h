#include <pthread.h>

struct station {
	pthread_mutex_t pc_mutex;
	pthread_cond_t trainIsLoading, trainReady;
	int waitingPassengers;
	int freeSeats;
	int passengerFoundPlace;
	
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
