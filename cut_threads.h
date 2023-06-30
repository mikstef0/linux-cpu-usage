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


pthread_mutex_t mutex_reader = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_analyzer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_printer = PTHREAD_MUTEX_INITIALIZER;

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

