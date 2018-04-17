
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
int concat(int x, int y){
    char str1[20];
    char str2[20];

    sprintf(str1,"%d",x);
    sprintf(str2,"%d",y);

    strcat(str1,str2);

    return atoi(str1);
}

int main(int argc, char** argv) {
  int msg[2], inmsg[2];
  int temp;
  MPI_Status stat;
  MPI_Init(&argc, &argv);
  int terminated = 0;
  int world_rank;
  MPI_Status Stat;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

// generate the first two numbers and make first send.
    if (world_rank == 0){
      if (terminated == 0){
      //srand(time(NULL)*world_rank);
      srand(time(NULL)*world_size);
      msg[0] = (rand() % 100) + 10;
      //printf("i am msg0 %d\n", msg[0]);
      if (msg[0] >= 100){
        msg[0] = 100 % 100;
      } else if (msg[0] < 10){
        msg[0] += 10;
      }

      msg[0] = concat(10+world_rank, msg[0]);
      //printf("i am msg0 after %d \n", msg[0]);


      if ((msg[0] % 2) == 0){
        msg[1] = 1;
      } else {
        msg[1] = 0;
      }
      terminated = 1;
      }
    MPI_Send(&msg, 2, MPI_INT, (world_rank + 1) % world_size,
             0, MPI_COMM_WORLD);
    printf("i am process 0 and sending %d to process 1 \n", msg[0]);
    }

    if (world_rank != 0) {
      MPI_Recv(&inmsg, 2, MPI_INT, world_rank - 1, 0,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("Process %d received number %d and number %d from process %d\n",
             world_rank, inmsg[0], inmsg[1], world_rank - 1);
        srand(time(NULL)*world_rank);
        msg[0] = (rand() % 100) + 10 ;

        if (msg[0] < 10 ){
          msg[0] += 10;
        }
        if (msg[0] == 100){
          msg[0] = msg[0] % 100;
        }
        msg[0] = concat((10+world_rank), msg[0] );
        printf("i am process number %d and these are my numbers [%d, %d]\n",world_rank, msg[0],msg[1]);
        //sorting the arrays, first element is even, second element is odd.


        if ((msg[0] % 2) != 0){
          temp = msg[1];
          msg[1] = msg[0];
          msg[0] = temp;
        }
        if((inmsg[0] % 2) != 0){
          temp = inmsg[1];
          inmsg[1] = inmsg[0];
          inmsg[0] = temp;
        }
        printf("i am msg after sorting [%d,%d]\n", msg[0], msg[1]);
        printf("i am inmsg after sorting [$%d,%d]\n", inmsg[0], inmsg[1]);
        if ((msg[0] % 2) == 0 && msg[0] != 0){
          printf("Comparing even number %d with even number %d\n", msg[0], inmsg[0]);
          if ((msg[0] % 100) > (inmsg[0] % 100)){
            printf("%d is bigger than %d, Swapping!\n", msg[0], inmsg[0]);
            inmsg[0] = msg[0];
          }
        } else {
          printf("comparing odd number %d with the odd number %d\n", msg[1], inmsg[1]);
          if((msg[1] % 100) > (inmsg[1] % 100)){
            printf("%d is bigger than %d, Swapping!\n", msg[1], inmsg[1]);
            inmsg[1] = msg[1];

          }
        }




        MPI_Send(&inmsg, 2, MPI_INT, (world_rank + 1) % world_size,
                 0, MPI_COMM_WORLD);
        printf("Process %d is sending %d and %d to process %d\n",world_rank, inmsg[0], inmsg[1], world_rank + 1);


      }
    if (world_rank == 0) {
        MPI_Recv(&inmsg, 2, MPI_INT, world_size - 1, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d declared value %d a an even leader, and value %d a an odd leader \n", world_rank, inmsg[0],inmsg[1]);
    }

    MPI_Finalize();
  }
