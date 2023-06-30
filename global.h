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



struct procdata_struct
{
    char cpuno[6];
    long values[10];
};

struct procdata_struct *rd, *rd_old;

float *CPU_Percentage;

long procno;


#endif

