/*	Name : Fahad Qayyum
 * 	EECS login: fahadq
 * 	YU ID : 215287253
 *      Assignment # 3 Question: Dining Philosophers
 * 	Course Code : EECS 3221
 * 	Course Name : Operating systems*/


#define PHILOSOPHERS 5
#define STATE_EATING 1
#define STATE_HUNGRY 2
#define STATE_THINKING 0
#define LIMIT 5
#define LEFT_CHOPSTICK (total + 1) % 5
#define RIGHT_CHOPSTICK total

#include<pthread.h>
#include<semaphore.h>
#include<stdio.h>
#include<string.h>
#include<time.h>

// Struct defining philosopher

typedef struct{
	int id;
	int state;
	int intervals[3];
	int eat_interval;
	int think_interval;
}Philosopher;

// ----------------------- Global variables and features ---------------------------

Philosopher philosopher[5];					// total philosophers are 5
void setter(int );						// setter feature
void setUpPhilosopher();					// setting up philosophers
void thinking(int);						// if the state is thinking		
void eating(int);						// if the state is eating
void hungry(int);						// if the state is hunry
sem_t chopSticks[PHILOSOPHERS];					// seampahore for chopsticks
int val[PHILOSOPHERS];						// value per philosopher

void main(){
	
	// ------------------- Local Variables ---------------------------
	int counter;
	char possibleStates[][10] = {"Thinking", "Eating", "Hungry"};
	
	setUpPhilosopher();
	
	// iterating 500 times
	for(counter = 1; counter<= 500; counter++){
		int j;
		for(j = 0; j < PHILOSOPHERS; j++){
			
			// for each philosopher set up the information based on what state it is in
			setter(j);
		}
		
	char array[3][500] = {"","", ""};
	for(j = 0; j < PHILOSOPHERS; j++){
		char string[99];
		
		// processes in the string
		sprintf(string, "P%d", j + 1);
		strcat(array[philosopher[j].state], string);
		
		// time left for each process after which it switchs
		sprintf(string, " (Time left: %d) ", philosopher[j].intervals[philosopher[j].state]);
		strcat(array[philosopher[j].state], string);
	}
	
	// output format
	printf("\n---------------------\n");
	printf("Iteration # %d:\n", counter);
	for(j = 0; j < 3; j++){
		printf("Philosophers in %s State: %s\n", possibleStates[j], array[j]);
	}
	}
}

void setUpPhilosopher(){
	
	int counter;
	
	// setting up the philosopher information
	for(counter = 0; counter< PHILOSOPHERS; counter++){
		sem_init(&chopSticks[counter], 0, 1);
		philosopher[counter].id = counter;
		
		// initial state for every philosopher is thinking
		philosopher[counter].state = STATE_THINKING;
		philosopher[counter].intervals[STATE_HUNGRY] = 0;
		
		// random eat interval
		philosopher[counter].eat_interval = rand() % LIMIT + 1;
		philosopher[counter].intervals[STATE_EATING] = philosopher[counter].eat_interval;
		
		// random think interval
		philosopher[counter].think_interval = rand() % LIMIT + 1;
		philosopher[counter].intervals[STATE_THINKING] = philosopher[counter].think_interval;
	}
	
}

void setter(int total){
	
	// if the state of the philosopher is THINKING
	if (philosopher[total].state == STATE_THINKING){
		thinking(total);
		
	// if the state of the philosopher is HUNGRY	
	}else if (philosopher[total].state == STATE_HUNGRY){
		hungry(total);
		
	// else the state of the philosopher is EATING	
	}else {
		eating(total);
	}
}

void thinking(int total){
	
	if(philosopher[total].intervals[STATE_THINKING] == 0){
		
		// switching states when th interval reaches 0
		philosopher[total].intervals[STATE_THINKING] = philosopher[total].think_interval;
		
		// change the state of the philosopher to hungry
		philosopher[total].state = STATE_HUNGRY;
		philosopher[total].intervals[STATE_HUNGRY]++;
	} else {
		philosopher[total].intervals[STATE_THINKING]--;
	}		
}

void hungry(int total){
	
	// checking for left and right chopsticks
	sem_getvalue(&chopSticks[LEFT_CHOPSTICK], &val[LEFT_CHOPSTICK]);
	sem_getvalue(&chopSticks[RIGHT_CHOPSTICK], &val[RIGHT_CHOPSTICK]);
	
	// if both the chopsticks are available
	if(val[LEFT_CHOPSTICK] == 1 && val[RIGHT_CHOPSTICK] == 1){
		sem_wait(&chopSticks[LEFT_CHOPSTICK]);
		sem_wait(&chopSticks[RIGHT_CHOPSTICK]);
			
		// change the philosopher state to eating
		philosopher[total].state = STATE_EATING;
		philosopher[total].intervals[STATE_HUNGRY] = 0;
		philosopher[total].intervals[STATE_EATING]--;		
	}else {
		philosopher[total].intervals[STATE_HUNGRY]++;
	}		
}

void eating(int total){
	
	// if the eating interval reaches 0
	if(philosopher[total].intervals[STATE_EATING] == 0){
		philosopher[total].intervals[STATE_EATING] = philosopher[total].eat_interval;
		sem_post(&chopSticks[LEFT_CHOPSTICK]);
		sem_post(&chopSticks[RIGHT_CHOPSTICK]);
			
		// change the state of the philosopher to thinking
		philosopher[total].state = STATE_THINKING;
		philosopher[total].intervals[STATE_THINKING]--;
	}else {
		philosopher[total].intervals[STATE_EATING]--;
	}	
}

