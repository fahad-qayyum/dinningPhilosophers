/*	Name : Fahad Qayyum
 * 	EECS login: fahadq
 * 	YU ID : 215287253
 *  Assignment # 3 Question Dining Philosophers
 * 	Course Code : EECS 3221
 * 	Course Name : Operating systems*/

#include<pthread.h>
#include<semaphore.h>
#include<stdio.h>
#include<string.h>
#include<time.h>

#define NUM 5
#define EATING 1
#define HUNGRY 2
#define THINKING 0
#define MAX_INTERVAL 5
#define NUM_ITERATION 500
#define LEFT_CHOP (num + 1) % 5
#define RIGHT_CHOP num

typedef struct{
	int id;
	int state;
	int intervals[3];
	int eat_interval;
	int think_interval;
}Philosopher;

sem_t chops[NUM];
int sem_value[NUM];
Philosopher phil[5];

void philosopher(int num){
	if (phil[num].state == THINKING){
		if(phil[num].intervals[THINKING] == 0){
			phil[num].intervals[THINKING] = phil[num].think_interval;
			phil[num].state = HUNGRY;
			phil[num].intervals[HUNGRY]++;
		} else {
			phil[num].intervals[THINKING]--;
		}
	}else if (phil[num].state == HUNGRY){
		sem_getvalue(&chops[LEFT_CHOP], &sem_value[LEFT_CHOP]);
		sem_getvalue(&chops[RIGHT_CHOP], &sem_value[RIGHT_CHOP]);
		
		if(sem_value[LEFT_CHOP] == 1 && sem_value[RIGHT_CHOP] == 1){
			sem_wait(&chops[LEFT_CHOP]);
			sem_wait(&chops[RIGHT_CHOP]);
			phil[num].state = EATING;
			phil[num].intervals[HUNGRY] = 0;
			phil[num].intervals[EATING]--;		
		}else {
			phil[num].intervals[HUNGRY]++;
		}
	}else {
		if(phil[num].intervals[EATING] == 0){
			phil[num].intervals[EATING] = phil[num].eat_interval;
			sem_post(&chops[LEFT_CHOP]);
			sem_post(&chops[RIGHT_CHOP]);
			phil[num].state = THINKING;
			phil[num].intervals[THINKING]--;
		}else {
			phil[num].intervals[EATING]--;
		}
	}
	
}
int main(){
	int i;
	char state[][10] = {"Thinking", "Eating", "Hungry"};
	rand(time(NULL));
	
	for(i = 0; i< NUM; i++){
		sem_init(&chops[i], 0, 1);
		phil[i].id = i;
		phil[i].state = THINKING;
		phil[i].intervals[HUNGRY] = 0;
		phil[i].eat_interval = rand() % MAX_INTERVAL + 1;
		phil[i].intervals[EATING] = phil[i].eat_interval;
		phil[i].think_interval = rand() % MAX_INTERVAL + 1;
		phil[i].intervals[THINKING] = phil[i].think_interval;
	}
	for(i = 1; i<= NUM_ITERATION; i++){
		int j;
		for(j = 0; j < NUM; j++){
			philosopher(j);
		}
		
	char outputs[3][100] = {"","", ""};
	for(j = 0; j < NUM; j++){
		char buf[10];
		sprintf(buf, "P%d", j + 1);
		strcat(outputs[phil[j].state], buf);
		sprintf(buf, "(%d)", phil[j].intervals[phil[j].state]);
		strcat(outputs[phil[j].state], buf);
	}
	printf("Iteration #%d:\n", i);
	for(j = 0; j < 3; j++){
		printf("Philosophers in %s State: %s\n", state[j], outputs[j]);
	}
	printf("\n");
}
}
