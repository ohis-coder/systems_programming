#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define FAILED -1
#define STRIDE_ID 8
#define STRIDE_PHONE 16
#define STRIDE_NAME 64
#define STRIDE_ADDR 128
#define TOTAL_USERS 256
#define ID_PAGE_SIZE STRIDE_ID *TOTAL_USERS
#define NAME_PAGE_SIZE STRIDE_NAME *TOTAL_USERS
#define PHONE_PAGE_SIZE STRIDE_PHONE *TOTAL_USERS
#define ADDRESS_PAGE_SIZE STRIDE_ADDR *TOTAL_USERS

int id = 0;

void write_to_db(char *base_pointer, int id, int stride) {
  int c;
  memset(&base_pointer[id * stride], '\0', stride);
  int length_of_user_input = 0;
  while ((c = getchar()) != EOF && c != '\n') {
    if (length_of_user_input < stride - 1) {
      base_pointer[(id * stride) + length_of_user_input] = c;
      ++length_of_user_input;
    }
  }
}

int main() {
  int fd_name = open("names.bin", O_RDWR | O_CREAT, 0666);
  int fd_phone = open("phone.bin", O_RDWR | O_CREAT, 0666);
  int fd_address = open("address.bin", O_RDWR | O_CREAT, 0666);
  int fd_id = open("id.bin", O_RDWR | O_CREAT, 0666);

  if (ftruncate(fd_name, NAME_PAGE_SIZE) == FAILED) {
    perror("ftruncate failed");
    close(fd_name);
    exit(1);
  }
  if (ftruncate(fd_phone, PHONE_PAGE_SIZE) == FAILED) {
    perror("ftruncate failed");
    close(fd_phone);
    exit(1);
  }
  if (ftruncate(fd_address, ADDRESS_PAGE_SIZE) == FAILED) {
    perror("ftruncate failed");
    close(fd_address);
    exit(1);
  }
  if (ftruncate(fd_id, ID_PAGE_SIZE) == FAILED) {
    perror("ftruncate failed");
    close(fd_id);
    exit(1);
  }

  char *user_name = mmap(NULL, NAME_PAGE_SIZE, PROT_READ | PROT_WRITE,
                         MAP_SHARED, fd_name, 0);
  char *user_phone = mmap(NULL, PHONE_PAGE_SIZE, PROT_READ | PROT_WRITE,
                          MAP_SHARED, fd_phone, 0);
  char *user_address = mmap(NULL, ADDRESS_PAGE_SIZE, PROT_READ | PROT_WRITE,
                            MAP_SHARED, fd_address, 0);
  long *user_id =
      mmap(NULL, ID_PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_id, 0);

  if (user_name == MAP_FAILED) {
    perror("mmap");
  }
  if (user_phone == MAP_FAILED) {
    perror("mmap");
  }
  if (user_address == MAP_FAILED) {
    perror("mmap");
  }
  if (user_id == MAP_FAILED) {
    perror("mmap");
  }

  long current_id = *user_id;
  printf("Your user id/position is: %ld\n", current_id);
  printf("Enter your name\n");
  write_to_db(user_name, id, STRIDE_NAME);
  printf("Enter your phone number\n");
  write_to_db(user_phone, id, STRIDE_NAME);
  printf("Enter your address\n");
  write_to_db(user_address, id, STRIDE_NAME);

  *user_id = current_id + 1;
  if (munmap(user_name, NAME_PAGE_SIZE) == FAILED) {
    perror("munmap failed");
  }
  if (munmap(user_phone, NAME_PAGE_SIZE) == FAILED) {
    perror("munmap failed");
  }
  if (munmap(user_address, NAME_PAGE_SIZE) == FAILED) {
    perror("munmap failed");
  }
  if (munmap(user_id, ID_PAGE_SIZE) == FAILED) {
    perror("munmap failed");
  }
  return 0;
}
