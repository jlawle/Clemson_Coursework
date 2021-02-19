/* CPSC/ECE 3220 server for client server
    John Lawler C10372381
    Feb 6 2019
 *
 * simple server that maintains a counter for each client
 *
 * request    meaning              reply
 * -------    -------              -----
 *   '0'      clear counter        counter value
 *   '+'      increment counter    counter value
 *   '-'      decrement counter    counter value
 *   '!'      stop                 <no reply>
 *
 * there is a timeout of 2 seconds for the call to select()
 */

#include "client_server.h"

int counters[MAX_CLIENTS];

void server( int client_count, int req_fds[][2], int reply_fds[][2] ){

  fd_set read_fds;        /* file descriptor set for active clients      */
  fd_set query_fds;       /* file descriptor set passed to select()      */
  struct timeval timeout; /* time value structure for select() timeout   */
  int active_clients;     /* current count of clients; end server when 0 */
  int max_fds = 0;        /* must be one more than largest req_fd value  */
  int retval;             /* return value from select ()                 */
  int found;              /* logical flag that a req_fd was identified   */
  int reply;              /* counter value sent as a reply               */
  char request;           /* single-character command sent to server     */
  int i;

  active_clients = client_count;             /* initialize active clients */
  max_fds = req_fds[0][0];                   /* initialize max file descriptor */
  FD_ZERO(&read_fds);                        /* initialize read_fds set */

  for(i = 0; i < active_clients; i++){       /* initialize for each active_client */
    close(req_fds[i][WRITE_FD]);             /* close unused ends of pipes */
    close(reply_fds[i][READ_FD]);
    FD_SET(req_fds[i][READ_FD], &read_fds);  /* Assign request to read_fds set */
    if(req_fds[i][1] > max_fds){             /* update max fds */
        max_fds = req_fds[i][1]; }
    counters[i] = 0;                         /* clear counter for client */
  }

  /* display information about server and file descriptors */
  printf( "server with %d clients, which have file descriptors\n",
    client_count );
  for( i = 0; i < client_count; i++ ){
    printf( "  %d %d %d %d\n",
      req_fds[i][0], req_fds[i][1], reply_fds[i][0], reply_fds[i][1] );
  }

  while( active_clients ){
    query_fds = read_fds;   /* reinitialize query file descriptors on each loop */
    timeout.tv_sec = 2;     /* set timeout value for 2 seconds */
    timeout.tv_usec = 0;

    retval = select(max_fds+1, &query_fds, NULL, NULL, &timeout);
    if(retval == -1){
      printf("\nError code returned from select().\n");
      active_clients = 0;
    }
    else if(retval == 0){
      printf("\nTimeout returned from select().\n");
      active_clients = 0;
    }
    else if(retval > 0){
      //for loop for checking each file descriptor for the found value
        for(i=0; i < client_count; i++){
            found = FD_ISSET(req_fds[i][READ_FD], &query_fds);
                if(found){
                    //print out the character found
                    read(req_fds[i][READ_FD], &request, 1);
                    switch(request){
                        case '0':
                            //clear counter and reply with value using call to write
                           printf("Client %i counter value %i reset to 0.\n",i,counters[i]);
                            counters[i] = 0;
                            reply = counters[i];
                            write(reply_fds[i][WRITE_FD], (char *) &reply, 4);
                            break;
                        case '+':
                          //  printf("message from client %i to server is %c\n",i,request);
                            counters[i]++;
                            reply = counters[i];
                            write(reply_fds[i][WRITE_FD], (char *) &reply, 4);
                            break;
                        case '-':
                          //  printf("message from client %i to server is %c\n",i,request);
                            counters[i]--;
                            reply = counters[i];
                            write(reply_fds[i][WRITE_FD], (char *) &reply, 4);
                        case '!':
                            printf("message from client %i to server is %c\n",i,request);
                          //decrement count
                           active_clients =- 1;


                            break;
                    } //end of switch statement
                } //end of if(found) statement
            } //end of for()
    } //end of if(retval > 0)
  } // end of while()

  //close other ends of pipes and clear all file descriptors from the reply set
  for(i = 0; i < active_clients; i++){
    FD_CLR(req_fds[i][READ_FD], &read_fds);
    close(req_fds[i][WRITE_FD]);
    close(reply_fds[i][READ_FD]);
  }
  exit( 0 );
}
