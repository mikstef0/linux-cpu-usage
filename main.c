#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


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
    char cpuno[max_cores][6];
    long values[max_cores][10];
} procdata;


procdata rd, rd_old;
long procno;
float CPU_Percentage[max_cores];


void* reader_code()
{

    FILE* file;
    file=fopen("/proc/stat","r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening /proc/stat file.\n");
        return 1;
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
    return 0;
}


void* analyzer_code()
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

}
}


void* printer_code()
{
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

    while(1)
    {

    reader_code();
    sleep(1);
    analyzer_code();
    sleep(1);


    printer_code();

    sleep(1);

    }








    return 0;
}

