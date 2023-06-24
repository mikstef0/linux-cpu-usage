#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


#define max_cores 32


 void* reader_code()
 {
    int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    char cpuno[6];
    FILE* file;
    file=fopen("/proc/stat","r");
    if (file == NULL)
        {
        fprintf(stderr, "Error opening /proc/stat file.\n");
        return 1;
        }

    fscanf(file, "%s %d %d %d %d %d %d %d %d %d %d", &cpuno, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);
    while(cpuno[0]=='c' && cpuno[1]=='p' && cpuno[2]=='u')
    {
    printf("%s %d %d %d %d %d %d %d %d %d %d\n", &cpuno, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice);
    fscanf(file, "%s %d %d %d %d %d %d %d %d %d %d", &cpuno, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);
    }
    fclose(file);
    return 0;
 }


 void* analyzer_code()
 {

 }


void* printer_code()
{

}

void* watchdog_code()
{

}

void* logger_code()
{

}

 int main(int argc, char  *argv[])
 {

    reader_code();









 return 0;
 }

