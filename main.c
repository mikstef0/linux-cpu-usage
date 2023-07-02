#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "global.h"
#include "cut_queue.h"

void term(void);

short int terminate; // SIGTERM/SIGINT catched-signalisation value
struct sigaction sa_term; // signal handling struct for SIGTERM
struct sigaction sa_int; // signal handling struct for SIGINT

pthread_t reader_thr, analyzer_thr, printer_thr, watchdog_thr, logger_thr;


int main()
{
    terminate=0;
    procno = sysconf(_SC_NPROCESSORS_ONLN);

    rd=(struct procdata_struct*)malloc((1+procno)*sizeof(*rd)); // memory for data read from /proc/stat
    rd_old=(struct procdata_struct*)malloc((1+procno)*sizeof(*rd_old)); // memory for previous values of above
    CPU_Percentage=(float*)malloc((1+procno)*sizeof(CPU_Percentage));

    data_queue=(dq*)malloc((1+procno)*sizeof(*data_queue));
    proc_queue=(pq*)malloc((1+procno)*sizeof(*proc_queue));

    int thr_ret=1;

    sa_term.sa_handler=(void(*)(int))term;
    sigaction(SIGTERM, &sa_term, NULL);
    sa_int.sa_handler=(void(*)(int))term;
    sigaction(SIGINT, &sa_int, NULL);

    if(pthread_create(&watchdog_thr, NULL, (void *(*)(void *))watchdog_thread, NULL))
    {
    fprintf(stderr, "Error creating watchdog thread! %d", thr_ret);
    raise(SIGTERM);
    }

    if(pthread_create(&analyzer_thr, NULL, (void *(*)(void *))analyzer_thread, NULL))
    if(thr_ret!=0)
    {
        fprintf(stderr, "Error creating analyzer thread! %d", thr_ret);
        raise(SIGTERM);
    }

    if(pthread_create(&printer_thr, NULL, (void *(*)(void *))printer_thread, NULL))
    if(thr_ret!=0)
    {
        fprintf(stderr, "Error creating printer thread! %d", thr_ret);
        raise(SIGTERM);
    }

    if(pthread_create(&reader_thr, NULL, (void *(*)(void *))reader_thread, NULL))
    if(thr_ret!=0)
    {
        fprintf(stderr, "Error creating reader thread! %d", thr_ret);
        raise(SIGTERM);
    }

    while(terminate==0)
    {
    sleep(1);
    }

    return 0;
}


void term()
{
    terminate=1;
    fprintf(stdout, "\nSIGTERM/SIGINT encountered. Terminating...\n");
    pthread_cancel(reader_thr);
    pthread_cancel(analyzer_thr);
    pthread_cancel(printer_thr);
    pthread_cancel(watchdog_thr);
    pthread_join(reader_thr, NULL);
    pthread_join(analyzer_thr, NULL);
    pthread_join(printer_thr, NULL);
    pthread_join(watchdog_thr, NULL);

    free(rd);
    free(rd_old);
    free(CPU_Percentage);
    free(data_queue);
    free(proc_queue);

    fprintf(stdout, "Program has terminated.\n");
    exit(0);
}
