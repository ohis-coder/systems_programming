#include <stdio.h>

int main() {
  struct Alligned_Bytes {
    int balance;
    long guess;
    char pad[44];
  };

  int size = sizeof(struct Alligned_Bytes);
  printf("The size of the struct is %d", size);
  return 0;
}
