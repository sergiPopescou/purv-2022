//arm-linux-gnueabihf-gcc periodic_task_posix_timer.c -std=gnu99 -Wall -o periodic_task_posix_timer
//gcc -Wall periodic_task_posix_timer.c -lrt -o naziv_izlaznog_fajla
// -lrt linkuje librt biblioteku u kojoj se nalaze timer_create i timer_settime
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>


static sigset_t sigset;

static int ctrl_var;
#define CTRL_MAX 4802

static void wait_next_activation(void)
{
    int dummy;
   if(ctrl_var==CTRL_MAX){
       ctrl_var=2;
   }
   ctrl_var+=2;
    sigwait(&sigset, &dummy);
}
int start_periodic_timer(uint64_t offs, int period)
{
    struct itimerspec t;
    struct sigevent sigev;
    timer_t timer;
    const int signal = SIGALRM;
    int res;

    t.it_value.tv_sec = offs / 1000000;
    t.it_value.tv_nsec = (offs % 1000000) * 1000;
    t.it_interval.tv_sec = period / 1000000;
    t.it_interval.tv_nsec = (period % 1000000) * 1000;
	
	// ovaj dio ostaje isti
    sigemptyset(&sigset);
    sigaddset(&sigset, signal);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

	// alociranje i popunjavanje sigevent strukture
    memset(&sigev, 0, sizeof(struct sigevent));
    sigev.sigev_notify = SIGEV_SIGNAL; // kada vrijeme istekne 
									   // okinuce se signal, jer se ostatak
									   // koda oslanja na signale
    sigev.sigev_signo = signal;	// koji signal? pa SIGALRM, naravno
	
	// kreiranje tajmera
    res = timer_create(CLOCK_MONOTONIC, &sigev, &timer);
    if (res < 0) {
        perror("Timer Create");

	exit(-1);
    }
	// pokretanje tajmera 
    return timer_settime(timer, 0 /*TIMER_ABSTIME*/, &t, NULL);
}

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

int main(int argc, char *argv[])
{
    int res;

    res = start_periodic_timer(1000000, 20000);
    if (res < 0) {
        perror("Start Periodic Timer");

        return -1;
    }

    while(1) {
        wait_next_activation();
        if(ctrl_var%6==0)
            task1();
        if(ctrl_var%8==0)
            task2();
    }

    return 0;
}
