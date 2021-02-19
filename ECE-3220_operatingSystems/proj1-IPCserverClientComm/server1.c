/* CPSC/ECE 3220 server for client server
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



/* helper functions can be defined here and used below */



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
  int i, j=0;

  active_clients = client_count;
  max_fds = req_fds[0][0];

  /* initially clear the file descriptor set for active clients */
  FD_ZERO(&read_fds);

  /* initialize for each client:

    - close unused ends of  pipes
          when reading, close element 1, if writing then close element 0 */

  for(i = 0; i < active_clients; i++){
    /* - set req_fd in the read_fds structure using FD_ZERO and FD_SET */
    close(req_fds[i][WRITE_FD]);
    close(reply_fds[i][READ_FD]);
    FD_SET(req_fds[i][READ_FD], &read_fds);

    /* - update max_fds if necessary */
    if(req_fds[i][1] > max_fds){
      max_fds = req_fds[i][1];
    }
    /* - clear counter for client             */
    counters[i] = 0;
  }

  /* set timeout value for 2 seconds */

  timeout.tv_sec = 2;
  timeout.tv_usec = 0;

  /* display information about server and file descriptors */

  printf( "server with %d clients, which have file descriptors\n",
    client_count );
  for( i = 0; i < client_count; i++ ){
    printf( "  %d %d %d %d\n",
      req_fds[i][0], req_fds[i][1], reply_fds[i][0], reply_fds[i][1] );
  }

  /* start processing with all clients active and continue until 0; */
  /*   stop requests should decrement the count of active clients   */
  query_fds = read_fds;

  while( active_clients != 0){
    query_fds = read_fds;
    /* prep for call to select */
    found = 0;
    timeout.tv_sec = 2;
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
        for(i=0; i < client_count; i++){
            found = FD_ISSET(req_fds[i][READ_FD], &query_fds);
                if(found){
                    read(req_fds[i][READ_FD], &request, 1);
                  //  printf("%c\n",request);
                    switch(request){
                        case '0':
                            //clear counters
                            break;
                        case '+':
                            counters[i]++;
                            reply = counters[i];
                            write(reply_fds[i][WRITE_FD], &reply, 4);
                            break;
                        case '-':

                            counters[i]--;
                            reply = counters[i];
                            write(reply_fds[i][WRITE_FD], &reply, 4);
                            //close(reply_fds[i][WRITE_FD]);
                            break;

                        case '!':
                            //clear client
                            active_clients--;
                            break;
                    }
                }
            }


    }
  }
  /* use FD_CLR macro to remove corresp. read_fd from set after client ends
    close the other file descriptors */

    printf("out of while\n");

  exit( 0 );
}
