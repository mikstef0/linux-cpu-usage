#ifndef CUT_THREADS_H
#define CUT_THREADS_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "cut_queue.h"
#include "global.h"

#define MAX_INACTIVE_TIME 4

pthread_t reader_thr, analyzer_thr, printer_thr, watchdog_thr, logger_thr;
int reader_inactive_time, analyzer_inactive_time, printer_inactive_time;

struct sigaction sa_analyzer; // signal handling struct for reader->analyzer signal
struct sigaction sa_printer; // signal handling struct for analyzer->printer signal
struct sigaction sa_reader_watchdog; // signal handling struct for reader->watchdog signal
struct sigaction sa_analyzer_watchdog; // signal handling struct for analyzer->watchdog signal
struct sigaction sa_printer_watchdog; // signal handling struct for printer->watchdog signal
struct sigaction sa_logger; // signal handling struct for logging trigger signal

void analyzer_code(void); // analyzer logic
void printer_code(void); // printer logic
void reader_code(void); // reader logic
void watchdog_code_reader(void); // watchdog logic for reader
void watchdog_code_analyzer(void); // watchdog logic for analyzer
void watchdog_code_printer(void); // watchdog logic for printer
void logger_code(void); // logger logic

#endif

