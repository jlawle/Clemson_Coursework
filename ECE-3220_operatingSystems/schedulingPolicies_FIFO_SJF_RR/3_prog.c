/*
John Lawler CPSC/ECE-3220
Program 3: Scheduling Policy Simulation

This program simulates scheduling policies for fifo, sjf, and round robin
and prints out which thread will execute at what time depending on the
selected policy via cmd line arg

use cmd line arguments with file redirection:
./a.out [PolicyTag] < [filename]
    * where policyTag can be:
    * -fifo, -sjf, or -rr
    * filename being a text file
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 26

typedef struct taskTag {
    int task_id,
        arrival_time,
        service_time,
        remaining_time,
        completion_time,
        response_time,
        wait_time,
        check,
        preempted,
        finished;
    struct taskTag *next;
} task;

void printResults1();
void printResults2(int taskcount, task **threads);
void fifo(int taskCount, task **threads);
void rr(int taskCounter, task **threads);
void sjf(int taskCount, task **threads);
int handleInput(int taskCounter, task **threads);
void settimes(task *cpuThread, int time);
void freeThread(task *thread);
void printfinal(int taskCounter, task **threads);

int main(int argc, char *argv[]){
    if(argc < 1){ printf("Error\n"); exit(0); }
    int PolicyIdentifier,       //case switch identifier for policy
        taskCounter = 0,        //counts number of total tasks
        i;
    task *threads[MAX];         //matrix for storing up to 26 possible tasks

    //allocate for most possible threads (26)
    for(i = 0; i < MAX; i++) threads[i] = (task *)malloc(sizeof(task));

    //identify scheduling policy
    if(strcmp(argv[1], "-fifo") == 0)     PolicyIdentifier = 0; //set 0 = FIFO
    else if(strcmp(argv[1], "-sjf") == 0) PolicyIdentifier = 1; //1 is case sjf
    else if(strcmp(argv[1], "-rr") == 0)  PolicyIdentifier = 2; //2 is case rr
    else printf("Error determining policy...\n\n"),  exit(0);

    //handle file I/O, return number of tasks, place inputs into thread pointer
    taskCounter = handleInput(taskCounter, threads);

    //switch statement for selecting what poolicy to implement
   switch(PolicyIdentifier){
       case 0: //FIFO
            printf("FIFO Scheduling Results\n\n");
            printResults1();
            fifo(taskCounter, threads);
            break;
       case 1: //SJF
            printf("SJF(preemptive) Scheduling Results\n\n");
            printResults1();
            sjf(taskCounter, threads);
            break;
       case 2: //RR
            printf("RR Scheduling Results: Time Slice = 1\n\n");
            printResults1();
            rr(taskCounter, threads);
            break;
        default:
            printf("\n* hello. welcome to the default. *\n");

   }
   printfinal(taskCounter, threads);

    //free memory
    for(i = 0; i< taskCounter; i++){
        freeThread(threads[i]);
    }

    return 0;
}

/******************************************************************************/
/*************************** SIMULATOR FUNCTIONS ******************************/
/******************************************************************************/

/* function for handling RR printout */
void rr(int taskCounter, task **threads){

    //cpuThread points to the current thread that is to be run,
    //the rover is used to interate through for printing the ready queue
    task *cpuThread, *rover = (task *)malloc(sizeof(task));

    int tasks = taskCounter,    // constant to store overall task count
        readylist,              //count for threads on ready queue
        time = 0,               //set start of time
        active,                 //active = 1 if there is a thread to execute
        found,                  //set when a thread is found to be ran
        i;
    cpuThread = threads[0];
    while(taskCounter > 0){
        //check each thread if viabile to run
        for( active = 0, i = 0; i < tasks; i++){
            if( threads[i]-> remaining_time > 0
            &&  threads[i]->arrival_time <= time) active = 1;
        }

        if (!active) printf("%3d\t\t--\n", time++);
        else {
        /*  iterate through the linked list until the thread that is
            next ready to execute can be set as the cpuThread */
            while(!found){
                if((cpuThread = cpuThread->next) == NULL)
                    cpuThread = threads[0];     //loop to front
                if( cpuThread->remaining_time > 0
                    && cpuThread->arrival_time <= time)
                {
                     found = 1;                 //set found int
                     if((rover = cpuThread->next) == NULL)
                        rover = threads[0];     //set rover to next thread
                }
            }
            printf( "%3d\t%c%d\t",              //print thread currently running
                    time, cpuThread->task_id, cpuThread->remaining_time--);

            //when current thread finishes
            if(cpuThread->remaining_time == 0){
                settimes(cpuThread, time+1);    //set completion/response/wait
                taskCounter--;                } //decrement remaining tasks to run

            while (rover != cpuThread) {        //print the ready list if necessary
                if(rover->remaining_time > 0 && rover->arrival_time <= time){
                    printf("%c%d ", rover->task_id, rover->remaining_time);
                    readylist++;                //when an item is added to list
                }
                if((rover = rover->next) == NULL) rover = threads[0];
            }
            if(readylist == 0) printf("--");
            printf("\n");
            time++;                             //increment time counter
            found = readylist = 0;              //reset variables
        }
    }
    printResults2(tasks, threads);
    freeThread(cpuThread);
    freeThread(rover);
}

