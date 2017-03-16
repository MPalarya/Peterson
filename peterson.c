
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>

#define barrier() asm volatile ("" : : : "memory")

#define TRUE	(1)
#define FALSE	(0)

FILE *fp;
#define _CACHE_FLUSH fprintf(fp,"3")

static pthread_t tid[2];

volatile static int cnt = 0;

volatile static int interested[2] = {0, 0};
volatile static int turn = 0;

void acquire (int process) {
	int other = 1 - process;

	interested[process] = TRUE;
	turn = other;

	//barrier();
	asm volatile ("mfence");
	while(interested[other] && turn==other);
}
void release (int process) {
	interested[process] = FALSE;
}

void *run (void *arg) {
	int p = *(int*)arg;
	int i;

	for(i=0;i<5000000;i++){
		acquire(p);
		cnt = cnt + 1;
		release(p);
	}
	return NULL;
}

int main (int argc, char *argv[]) {
	int p[2] = {0,1};

	//fp = fopen ("/proc/sys/vm/drop_caches", "w");
	//if(fp==NULL) { fprintf(stderr, "Permission Denied. Try with sudo.\n"); return 41; }

	if( pthread_create(&tid[0], NULL, &run, &p[0]) ) { fprintf(stderr, "Thread Create failed(1).\n"); return 1; }
	if( pthread_create(&tid[1], NULL, &run, &p[1]) ) { fprintf(stderr, "Thread Create failed(2).\n"); return 2; }

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);	

	printf("cnt : %d\n",cnt);

	//fclose(fp);
	return 0;
}

