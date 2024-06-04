#include <stdio.h>

void broadcast(int source, int nodes)
{
  int data = 0; 
  if (nodes == 2) {
    printf("Node %d sends data to Node %d\n", source, (source + 1) % nodes);
    
    printf("Node %d receives data from Node %d\n", (source + 1) % nodes,source);
    
  } 


    
  
  else 
  {
    int mirror_node = source + nodes / 2;
    if (mirror_node >= nodes)
    {
      mirror_node -= nodes;
    }
    printf("Node %d sends data to Node %d\n", source, mirror_node);
    broadcast(mirror_node, nodes / 2);

    int second_half_start = (source + nodes / 2 + 1) % nodes;
    printf("Node %d sends data to Node %d\n", source, second_half_start);
    broadcast(source, nodes / 2);
  }
}

int main() {
  int source, nodes;

  printf("==============================================\n");
  printf("Enter the source node: ");
  scanf("%d", &source);

  printf("==============================================\n");
  printf("Enter the number of nodes: ");
  scanf("%d",&nodes);
  broadcast(source, nodes);
  


  printf("==============================================\n");
  printf("Broadcast successful. :)\n");
  printf("==============================================\n");
}
