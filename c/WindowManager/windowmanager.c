#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "windowmanager.h"

static struct WinMan {
  int read_fd;
  int write_fd;
} winman;

static struct addr {
  sa_family_t family;
  char path[108];
} addr_read, addr_write;

int winman_setup() {
  char socket_addr[150] = {'\0'};
  addr_read.family = addr_write.family = AF_UNIX;

  char *tmp = getenv("XDG_RUNTIME_DIR");
  if (tmp==NULL) {
    winman.read_fd = -1;
    return -1;
  }
  strcat(socket_addr, tmp);
  strcat(socket_addr, "/hypr/");
  tmp = getenv("HYPRLAND_INSTANCE_SIGNATURE");
  if (tmp==NULL) {
    winman.read_fd = -1;
    return -1;
  }
  strcat(socket_addr, tmp);
  strcpy(addr_read.path, socket_addr);
  strcat(addr_read.path, "/.socket2.sock");
  strcpy(addr_write.path, socket_addr);
  strcat(addr_write.path, "/.socket.sock");

  errno = 0;
  int val = socket(AF_UNIX, SOCK_STREAM, 0);
  if (val==-1) {
    winman.read_fd = -1;
    return -1;
  }
  winman.read_fd = val;
  int read_flags = fcntl(winman.read_fd, F_GETFL);
  read_flags |= O_NONBLOCK;
  fcntl(winman.write_fd, F_SETFL, read_flags);
  
  val = socket(AF_UNIX, SOCK_STREAM, 0);
  if (val==-1) {
    winman.write_fd = -1;
    return -1;
  }
  winman.write_fd = val;

  errno = 0;
  if (connect(winman.read_fd, (struct sockaddr *)&addr_read, sizeof(addr_read))==-1) {
    winman.read_fd = -1;
    return -1;
  }
  /*
  if (connect(winman.write_fd, (struct sockaddr *)&addr_write, sizeof(addr_write))==-1) {
    winman.write_fd = -1;
    return -1;
  }
  */

  if (hyprland_setup()==-1)
    return -1;
  return 0;
}

int winman_update() {
  int events_called = 0;
  char buf;
  if (winman.read_fd!=-1)
    recv(winman.read_fd, &buf, 1, MSG_WAITALL);
  return events_called;
}

int winman_destroy() {
  if (winman.read_fd!=-1)
    close(winman.read_fd);
  if (winman.write_fd!=-1)
    close(winman.write_fd);
}