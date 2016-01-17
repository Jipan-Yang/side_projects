#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>

#include <dmalloc.h>

#define THREAD_NUM 32

pthread_mutex_t __big_lock;

typedef int(* _Function )(void);

typedef struct {
  char *cmd;                    /* user command input. */
  _Function func;               /* Function to call to do the job. */
  char *desc;                   /* description of the command.  */
} COMMAND;

COMMAND commands[];

int
cmd_show_dmalloc_info(void)
{
	static unsigned long mark;
	time_t timer;
	char buffer[256];
	struct tm* tm_info;

	time(&timer);
	tm_info = localtime(&timer);
	strftime(buffer, sizeof(buffer), "%Y:%m:%d %H:%M:%S", tm_info);
	
	dmalloc_log_stats();
	//dmalloc_log_unfreed();
	/*
	 * void dmalloc_log_changed
	 *
	 * DESCRIPTION:
	 *
	 * Dump the pointers that have changed since a point in time.
	 *
	 * RETURNS:
	 *
	 * mark -> Sets the point to compare against.  You can use
	 * dmalloc_mark to get the current mark value which can later be
	 * passed in here.  Pass in 0 to log what has changed since the
	 * program started.
	 *
	 * not_freed_b -> Set to 1 to log the new pointers that are non-freed.
	 *
	 * free_b -> Set to 1 to log the new pointers that are freed.
	 *
	 * details_b -> Set to 1 to dump the individual pointers that have
	 * changed otherwise the summaries will be logged.
	 */	
	dmalloc_log_changed(mark, 1, 0, 0);
	mark = dmalloc_mark();
	printf("dmalloc stats dumped at %s  timestamp %ld\n", buffer, (long)time(NULL));
	return 0;
}

static int
cmd_help (void)
{
	int i;

	for (i = 0; commands[i].cmd; i++) {
    		printf ("%s\t\t%s.\n", commands[i].cmd, commands[i].desc);
	}
	return 0;
}

void* dummy_task(void *arg) {
    char *p;
	
	printf("tid = 0x%lx\n", (long)pthread_self());
	
	while(1) {
    	int size = rand()%(1024*1024);
    	int not_leak;

    	p = malloc(size);

		assert(p!=NULL);
    	usleep(rand()%100000);
//pthread_mutex_lock(&__big_lock);
		
		// leak memory in probability of 1/100
		not_leak = rand()%100;
		if (not_leak) {
    		free(p);
    	}
//pthread_mutex_unlock(&__big_lock);
    }
}

COMMAND commands[] = {
  { "dump", cmd_show_dmalloc_info, "dump dmalloc stats" },
  { "help", cmd_help, "Display commands help" },
  { (char *)NULL, (_Function)NULL, (char *)NULL }
};


int 
main(int argc, char **argv)
{
	pthread_t 	tid;
	int i, err;	

	pthread_mutex_init(&__big_lock, NULL);

	for(i=0; i<THREAD_NUM; i++) {
		err = pthread_create(&tid, NULL, dummy_task, NULL);
		if (err) {
        		printf( "Failed to create thread\n");
        		return -1;
    		}	
    		printf( "Thread 0x%lx created\n", (long)tid);
	}
	sleep(1);
	for(;;) {
		char* line;
		
		// readline allocates space for returned string
		line = readline("\ntype a command:  "); 
       		if(line != NULL) { 
           		printf("You entered: %s\n", line);
           		for (i = 0; commands[i].cmd; i++) {
				if (strcasecmp (line, commands[i].cmd) == 0) {
      					err = commands[i].func();
					//ignoring err processing
      				}
			}
      		}

		free(line);   //freeing the readline space
	}
	
	return 0;
}
