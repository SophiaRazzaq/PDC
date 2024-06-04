#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main() {
  int noProcess;
  int source;
  printf("==============================================\n");
  printf("Enter the number of processes (power of 2): ");
  scanf("%d", &noProcess);

  printf("==============================================\n");
  printf("Enter the source node (0 to %d): ", noProcess - 1);
  scanf("%d", &source);
  printf("==============================================\n");

  //handling exceptions
  if (source >= noProcess || source < 0) {
    printf("Invalid source node.\n");
    return 1;
  } 
  if (noProcess & (noProcess - 1)) {
    printf("Number of processes must be a power of 2.\n");
    return 1;
  }



  
  int dimensions = log2(noProcess);
  int message = 1;
  int received[noProcess];
  memset(received, 0, sizeof(received));

  // Broadcast message from source node
  for (int i = 0; i < dimensions; i++) {
    int mask = 1 << i;
    for (int j = 0; j < noProcess; j++) {
      if ((j ^ source) & mask) {
        if (j & mask) {
          received[j] = 1;
          printf("Process %d || received message from process %d.\n", j, j ^ mask );
          
        } else {
          printf("Process %d || sent message to process %d.\n", j, j ^ mask);
        }
      }
    }
  }

  

  printf("==============================================\n");
  printf("Broadcast successful. :)\n");
  printf("==============================================\n");
  return 0;
}
