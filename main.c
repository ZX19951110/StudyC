#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
void makenext(const char* ptrn, int *next)
{
    int len = strlen(ptrn);
    next[0] = 0;
    next[1] = 0;
    int i = 1;
    while(i < len - 1)
    {
        int j = next[i];
        while(j > 0 && ptrn[j] != ptrn[i]) j = next[j];
        if(ptrn[j] == ptrn[i])  next[i + 1] = j + 1;
        else next[i + 1] = j;
        i++;
    }
}
//KMP
int indexofsubstr(char* str, char* ptrn)
{
    int lenstr = strlen(str);
    int lenptrn = strlen(ptrn);
    int next[1024]; //假设模式串的长度不超过1024
    makenext(ptrn, next);
    int i = 0, j = 0;
    while(i < lenstr && j < lenptrn)
    {
        if(str[i] == ptrn[j])
        {
            i++,j++;
        }
        else
        {
            j = next[j];
            // if j euqals zero, increase i by 1. Otherwize, there may be a infinite loop!
            if(j == 0) i++;

        }
        if(j == lenptrn) return i - j; //match successfully, return the index
    }
    return -1;
}

long long getCurrentTime(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*1000*1000+tv.tv_usec;
}

int main(int argc, char** argv)
{



    unsigned long long start = getCurrentTime();
    printf("%d",start);
    char* str = "A former Canadian diplomat detained in China last week is being denied legal representation and is not allowed to turn lights off at night, sources say. Michael Kovrig, who now works for the International Crisis Group (ICG) think tank, was detained on accusations of harming national security. Canada drew Chinese protests after it arrested an executive at telecoms giant Huawei at the request of the US. China says it has guaranteed Mr Kovrig's lawful rights. ICG spokesman Karim Lebhour told the BBC: \"Michael has not been allowed access to his lawyers. The arrest is unjustified.\" Mr Kovrig has been working as North East Asia senior adviser for the Brussels-based think tank since February 2017. Meanwhile an unnamed source told Reuters news agency that he was being held at an undisclosed location and being questioned every morning, afternoon and evening. He is not allowed to turn off the lights when he tries to sleep at night, the person added. \"He is physically all right, but tired and stressed,\" a source told the Financial Times. \"Physically, he does not appear mistreated.\"";
    char* sub = "He is not allowed to turn off the lights when he tries to sleep at night, the person added.";
    printf("Index:%d\n", indexofsubstr(str, sub));
    long long end = getCurrentTime();
    printf("time=%d\n",end-start);

    return 0;
}