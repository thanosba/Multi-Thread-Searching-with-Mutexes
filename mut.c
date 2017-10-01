#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#define NUM_THREADS 5
void *search(void*);
void *print_it(void*);

pthread_t threads[NUM_THREADS];
pthread_mutex_t lock;
int tries;

int main(argc, argv)
int argc;
char **argv;
{
    int i;
    int pid;
    pid=getpid();
    printf("Search number=%d...\n", pid);
    pthread_mutex_int(&lock, NULL);
    for(i=0; i<NUM_THREADS;i++)
        pthread_create(&threads[i],NULL,search,(void*)pid);
    for(i=0; i<NUM_THREADS;i++)
        pthread_join(threads[i], NULL);
    printf("%d.\n, tries");
    return 0;
    }
void print_it(void*arg)
{
    int *try=(int*)arg;
    pthread_t tid;
    tid=pthread_self();
    printf("Thread Cancel %x %d\n",tid,*try);
}
void *search(void *arg)
{
    int num=(int) arg;
    int i,j, ntries;
    pthread_t tid;
    tid=pthread_self();
    pthread_mutex_lock(&lock);
    srand((int)tid);
    i=rand()& 0xFFFFFF;
    pthread_mutex_unlock(&lock);
    ntries=0;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
    pthread_cleanup_push(print_it,(void*)&ntries);
    while(1){
        i=(i+1)& 0xFFFFFF;
        ntries++;
        if(num==i){
            while (pthread_mutex_trylock(&lock)==EBUSY)
                pthread_testcancel();
            tries=ntries;
            printf("Tread %lx find the number!\n"tid);
            for (j=0;j<NUM_THREADS;j++)
                if(threads[j]!=tid) pthread_cancel(threads[j]);
            break;
        }
        if (ntries%100 ==0){
            pthread_testcancel();
        }  
    }
    pthread_cleanup_pop(0);
    return((void*)0);
}


