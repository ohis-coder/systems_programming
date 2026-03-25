#include <fcntl.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define FAILED -1
#define STRIDE_PHONE 8
#define STRIDE_NAME 16
#define STRIDE_ADDR 128
#define PAGE_SIZE 4096

int main() {
  int fd_name = open("names.bin", O_RDWR | O_CREAT, 0666);
  int fd_phone = open("phone.bin", O_RDWR | O_CREAT, 0666);
  int fd_address = open("address.bin", O_RDWR | O_CREAT, 0666);

  if (ftruncate(fd_name, PAGE_SIZE) == FAILED) {
    perror("ftruncate failed");
    close(fd_name);
  }
  if (ftruncate(fd_phone, PAGE_SIZE) == FAILED) {
    perror("ftruncate failed");
    close(fd_phone);
  }
  if (ftruncate(fd_address, PAGE_SIZE) == FAILED) {
    perror("ftruncate failed");
    close(fd_address);
  }

  char *user_name =
      mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_name, 0);
  char *user_phone =
      mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_phone, 0);
  char *user_address =
      mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_address, 0);

  if (user_name == MAP_FAILED) {
    perror("mmap");
  }
  if (user_phone == MAP_FAILED) {
    perror("mmap");
  }
  if (user_address == MAP_FAILED) {
    perror("mmap");
  }
  return 0;
}
