#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <stdio.h>

static int signal_entry = 0;
int pom = 0;
#define wait_next_activation pause

void task1(void)
{
  int i,j;
 
  for (i=0; i<4; i++) {
    for (j=0; j<1000; j++) ;
    printf("1");
    fflush(stdout);
  }
}

void task2(void)
{
  int i,j;

  for (i=0; i<6; i++) {
    for (j=0; j<10000; j++) ;
    printf("2");
    fflush(stdout);
  }
}

static void sighand(int s)
{
	pom++;
	if(pom == 24) // 24 % 8 = 0 && 24 % 6 = 0
		pom = 0;
}

int start_periodic_timer(uint64_t offs, int period)
{
    struct itimerval t;

    t.it_value.tv_sec = offs / 1000000;
    t.it_value.tv_usec = offs % 1000000;
    t.it_interval.tv_sec = period / 1000000;
    t.it_interval.tv_usec = period % 1000000;

    signal(SIGALRM, sighand);

    return setitimer(ITIMER_REAL, &t, NULL);
}

int main(int argc, char *argv[])
{
    int res;
    res = start_periodic_timer(2000000, 10000);
    if (res < 0) {
        perror("Start Periodic Timer");
        return -1;
    }

    while(1) {
        wait_next_activation();
        if(pom%8 == 0)
			task1();
		if(pom%6 == 0)
			task2();
    }

    return 0;
}
