/*
 * Malloc test
 * (C) Copyright 2014  Jipan Yang
 *  Jipan.Yang@gmail.com
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.
 */

#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>

//32k bytes
#define MALLOC_BLOCK_SIZE	(1<<16)
//Number of allocation blocks
#define MALLOC_BLOCK_NUM	(1<<2)  

//4Mbyte
#define MALLOC_LARGE_BLOCK_SIZE	(1<<22)
//Number of large allocation blocks
#define MALLOC_LARGE_BLOCK_NUM	(1<<0)

//a pthread mutex may only be unlocked by the thread that locked it.
//so semaphore is used here as signaling mechanism
sem_t mutex; // declaration of mutex

static void
display_mallinfo(void)
{
    struct mallinfo mi;

    mi = mallinfo();

    printf("\nTotal non-mmapped bytes (arena):       %d\n", mi.arena);
    printf("# of free chunks (ordblks):            %d\n", mi.ordblks);
    printf("# of free fastbin blocks (smblks):     %d\n", mi.smblks);
    printf("# of mapped regions (hblks):           %d\n", mi.hblks);
    printf("Bytes in mapped regions (hblkhd):      %d\n", mi.hblkhd);
    printf("Max. total allocated space (usmblks):  %d\n", mi.usmblks);
    printf("Free bytes held in fastbins (fsmblks): %d\n", mi.fsmblks);
    printf("Total allocated space (uordblks):      %d\n", mi.uordblks);
    printf("Total free space (fordblks):           %d\n", mi.fordblks);
    printf("Topmost releasable block (keepcost):   %d\n", mi.keepcost);
    
    printf("------ malloc_stats -------\n");
    malloc_stats();	
}



static void *
chunk_malloc (size_t size, void *user_ptr )
{
    return malloc( size);
}

static void *
thread_start(void *arg)
{
	unsigned int i = 0;
	char *p;

	for(i=0; i<MALLOC_BLOCK_NUM*2; i++) {
                p= chunk_malloc(MALLOC_BLOCK_SIZE, NULL);
                if(p==NULL)
                        break;
        }
        printf("Child thread mallocated %u bytes in block size of %d\n", (i*MALLOC_BLOCK_SIZE), MALLOC_BLOCK_SIZE);
	sem_post(&mutex);
	sleep(10000);
	return NULL;
}


int main(void) 
{
	pthread_t thread;
	unsigned int i = 0;
	char *p;
	int ret;

	sem_init(&mutex, 0, 0);	

	ret = pthread_create(&thread, NULL,
                                  &thread_start, NULL);
        if (ret != 0)
                perror("pthread_create"); 	

	for(i=0; i<MALLOC_BLOCK_NUM; i++) {
		p= chunk_malloc(MALLOC_BLOCK_SIZE, NULL);
		if(p==NULL)
			break;
	}
	printf("Main thread mallocated %u bytes in block size of %d\n", (i*MALLOC_BLOCK_SIZE), MALLOC_BLOCK_SIZE);

	//wait for child thread to finish allocation
	sem_wait(&mutex);
	sem_destroy(&mutex); //done with its mission

	for(i=0; i<MALLOC_LARGE_BLOCK_NUM; i++) {
                p= chunk_malloc(MALLOC_LARGE_BLOCK_SIZE, NULL);
                if(p==NULL)
                        break;
        }
	printf("Main thread mallocated %u bytes in block size of %d\n", (i*MALLOC_LARGE_BLOCK_SIZE), MALLOC_LARGE_BLOCK_SIZE);	
		
	display_mallinfo();
	sleep(10000);
	return 0;
}
