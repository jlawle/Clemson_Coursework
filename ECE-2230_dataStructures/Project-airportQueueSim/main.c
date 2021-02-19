/*
    John Lawler
    Molly Pabst
    Dec 5 2019

    the following main.c file is the general purpose driver for program 2.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "sim.h"
#include "event.h"
#include "queue.h"
#include "time.h"

#define MAX_SCAN 4

//static void parse_args(argc, argv);

int num_passengers = 0; /* counts the number of passengers */
int MAX_PASS, QSZ;

queue_t *airlineQ;
queue_t *idQ;
queue_t *scanQ[MAX_SCAN];
queue_t *trainQ;

int main(int argc, char **argv){
    int i, scanL=0; //scanL variable for scanQ array

    /* process command line arguments */
    int eventCount, opt;
    while((opt = getopt(argc, argv, "e:")) != -1){
        switch(opt){
            case 'e':
                eventCount = atoi(optarg);
                break;
            default:
                fprintf(stderr, "No -e command line argument detected. Error - exiting\n");
                exit(1);
        }
    }
    MAX_PASS = eventCount;
    QSZ = eventCount;

    /* initialize modules */
    event_init(QSZ);
    time_init();

    /* initialize queues */
    airlineQ = queue_init(QSZ);
    idQ = queue_init(QSZ);
    for(i = 0; i < MAX_SCAN; i++)
        scanQ[i] = queue_init(QSZ);
    trainQ = queue_init(QSZ);

    event_t *start_ev;
    start_ev = event_create();
    start_ev->passenger = passenger_create();
    start_ev->passenger->pass_id = ++num_passengers;
    start_ev->event_time = 0.0;
    start_ev->event_type = EV_ARRIVE;
    event_schedule(start_ev);
    /* run main loop */
    while(!event_empty())
    {
        event_t *new_ev;
        new_ev = event_cause();
        //time_set(new_ev->event_time);
        switch (new_ev->event_type)
        {
        case (EV_ARRIVE) :
        {
            event_t *airline_ev;

            printf("Passenger\t%d\tarrives at the airport:\t\t%0.2f\n",
                       new_ev->passenger->pass_id,
                       new_ev->event_time);

            airline_ev = event_create();
            airline_ev->passenger = new_ev->passenger;
            airline_ev->passenger->arrival_time = time_get();
            airline_ev->event_time = time_airlineQ();
            airline_ev->event_type = EV_AIRLINEQ;
            event_schedule(airline_ev);
            if (MAX_PASS > num_passengers)
            {
                event_t *arrive_ev;
                arrive_ev = event_create();
                arrive_ev->passenger = passenger_create();
                arrive_ev->passenger->pass_id = ++num_passengers;
                arrive_ev->event_time = time_arrive();
                arrive_ev->event_type = EV_ARRIVE;
                event_schedule(arrive_ev);
            }
        }
            break;
            case (EV_AIRLINEQ) :
            {

                new_ev->passenger->airlineQ_time = time_get();

                printf("Passenger\t%d\tenters the airline queue:\t%0.2f\n",
                       new_ev->passenger->pass_id,
                       new_ev->event_time);

                queue_insert(airlineQ, new_ev->passenger);
                if(queue_size(airlineQ) >= 1){
                    event_t *airline_ev;
                    airline_ev = event_create();
                    airline_ev->event_type = EV_AIRLINE;
                    airline_ev->passenger = queue_remove(airlineQ);
                    airline_ev->event_time = time_airlineQ();
                    airline_ev->queue = airlineQ;
                    event_schedule(airline_ev);
                }
            }
            break;
        case (EV_AIRLINE) :
            {
                event_t *airline_ev;
                //print passenger information and time as he immediately intiates this event
                printf("Passenger\t%d\tarrives at airline:\t\t%0.2f\n",
                       new_ev->passenger->pass_id,
                       new_ev->event_time);

                passenger_t *front;
                front = NULL;
                front = queue_remove(airlineQ);
                if(queue_size(airlineQ) != 0){  //make sure queue is not empty before creating a new event from the queue
                    airline_ev = event_create();
                    airline_ev->event_type = EV_AIRLINE;
                    airline_ev->passenger = front;
                    airline_ev->passenger->airline_time = time_get();
                    airline_ev->event_time = time_airline();
                    event_schedule(airline_ev);

                }
                    //make a new event of the next event type and intitialize it
                    event_t *id_ev;
                    id_ev = event_create();
                    id_ev->event_type = EV_IDQ;
                    id_ev->passenger = new_ev->passenger;
                    id_ev->event_time = time_idQ();
                    event_schedule(id_ev);


            }
            break;
        case (EV_IDQ) :
        {
            //get time for each time interval
            new_ev->passenger->idQ_time = time_get();

            printf("Passenger\t%d\tenters line for ID at:\t\t%0.2f\n",
                       new_ev->passenger->pass_id,
                       new_ev->event_time);
            //insert the new event into the queue for the event type
            queue_insert(idQ, new_ev->passenger);
            if(queue_size(idQ) >= 1){
                event_t *id_ev;
                id_ev = event_create();
                id_ev->event_type = EV_ID;
                id_ev->passenger = queue_remove(idQ);
                id_ev->event_time = time_id();
                id_ev->queue = idQ;
                event_schedule(id_ev);


            }
        }
            break;
        case (EV_ID) :
            {
                event_t *id_ev;

                printf("Passenger\t%d\trecieves ID at:\t\t\t%0.2f\n",
                       new_ev->passenger->pass_id,
                       new_ev->event_time);

                passenger_t *front;
                front = NULL;
                front = queue_remove(idQ);
                if(queue_size(idQ) != 0){
                    id_ev = event_create();
                    id_ev->event_type = EV_ID;
                    id_ev->passenger = front;
                    id_ev->passenger->id_time = time_get();
                    id_ev->event_time = time_id();
                    event_schedule(id_ev);
                }

                //create scan event
                event_t *scan_ev;
                scan_ev = event_create();
                scan_ev->event_type = EV_SCANQ;
                scan_ev->passenger = new_ev->passenger;
                scan_ev->event_time = time_scanQ();

                event_schedule(scan_ev);




            }
            break;
        case (EV_SCANQ) :
            {
                new_ev->passenger->scanQ_time = time_get();
                
                printf("Passenger\t%d\tenters Scan queue at:\t\t%0.2f\n",
                       new_ev->passenger->pass_id,
                       new_ev->event_time);
                if(scanL >= 4) scanL = 0;
                queue_t* temp = scanQ[scanL];
                               
                queue_insert(temp,new_ev->passenger);
                if(queue_size(temp) >= 1){

                    event_t *scan_ev;
                    scan_ev = event_create();
                    scan_ev->event_type = EV_SCAN;
                    scan_ev->passenger = queue_remove(temp);
                    scan_ev->event_time = time_scanQ();
                    scan_ev->queue = temp;
                    event_schedule(scan_ev);
                }
                scanL++;
            }
            break;
        case (EV_SCAN) :
                {
                event_t *scan_ev;
                printf("Passenger\t%d\tis scanned at:\t\t\t%0.2f\n",
                       new_ev->passenger->pass_id,
                       new_ev->event_time);

                passenger_t *front;
                front = NULL;
                front = queue_remove(new_ev->queue);
                if(queue_size(new_ev->queue) != 0){
                    scan_ev = event_create();
                    scan_ev->event_type = EV_SCAN;
                    scan_ev->passenger = front;
                    scan_ev->passenger->scan_time = time_get();
                    scan_ev->event_time = time_scan();
                    event_schedule(scan_ev);
                }

                event_t *train_ev;
                train_ev = event_create();
                train_ev->event_type = EV_TRAINQ;
                train_ev->passenger = new_ev->passenger;
                train_ev->event_time = time_trainQ();
                event_schedule(train_ev);
            }
            break;
        case (EV_TRAINQ) :
        {
            new_ev->passenger->trainQ_time = time_get();

            printf("Passenger\t%d\tenters line for train at:\t%0.2f\n",
                       new_ev->passenger->pass_id,
                       new_ev->event_time);

            queue_insert(trainQ, new_ev->passenger);
            if(queue_size(trainQ) >= 1){
                event_t *train_ev;
                train_ev = event_create();
                train_ev->event_type = EV_TRAIN;
                train_ev->passenger = queue_remove(trainQ);
                train_ev->event_time = time_train();
                train_ev->queue = trainQ;
                event_schedule(train_ev);
            }
        }
            break;
        case (EV_TRAIN) :
        {
            event_t *train_ev;

                printf("Passenger\t%d\tenters the train at:\t\t%0.2f\n",
                       new_ev->passenger->pass_id,
                       new_ev->event_time);

                passenger_t *front;
                front = NULL;
                front = queue_remove(trainQ);
                if(queue_size(trainQ) != 0){
                    train_ev = event_create();
                    train_ev->event_type = EV_TRAIN;
                    train_ev->passenger = front;
                    train_ev->passenger->train_time = time_get();
                    train_ev->event_time = time_train();
                    event_schedule(train_ev);
                }

                event_t *gate_ev;
                gate_ev = event_create();
                gate_ev->event_type = EV_GATE;
                gate_ev->passenger = new_ev->passenger;
                gate_ev->event_time = time_trainQ();
                event_schedule(gate_ev);
        }
            break;
        case (EV_GATE) :
        {
            printf("Passenger\t%d\thas arrived to the gate.\t%0.2f\t Have a safe flight!\n",
                       new_ev->passenger->pass_id,
                       new_ev->event_time);
            event_destroy(new_ev);
        }
            break;

        default :
            /* error */
            break;
        }
        /* free event */
    }
    /* Print overall stats */
    /* finalize modules */
    event_finalize();
    time_finalize();
    return 0;
}
