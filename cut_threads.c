#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "cut_threads.h"
#include "global.h"
//#include "header.h"

//pthread_mutex_t mutex_reader = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mutex_analyzer = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mutex_printer = PTHREAD_MUTEX_INITIALIZER;

void* analyzer_thread()
{
    struct sigaction sa1;
    sa1.sa_handler=analyzer_code;
    sigaction(SIGUSR1, &sa1, NULL);
    while(1) sleep(1);
    //analyzer_code();
}

void* printer_thread()
{
    struct sigaction sa2;
    sa2.sa_handler=printer_code;
    sigaction(SIGUSR2, &sa2, NULL);
    while(1) sleep(1);
    //printer_code();
}

void* reader_thread()
{
    while(1)
    {
        reader_code();
        sleep(1);

    }

}

void* watchdog_thread()
{
    reader_inactive_time=0;
    analyzer_inactive_time=0;
    printer_inactive_time=0;


    struct sigaction sa7;
    sa7.sa_handler=watchdog_code_reader;
    sigaction(SIGBUS, &sa7, NULL);
    struct sigaction sa8;
    sa8.sa_handler=watchdog_code_analyzer;
    sigaction(SIGCONT, &sa8, NULL);
    struct sigaction sa9;
    sa9.sa_handler=watchdog_code_printer;
    sigaction(SIGCLD, &sa9, NULL);

    while(1)
    {
        pthread_mutex_lock(&mutex_reader);
        reader_inactive_time++;
        pthread_mutex_unlock(&mutex_reader);
        pthread_mutex_lock(&mutex_analyzer);
        analyzer_inactive_time++;
        pthread_mutex_unlock(&mutex_analyzer);
        pthread_mutex_lock(&mutex_printer);
        printer_inactive_time++;
        pthread_mutex_unlock(&mutex_printer);




        if(reader_inactive_time>4 || analyzer_inactive_time>4 || printer_inactive_time>4)
        {
            printf("ZAWIESILO SIE! %d %d %d\n", reader_inactive_time, analyzer_inactive_time, printer_inactive_time); raise(SIGTERM);
        }
        sleep(1);
    }
}

void reader_code()
{
    FILE* file;
    file=fopen("/proc/stat","r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening /proc/stat file.\n");
    }

    for(int i=0; i<=procno; i++)
    {
        rd_old[i].values[user]=rd[i].values[user];
        rd_old[i].values[nice]=rd[i].values[nice];
        rd_old[i].values[sys]=rd[i].values[sys];
        rd_old[i].values[idle]=rd[i].values[idle];
        rd_old[i].values[iowait]=rd[i].values[iowait];
        rd_old[i].values[irq]=rd[i].values[irq];
        rd_old[i].values[softirq]=rd[i].values[softirq];
        rd_old[i].values[steal]=rd[i].values[steal];
        rd_old[i].values[guest]=rd[i].values[guest];
        rd_old[i].values[guest_nice]=rd[i].values[guest_nice];

        fscanf(file, "%s %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld", &rd[i].cpuno, &rd[i].values[user], &rd[i].values[nice], &rd[i].values[sys], &rd[i].values[idle], &rd[i].values[iowait],
               &rd[i].values[irq], &rd[i].values[softirq], &rd[i].values[steal], &rd[i].values[guest], &rd[i].values[guest_nice]);
    }
    fclose(file);
    pthread_kill(watchdog_thr, SIGBUS);
    pthread_kill(analyzer_thr, SIGUSR1);

}


void analyzer_code()
{
    long PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, totald, idled;


    for(int i=0; i<=procno; i++)
    {

        PrevIdle = rd_old[i].values[idle];
        Idle = rd[i].values[idle];

        PrevNonIdle = rd_old[i].values[user] + rd_old[i].values[nice] + rd_old[i].values[sys] + rd_old[i].values[irq] + rd_old[i].values[softirq] + rd_old[i].values[steal]+rd_old[i].values[iowait];
        NonIdle = rd[i].values[user] + rd[i].values[nice] + rd[i].values[sys] + rd[i].values[irq] + rd[i].values[softirq] + rd[i].values[steal]+ rd[i].values[iowait];

        PrevTotal = PrevIdle + PrevNonIdle;
        Total = Idle + NonIdle;

        totald = Total - PrevTotal;
        idled = Idle - PrevIdle;

        CPU_Percentage[i] = 100.0*(totald - idled)/totald;


    }
    pthread_kill(watchdog_thr, SIGCONT);
    pthread_kill(printer_thr, SIGUSR2);
}


void printer_code()
{
    system("clear");
    for(int i=0; i<=procno; i++)
    {
        printf("CPU: %s PERCENTAGE: %.2f\n", rd[i].cpuno,CPU_Percentage[i]);
    }
    pthread_kill(watchdog_thr, SIGCLD);
}

void watchdog_code_reader()
{
    pthread_mutex_lock(&mutex_reader);
    reader_inactive_time=0;
    pthread_mutex_unlock(&mutex_reader);
}

void watchdog_code_analyzer()
{
    pthread_mutex_lock(&mutex_analyzer);
    analyzer_inactive_time=0;
    pthread_mutex_unlock(&mutex_analyzer);
}

void watchdog_code_printer()
{
    pthread_mutex_lock(&mutex_printer);
    printer_inactive_time=0;
    pthread_mutex_unlock(&mutex_printer);
}

void logger_code()
{

}

