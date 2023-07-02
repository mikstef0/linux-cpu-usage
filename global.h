#ifndef GLOBAL_H
#define GLOBAL_H

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

#define MAX_Q_SIZE 8

void* analyzer_thread(void); // analyzer thread code
void* printer_thread(void); // printer thread code
void* reader_thread(void); // reader thread code
void* watchdog_thread(void); // watchdog thread code
void* logger_thread(void); // logger thread code

struct procdata_struct // structure for /proc/stat read date values
{
    char cpuno[6]; // name of a CPU / CPU core
    long values[10]; // values: user, nice, sys... for each CPU core
};

typedef struct
{
    struct procdata_struct data[MAX_Q_SIZE];
    int q_front;
    int q_rear;
} dq; // /proc/stat data queue

struct procdata_struct *rd, *rd_old, *rd2;
dq *data_queue; // pointer for /proc/stat read data queue

float *CPU_Percentage;
long procno; // number of CPUs / CPU cores

static char logs[100];


#endif

