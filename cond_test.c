#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"
#include "spinlock.h"

int stdout = 1;

void
cond_test(int cond) {
  int ticket;
  if ( fork() != 0)
    return;
  // i'm the child
  // now take a ticket
  ticket = cond_get(cond);
  printf (stdout, "My ticket is: %d\n", ticket);
  cond_wait(cond);
  printf (stdout,"I have been waked up, my pid is %d and my ticket is %d\n", getpid(), ticket);
  sleep(20);
  cond_wait(cond);
  printf (stdout, "Bye %d\n", getpid());
  exit();
}


int
main(int argc, char *argv[])
{
  int cond, i;
  cond = cond_alloc();
  cond_set(cond, 10);
  printf (stdout, "%d\n",getpid());
  while (cond_get(cond) > 0) {
    cond_test(cond);
    sleep(10);
    printf (stdout, "decresing cond val\n");
    cond_set(cond, cond_get(cond)-1);
  }
  sleep(10);
  cond_broadcast(cond);
  sleep(50);
  for (i = 0; i <10; i++) {
    cond_signal (cond);
  } 
  printf (stdout, "the cond val is %d\n", cond_get(cond));
  printf (stdout, "main process exit\n");
  exit();
}
