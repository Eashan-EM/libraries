#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "Comms.h"

struct CommsData {
  unsigned char to;
  unsigned char from;
  unsigned short func;
  unsigned int value;
} toRead, toWrite;

struct CommsHeader {
  unsigned char beingUsed;
  unsigned short max;
  unsigned short current;
  unsigned remaining: 16;
};

union CommsFirstLine {
  struct CommsData data;
  struct CommsHeader header;
} CommsFirstLine;

struct Comms {
  unsigned int lastRead;
  struct CommsData *memory;
  char memoryValid; // If file was actualy created
  unsigned char created; // whether /comms file was created in this instance
} Comms;

#define NUM 128
#define SIZE NUM*sizeof(struct CommsData)

void writeHeader();
void readHeaderNoBlock();

callback ToCallbacks[sizeof(unsigned char)];
callback FromCallbacks[sizeof(unsigned char)];

void setup() {
  Comms.lastRead = 0;
  Comms.memoryValid = 1;
  if (Comms.created) {
    CommsFirstLine.header.max = NUM;
    CommsFirstLine.header.current = 1;
    CommsFirstLine.header.beingUsed = 0; 
    writeHeader();
  } else
    readHeaderNoBlock();
}

int comms_open() {
  int fd = shm_open("/comms", O_CREAT | O_EXCL | O_RDWR, 0666);
  if (fd==-1)
    return 1;
  ftruncate(fd, SIZE);
  Comms.memory = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  Comms.created = 1;
  close(fd);
  setup();
  return 0;
}

void test() {
  printf("%ld\n%ld\n", sizeof(struct CommsHeader), sizeof(struct CommsData));
}
int comms_connect() {
  int fd = shm_open("/comms", O_RDWR, 0666);
  if (fd==-1)
    return 1;
  Comms.memory = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  Comms.created = 0;
  close(fd);
  setup();
  return 0;
}

int comms_close() {
  if (Comms.memoryValid) {
    munmap(Comms.memory, SIZE);
    if (Comms.created) {
      shm_unlink("/comms");
      Comms.created = 0;
    }
    return 0;
  }
  return 1;
}

int getNext() {
  int current = CommsFirstLine.header.current;
  int next = (current+1)%CommsFirstLine.header.max;
  if (next==0)
    next = 1;
  CommsFirstLine.header.current = next;
  return current;
}

int getNextForCurrent() {
  int curr = Comms.lastRead;
  curr = (curr+1)%CommsFirstLine.header.max;
  if (curr==0)
    curr = 1;
  return curr;
}

void writeHeader() {
  CommsFirstLine.header.beingUsed = 0;
  Comms.memory[0] = CommsFirstLine.data;
}

void readHeaderBlock() {
  do {
    CommsFirstLine.data = Comms.memory[0];
  } while (CommsFirstLine.header.beingUsed);
  CommsFirstLine.header.beingUsed = 1;
  writeHeader();
}

void readHeaderNoBlock() {
  CommsFirstLine.data = Comms.memory[0];
}

void comms_send(char to, char from, unsigned short function, unsigned int value) {
  toWrite.to = to;
  toWrite.from = from;
  toWrite.func = function;
  toWrite.value = value;
  readHeaderBlock();
  Comms.memory[getNext()] = toWrite;
  writeHeader();
}

void comms_read() {
  readHeaderNoBlock();
  while (getNextForCurrent()!=CommsFirstLine.header.current) {
    Comms.lastRead = getNextForCurrent();
    toRead = Comms.memory[Comms.lastRead];
    if (ToCallbacks[toRead.from]!=NULL) {
      ToCallbacks[toRead.from](toRead.to, toRead.from, toRead.func, toRead.value);
    }
    if (FromCallbacks[toRead.from]!=NULL) {
      FromCallbacks[toRead.from](toRead.to, toRead.from, toRead.func, toRead.value);
    }
  }
}

void comms_subscribe_to(unsigned char to, callback func) {
  ToCallbacks[to] = func;
}

void comms_subscribe_from(unsigned char from, callback func) {
  FromCallbacks[from] = func;
}