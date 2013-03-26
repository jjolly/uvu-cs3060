/* PingPong
   Multi-process signal handling example
   By : John Jolly
   This example will spawn another process and play ping-pong between
   the two processes using signals. The parent will wait for the child
   to send a signal, at which point it will send a signal to the child.
   The reverse happens with the child process. Both processes will
   only exit when the SIGINT is received. */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

/* My two signal handlers. p1 for the parent, p2 for the child. */
void p1sig(int);
void p2sig(int);

/* Main will call the appropriate child or parent function after
   the fork */
void child();
void parent(int);

int main()
{
	int childpid = fork();

	if ( childpid == 0 )
	{
		child();
	}
	else
	{
		parent(childpid);
	}

	return 0;
}

void parent(int childpid)
{
	struct sigaction sa;
	sigset_t sigmask;

	/* First set up the signal handler. We only want to handle the USR1
	   signal */
	sa.sa_handler = p1sig;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGUSR1, &sa, NULL);

	while(1)
	{
		/* The parent waits for the first signal. Let the child serve */
		sigfillset(&sigmask);
		/* Every signal active in the signal mask will be blocked during
		   the suspend. If I want a signal to resume my code, I must
		   delete that signal from the signal mask */
		sigdelset(&sigmask,SIGUSR1);
		/* The signal mask will identify all signals to be blocked for
		   this wait. If I keep the SIGINT blocked in the sigmask, my
		   process will not return if the user presses Ctrl-C at the
		   keyboard. In order to let SIGINT be processed, I must remove
		   it from this mask, or it will be blocked indefinitely. */
		sigdelset(&sigmask,SIGINT);
		/* Two signals are unmasked for this suspend: SIGUSR1 and SIGINT
		   Because SIGUSR1 has a custom signal handler that does not
		   call exit(), sigsuspend will exit on SIGUSR1.
		   Because SIGINT is not handled, the default handler will call
		   the exit() function, and sigsuspend will never return. */
		sigsuspend(&sigmask);

		/* A signal has been received by the parent from the child.
		   Now it's time to volley back. Sleep a second to allow the
		   child to get set up to receive */
		sleep(1);
		kill(childpid, SIGUSR1);
	}
}

void child()
{
	struct sigaction sa;
	sigset_t sigmask;

	/* First set up the signal handler. We only want to handle the USR1
	   signal */
	sa.sa_handler = p2sig;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGUSR1, &sa, NULL);

	while(1)
	{
		/* Child serves first */
		sleep(1);
		kill(getppid(), SIGUSR1);

		/* Now wait for the parent to return */
		sigfillset(&sigmask);
		sigdelset(&sigmask,SIGUSR1);
		sigdelset(&sigmask,SIGINT);
		sigsuspend(&sigmask);
	}
}

void p1sig(int signo)
{
	fprintf(stderr, "Ping...\n");
}

void p2sig(int signo)
{
	fprintf(stderr, "Pong...\n");
}
