#include "cut_threads.h"

static pthread_mutex_t mutex_reader = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_analyzer = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_printer = PTHREAD_MUTEX_INITIALIZER;

void* analyzer_thread()
{
    sa_analyzer.sa_handler=(void(*)(int))analyzer_code;
    sigaction(SIGUSR1, &sa_analyzer, NULL);
    while(1)
    {
        pause();
    }
}

void* printer_thread()
{
    sa_printer.sa_handler=(void(*)(int))printer_code;
    sigaction(SIGUSR2, &sa_printer, NULL);
    while(1)
    {
        pause();
    }
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

    sa_reader_watchdog.sa_handler=(void(*)(int))watchdog_code_reader;
    sigaction(SIGBUS, &sa_reader_watchdog, NULL);
    sa_analyzer_watchdog.sa_handler=(void(*)(int))watchdog_code_analyzer;
    sigaction(SIGCONT, &sa_analyzer_watchdog, NULL);
    sa_printer_watchdog.sa_handler=(void(*)(int))watchdog_code_printer;
    sigaction(SIGCLD, &sa_printer_watchdog, NULL);

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

        if(reader_inactive_time>MAX_INACTIVE_TIME || analyzer_inactive_time>MAX_INACTIVE_TIME || printer_inactive_time>MAX_INACTIVE_TIME)
        {
            sprintf(logs, "Program has hung up! Re/An/Pr inactive for: %d %d %d seconds\n", reader_inactive_time, analyzer_inactive_time, printer_inactive_time);
            raise(SIGALRM);
            fprintf(stdout, "Program has hung up! Re/An/Pr inactive for: %d %d %d seconds\n", reader_inactive_time, analyzer_inactive_time, printer_inactive_time);
            raise(SIGTERM);
        }
        sleep(1);
    }
}

void* logger_thread()
{
    sa_logger.sa_handler=(void(*)(int))logger_code;
    sigaction(SIGALRM, &sa_logger, NULL);
    while(1)
    {
        pause();
    }
}

void reader_code()
{
    FILE* file;
    file=fopen("/proc/stat","r");
    if (file == NULL)
    {
        sprintf(logs, "Error opening /proc/stat file.\n");
        raise(SIGALRM);
        fprintf(stderr, "Error opening /proc/stat file.\n");
    }

    for(int i=0; i<=procno; i++)
    {
        fscanf(file, "%s %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld", rd[i].cpuno, &rd[i].values[user], &rd[i].values[nice], &rd[i].values[sys], &rd[i].values[idle], &rd[i].values[iowait],
               &rd[i].values[irq], &rd[i].values[softirq], &rd[i].values[steal], &rd[i].values[guest], &rd[i].values[guest_nice]);
        dqueue_enq(&data_queue[i],rd[i]);
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
        strcpy(rd_old[i].cpuno, rd2[i].cpuno);
        for(int j=0; j<10; j++)
        {
            rd_old[i].values[j]=rd2[i].values[j];
        }
        rd2[i]=dqueue_deq(&data_queue[i]);

        PrevIdle = rd_old[i].values[idle];
        Idle = rd2[i].values[idle];

        PrevNonIdle = rd_old[i].values[user] + rd_old[i].values[nice] + rd_old[i].values[sys] + rd_old[i].values[irq] + rd_old[i].values[softirq] + rd_old[i].values[steal]+rd_old[i].values[iowait];
        NonIdle = rd2[i].values[user] + rd2[i].values[nice] + rd2[i].values[sys] + rd2[i].values[irq] + rd2[i].values[softirq] + rd2[i].values[steal]+ rd2[i].values[iowait];

        PrevTotal = PrevIdle + PrevNonIdle;
        Total = Idle + NonIdle;

        totald = Total - PrevTotal;
        idled = Idle - PrevIdle;

        CPU_Percentage[i] = (float)100.0*(totald - idled)/totald;

    }
    pthread_kill(watchdog_thr, SIGCONT);
    pthread_kill(printer_thr, SIGUSR2);
}


void printer_code()
{
    //system("clear");
    for(int i=0; i<=procno; i++)
    {
        if(rd2[i].cpuno[0]=='c')
            fprintf(stdout,"CPU: %s PERCENTAGE: %.2f\n", rd2[i].cpuno,(double)CPU_Percentage[i]);
        else
        {
            sprintf(logs, "Error printing CPU usage for CPU: %s.\n", rd2[i].cpuno);
            raise(SIGALRM);
        }
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
    FILE* file2;
    file2=fopen("log.txt","a");
    time_t timestamp = time(NULL);
    char * timestamp_str = ctime(&timestamp);
    timestamp_str[strlen(timestamp_str)-1] = '\0';
    if (file2 == NULL)
    {
        fprintf(stderr, "Error opening the log file.\n");
    }
    fprintf(file2, "%s %s", timestamp_str, logs);
    strcpy(logs,"");
    fclose(file2);
}

