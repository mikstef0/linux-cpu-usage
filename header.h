#ifndef HEADER_H

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

short int terminate;

typedef struct procdata_struct
{
    char cpuno[max_cores+1][6];
    long values[max_cores+1][10];
} procdata;


procdata rd, rd_old;
long procno;
float CPU_Percentage[max_cores+1];

pthread_t reader_thr, analyzer_thr, printer_thr, watchdog_thr, logger_thr;

int reader_inactive_time, analyzer_inactive_time, printer_inactive_time;

void analyzer_code();
void printer_code();
void reader_code();
void watchdog_code_reader();
void watchdog_code_analyzer();
void watchdog_code_printer();
void logger_code();


void* analyzer_thread(); // ush1
void* printer_thread(); //ush2
void* reader_thread();
void* watchdog_thread();
void* logger_thread();


#endif