void sjf(int taskCounter, task **threads){
    int tasks = taskCounter, active, min, hit, i, time = 0;
    task *rover = (task *)malloc (sizeof(task));
    while(taskCounter > 0){
        for( active = 0, i = 0; i < tasks; i++){
            if( threads[i]-> remaining_time > 0
            &&  threads[i]->arrival_time <= time) active++;
        }
        if (!active) printf("%3d\t\t--\n", time++);
        else {
            //find the task with lowest remaing service time left
            //min = 0;
            for(min = 0, i = 0; i < tasks; i++){
                if(min == 0 && threads[i]->remaining_time > min){
                    min = threads[i]->remaining_time;
                    hit = i;
                }
                if( threads[i]->remaining_time > 0
                    && threads[i]->arrival_time <= time
                    && threads[i]->remaining_time < min){
                    min = threads[i]->remaining_time;
                    hit = i;
                }
            }
            printf( "%3d\t%c%d\t",              //print thread currently running
                    time, threads[hit]->task_id, threads[hit]->remaining_time--);

            if(threads[hit]->remaining_time == 0){
                threads[hit]->finished = 1;
                settimes(threads[hit], time+1);    //set completion/response/wait
                taskCounter--;                } //decrement remaining tasks to run

            //print ready list
            if(active > 1){
                if((rover = threads[hit]->next) == NULL) rover = threads[0];

                while (rover != threads[hit]) {        //print the ready list if necessary
                    if(rover->remaining_time > 0 && rover->arrival_time <= time){
                        printf("%c%d ", rover->task_id, rover->remaining_time);
                       // readylist++;                //when an item is added to list
                    }
                    if((rover = rover->next) == NULL) rover = threads[0];
                 }
            } else {
                printf("--");
            }
            printf("\n");
            time++;
        }


    }
    printResults2(tasks, threads);
    freeThread(rover);

}

/* function for handling FIFO printout*/
void fifo(int taskCount, task **threads){
    int i, time = 0;
    task *cpuThread, *rover = (task *)malloc (sizeof(task));

    //for loop to execute each task starting with A
    for(i = 0; i < taskCount; i++){

        cpuThread = threads[i];
        while(cpuThread->arrival_time > time)
            printf("%3d\t\t--\n", time++);      //executes one time for first thread

        if(cpuThread->arrival_time <= time){    //parameter check
            while(cpuThread->remaining_time > 0){
                printf("%3d\t%c%d\t",           //print thread currently running
                        time, cpuThread->task_id, cpuThread->remaining_time--);

                //check next tasks in ready queue for matching arrivals
                rover = cpuThread->next;
                if(rover != NULL && rover->arrival_time <= time){
                    while (rover != NULL && rover->arrival_time <= time) {
                        printf("%c%d ", rover->task_id, rover->remaining_time);
                        rover = rover->next;
                    }
                }   else printf("--");
                printf("\n");
                time++;
            }
            settimes(cpuThread, time);
        }
    }
    printResults2(taskCount, threads);
    freeThread(cpuThread);
    freeThread(rover);
}

/******************************************************************************/
/***************************** HELPER FUNCTIONS *******************************/
/******************************************************************************/

/* function for setting the input file */
int handleInput(int taskCounter, task **threads){
     while(fscanf(stdin,"%d %d",
            &threads[taskCounter]->arrival_time,    //read in arrival
            &threads[taskCounter]->service_time )   //read in service time
            && !feof(stdin))                        //until end of buffer reached
    {
        threads[taskCounter]->task_id = 'A'+taskCounter;
        threads[taskCounter]->finished = 0;
        threads[taskCounter]->check = 0;
        threads[taskCounter]->remaining_time =threads[taskCounter]->service_time;
        threads[taskCounter]->next = threads[taskCounter+1];

        taskCounter++;
        if(taskCounter > 26) printf("Too many tasks\n"), exit(0);
    }
    threads[taskCounter] = NULL;
    threads[taskCounter-1]->next = NULL;
    return taskCounter;
}

/* sets the times for the cputhread once it finishes executing  */
void settimes(task *cpuThread, int time){
    cpuThread->completion_time = time;
    cpuThread->response_time = cpuThread->completion_time - cpuThread->arrival_time;
    cpuThread->wait_time = cpuThread->response_time - cpuThread->service_time;
}

/* print header for time/cpu/queue */
void printResults1(){
    printf("time\tcpu\tready queue (tid/rst)\n");
    printf("----\t---\t---------------------\n");
}

/* print header for listing the diffirernt time stamps */
void printResults2(int taskcount, task **threads){
    int i;
    printf("\n\n     arrival service completion respose wait\n");
    printf("tid   time    time\ttime\t time\ttime\n");
    printf("--- ------- ------- ----------- -------- ----\n\n");

    for(i = 0; i < taskcount; i++){
        printf("%2c%8d%7d%8d%8d%8d\n",
                threads[i]->task_id,
                threads[i]->arrival_time,
                threads[i]->service_time,
                threads[i]->completion_time,
                threads[i]->response_time,
                threads[i]->wait_time);
    }
}
/* free a thread that is of type task, structure of times and link */
void freeThread(task *thread){
    if(thread != NULL) free(thread->next);
    free(thread);
}

void printfinal(int taskCounter, task **threads){
    int i;
    int min = threads[0]->service_time;
    task *rover, *rov2 = (task *)malloc(sizeof(task));
    printf("\nservice\twait\n time\ttime\n-------\t----\n");
    for(i = 0; i < taskCounter; i++){
        rov2 = rover = threads[0];
        while(rover != NULL){
            if(rover->check == 0){
                if(rover->service_time <= min){
                    rov2 = rover;
                }
            }
            rover = rover->next;
        }
        printf("   %d\t%d\n", rov2->service_time, rov2->wait_time);
        rov2->check = 1;
    }

}
/******************************************************************************/
