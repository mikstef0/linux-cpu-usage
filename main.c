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
//#include "header.h"




void term()
{
    terminate=1;
    printf("SIGTERM encountered. Terminating...\n");
}


int main(int argc, char  *argv[])
{
    terminate=0;
    procno = sysconf(_SC_NPROCESSORS_ONLN);

    rd=(struct procdata_struct*)malloc((1+procno)*sizeof(*rd));
    rd_old=(struct procdata_struct*)malloc((1+procno)*sizeof(*rd_old));
    CPU_Percentage=(float*)malloc((1+procno)*sizeof(CPU_Percentage));

    int a=0, b=0, c=0, d=0;

    struct sigaction sa_term;
    sa_term.sa_handler=term;
    sigaction(SIGINT, &sa_term, NULL);



d=pthread_create(&watchdog_thr, NULL, watchdog_thread, NULL);
    b=pthread_create(&analyzer_thr, NULL, analyzer_thread, NULL);
    c=pthread_create(&printer_thr, NULL, printer_thread, NULL);
    sleep(1);
    a=pthread_create(&reader_thr, NULL, reader_thread, NULL);
 //   printf("%d %d %d", a,b,c);


    while(terminate==0)
    {
    sleep(1);
    }

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

    printf("Program has terminated.\n");


    return 0;
}

