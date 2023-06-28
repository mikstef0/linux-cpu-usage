#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "header.h"

void* analyzer_thread() // ush1
{
    struct sigaction sa1;
    sa1.sa_handler=analyzer_code;
    sigaction(SIGUSR1, &sa1, NULL);
    while(1) sleep(1);
    //analyzer_code();
}

void* printer_thread() //ush2
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
        rd_old.values[i][user]=rd.values[i][user];
        rd_old.values[i][nice]=rd.values[i][nice];
        rd_old.values[i][sys]=rd.values[i][sys];
        rd_old.values[i][idle]=rd.values[i][idle];
        rd_old.values[i][iowait]=rd.values[i][iowait];
        rd_old.values[i][irq]=rd.values[i][irq];
        rd_old.values[i][softirq]=rd.values[i][softirq];
        rd_old.values[i][steal]=rd.values[i][steal];
        rd_old.values[i][guest]=rd.values[i][guest];
        rd_old.values[i][guest_nice]=rd.values[i][guest_nice];

        fscanf(file, "%s %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld", &rd.cpuno[i], &rd.values[i][user], &rd.values[i][nice], &rd.values[i][sys], &rd.values[i][idle], &rd.values[i][iowait],
               &rd.values[i][irq], &rd.values[i][softirq], &rd.values[i][steal], &rd.values[i][guest], &rd.values[i][guest_nice]);
    }
    fclose(file);
    pthread_kill(analyzer_thr, SIGUSR1);
}


void analyzer_code()
{
    long PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, totald, idled;


    for(int i=0; i<=procno; i++)
    {

PrevIdle = rd_old.values[i][idle];
Idle = rd.values[i][idle];

PrevNonIdle = rd_old.values[i][user] + rd_old.values[i][nice] + rd_old.values[i][sys] + rd_old.values[i][irq] + rd_old.values[i][softirq] + rd_old.values[i][steal]+rd_old.values[i][iowait];
NonIdle = rd.values[i][user] + rd.values[i][nice] + rd.values[i][sys] + rd.values[i][irq] + rd.values[i][softirq] + rd.values[i][steal]+ rd.values[i][iowait];

PrevTotal = PrevIdle + PrevNonIdle;
Total = Idle + NonIdle;

totald = Total - PrevTotal;
idled = Idle - PrevIdle;

CPU_Percentage[i] = 100.0*(totald - idled)/totald;
pthread_kill(printer_thr, SIGUSR2);

}
}


void printer_code()
{
    system("clear");
    for(int i=0; i<=procno; i++)
    {
            printf("CPU: %s PERCENTAGE: %.2f\n", rd.cpuno[i],CPU_Percentage[i]);
    }
}

void* watchdog_code()
{

}

void* logger_code()
{

}
 
