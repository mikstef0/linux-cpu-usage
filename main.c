#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#define max_cores 32

#define user 0
#define nice 1
#define sys 2
#define idle 3
#define iowait 4
#define irq 5
#define softirq 6
#define steal 7
#define guest 8
#define guest_nice 9

typedef struct procdata_struct
{
    char cpuno[max_cores+1][6];
    long values[max_cores+1][10];
} procdata;


procdata rd, rd_old;
long procno;
float CPU_Percentage[max_cores+1];

 pthread_t reader_thr, analyzer_thr, printer_thr;

void analyzer_code();
void printer_code();


void* analyzer_thread() // ush1
{
    printf("analyzer_thread is working");
    struct sigaction sa1;
    sa1.sa_handler=analyzer_code;
    sa1.sa_flags=SA_RESTART;
    sigaction(SIGUSR1, &sa1, NULL);
    while(1) sleep(1);
    //analyzer_code();
}

void* printer_thread() //ush2
{
    printf("printer_thread is working");
    struct sigaction sa2;
    sa2.sa_handler=printer_code;
    sa2.sa_flags=SA_RESTART;
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
    printf("reader started");
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
 //   printf("Reader working, sending SIGUSR1");
    pthread_kill(analyzer_thr, SIGUSR1);
    return 0;
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
//raise(SIGUSR2);
pthread_kill(printer_thr, SIGUSR2);

//printf("%s %d %d %d %d %d\n", rd.cpuno[i], rd.values[i][user], rd.values[i][nice], rd.values[i][sys], rd.values[i][idle], rd.values[i][iowait]);
      //         rd.values[i][irq], rd.values[i][softirq], rd.values[i][steal], rd.values[i][guest], rd.values[i][guest_nice]);

//printf("%s %d %d %d %d %d\n", rd_old.cpuno[i], rd_old.values[i][user], rd_old.values[i][nice], rd_old.values[i][sys], rd_old.values[i][idle], rd_old.values[i][iowait]);
 //         rd.values[i][irq], rd.values[i][softirq], rd.values[i][steal], rd.values[i][guest], rd.values[i][guest_nice]);

//printf("PI, I, PNI, NI, PT, T, tld, idled, CPU\n%d %d %d %d %d %d %d %d %f\n", PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, totald,idled, CPU_Percentage[i]);
}
}


void printer_code()
{


    system("clear");
    for(int i=0; i<=procno; i++)
    {
        printf("CPU: %s PERCENTAGE: %f\n", rd.cpuno[i],CPU_Percentage[i]);
    }


}

void* watchdog_code()
{

}

void* logger_code()
{

}

int main(int argc, char  *argv[])
{
    procno = sysconf(_SC_NPROCESSORS_ONLN);

    printf("%d\n",procno);
/*
    struct sigaction sa2;
    sa2.sa_handler=&ush2;
    sa2.sa_flags=SA_RESTART;
    sigaction(SIGUSR2, &sa2, NULL);

    struct sigaction sa1;
    sa1.sa_handler=&ush1;
    sa1.sa_flags=SA_RESTART;
    sigaction(SIGUSR1, &sa1, NULL);
*/




    b=pthread_create(&analyzer_thr, NULL, analyzer_thread, NULL);
    c=pthread_create(&printer_thr, NULL, printer_thread, NULL);
    sleep(1);
    a=pthread_create(&reader_thr, NULL, reader_thread, NULL);
    printf("%d %d %d", a,b,c);

  //  while(1)
 //   {


   // }
    pthread_cancel(reader_thr);
    pthread_cancel(analyzer_thr);
    pthread_cancel(printer_thr);
    pthread_join(reader_thr, NULL);
    pthread_join(analyzer_thr, NULL);
    pthread_join(printer_thr, NULL);




    return 0;
}

