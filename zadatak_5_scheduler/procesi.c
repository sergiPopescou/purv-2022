#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include<sched.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

static sigset_t sigset;
static pid_t child1,child2,child3;

void kill_handler(int signo, siginfo_t *info, void *context){
  if(signo==SIGINT){
    kill(child1,SIGSTOP);
    kill(child2,SIGSTOP);
    kill(child3,SIGSTOP);
  }
}
// periodicni zadatak na 40 milisekundi
void task1(void)
{
  int i, j;

  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 1000; j++)
      ;
    printf("1");
    fflush(stdout);
  }
}

// periodicni zadatak na 80 milisekundi
void task2(void)
{
  int i, j;

  for (i = 0; i < 6; i++)
  {
    for (j = 0; j < 10000; j++)
      ;
    printf("2");
    fflush(stdout);
  }
}

// periodicni zadatak na 120 milisekundi
void task3(void)
{
  int i, j;

  for (i = 0; i < 6; i++)
  {
    for (j = 0; j < 100000; j++)
      ;
    printf("3");
    fflush(stdout);
  }
}

static void wait_next_activation(void)
{
  int dummy;

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
  sigev.sigev_signo = signal;        // koji signal? pa SIGALRM, naravno

  // kreiranje tajmera
  res = timer_create(CLOCK_MONOTONIC, &sigev, &timer);
  if (res < 0)
  {
    perror("Timer Create");

    exit(-1);
  }
  // pokretanje tajmera
  return timer_settime(timer, 0 /*TIMER_ABSTIME*/, &t, NULL);
}

void print_child_status (int status) {
	if (WIFEXITED (status)) {
		printf("Child exited with status %d\n", WEXITSTATUS (status));
	} else if (WIFSTOPPED (status)) {
		printf("Child stopped by signal %d (%d)\n", WSTOPSIG (status),
	    strsignal(WSTOPSIG (status)));
	} else if (WIFSIGNALED (status)) {
		printf("Child killed by signal %d (%d)\n", WTERMSIG (status),
	    strsignal(WTERMSIG (status)));
	} else {
		printf("Unknown child status\n");
	}
}

int main(int argc, char *argv[])
{
  int status1=0,status2=0,status3=0;
  child1 = fork();
  if (child1 == 0)// First child proces task3
  {
    int pmin = sched_get_priority_min(SCHED_FIFO);
		struct sched_param param; 
		param.sched_priority = pmin; 
		sched_setscheduler(0, SCHED_FIFO, &param);
    int res;

    res = start_periodic_timer(1000000, 120000);
    if (res < 0)
    {
      perror("Start Periodic Timer");

      return -1;
    }

    while (1)
    {
      wait_next_activation();
      task3();
    }

    return 3;
  }



  child2 = fork();
  if (child2 == 0)// Second child process task2
  {

    int pmin = sched_get_priority_min(SCHED_FIFO);
		struct sched_param param; 
		param.sched_priority = pmin + 10; 

		sched_setscheduler(0, SCHED_FIFO, &param);
    int res;

    res = start_periodic_timer(1000000, 80000);
    if (res < 0)
    {
      perror("Start Periodic Timer");

      return -1;
    }

    while (1)
    {
      wait_next_activation();
      task2();
    }

    return 2;
  }

  child3=fork();
  if(child3==0){
    // Third child process task1
  int pmin = sched_get_priority_min(SCHED_FIFO);
	struct sched_param param; 
	param.sched_priority = pmin+20; 
	sched_setscheduler(0, SCHED_FIFO, &param);

  int res;

  res = start_periodic_timer(1000000, 40000);
  if (res < 0)
  {
    perror("Start Periodic Timer");

    return -1;
  }

  while (1)
  {
    wait_next_activation();
    task1();
  }
  return 1;
  }
  // parent process task1
  struct sigaction act;
  memset(&act,0,sizeof(act));
  act.sa_sigaction=kill_handler;
  act.sa_flags=SA_SIGINFO;
  sigaction(SIGINT,&act,NULL);
  pause();
  waitpid(child1,&status1,WNOHANG);
  waitpid(child2,&status2,WNOHANG);
  waitpid(child3,&status3,WNOHANG);
  printf("\nCHILD[1]\n");
  print_child_status(status1);
  printf("\nCHILD[2]\n");
  print_child_status(status2);
  printf("\nCHILD[3]\n");
  print_child_status(status3);
  return 0;
}
