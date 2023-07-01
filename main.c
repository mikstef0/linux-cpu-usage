#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "main.h"
#include "global.h"

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

    fprintf(stdout, "Program has terminated.\n");
}


int main()
{
    terminate=0;
    procno = sysconf(_SC_NPROCESSORS_ONLN);

    rd=(struct procdata_struct*)malloc((1+procno)*sizeof(*rd)); 
    rd_old=(struct procdata_struct*)malloc((1+procno)*sizeof(*rd_old));
    CPU_Percentage=(float*)malloc((1+procno)*sizeof(CPU_Percentage));

    int thr_ret=1;

    sa_term.sa_handler=(void(*)(int))term;
    sigaction(SIGTERM, &sa_term, NULL);
    sa_int.sa_handler=(void(*)(int))term;
    sigaction(SIGINT, &sa_int, NULL);


    thr_ret=pthread_create(&watchdog_thr, NULL, (void *(*)(void *))watchdog_thread, NULL);
    if(thr_ret!=0) 
    {
        fprintf(stderr, "Error! %d", thr_ret); raise(SIGTERM); 
    } 
    else 
    {
        thr_ret=1;
    }
    
    thr_ret=pthread_create(&analyzer_thr, NULL, (void *(*)(void *))analyzer_thread, NULL);
    if(thr_ret!=0) 
    {
        fprintf(stderr, "Error! %d", thr_ret); raise(SIGTERM); 
    } 
    else 
    {
        thr_ret=1;
    }
    
    thr_ret=pthread_create(&printer_thr, NULL, (void *(*)(void *))printer_thread, NULL);
    if(thr_ret!=0) 
    {
        fprintf(stderr, "Error! %d", thr_ret); raise(SIGTERM); 
    } 
    else 
    {
        thr_ret=1;
    }
    
    thr_ret=pthread_create(&reader_thr, NULL, (void *(*)(void *))reader_thread, NULL);
    if(thr_ret!=0) 
    {
        fprintf(stderr, "Error! %d", thr_ret); raise(SIGTERM); 
    } 


    while(terminate==0)
    {
    sleep(1);
    }

    
    return 0;
}

