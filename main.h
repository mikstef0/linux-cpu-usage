#ifndef HEADER_H

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
long procno;

pthread_t reader_thr, analyzer_thr, printer_thr, watchdog_thr, logger_thr;

void* analyzer_thread(); // ush1
void* printer_thread(); //ush2
void* reader_thread();
void* watchdog_thread();
void* logger_thread();


#endif

