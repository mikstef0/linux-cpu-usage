#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "header.h"

int main(int argc, char  *argv[])
{
    procno = sysconf(_SC_NPROCESSORS_ONLN);

    int a=0, b=0, c=0, d=0;

/*
    struct sigaction sa1;
    sa1.sa_handler=&ush1;
    sa1.sa_flags=SA_RESTART;
    sigaction(SIGUSR1, &sa1, NULL);
*/




d=pthread_create(&watchdog_thr, NULL, watchdog_thread, NULL);
    b=pthread_create(&analyzer_thr, NULL, analyzer_thread, NULL);
    //pthread_kill(analyzer_thr, SIGUSR1);
    c=pthread_create(&printer_thr, NULL, printer_thread, NULL);
    sleep(1);
    a=pthread_create(&reader_thr, NULL, reader_thread, NULL);
 //   printf("%d %d %d", a,b,c);


    while(1)
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



    return 0;
}

