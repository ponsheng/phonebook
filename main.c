#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#define DICT_FILE "./dictionary/words.txt"

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

    /* build the entry */
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    clock_gettime(CLOCK_REALTIME, &start);
#if !defined(OPT2)
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        e = append(line, e);
    }
#else
    entry *hashtable[TABLE_SIZE +1] = {};
    while(fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        append(line,hashtable);
    }
    /*for(int j=0;j<30000;j++){
        printf("ll %d  %s ",j,hashtable[j]->lastName);

    }*/
#endif
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    fclose(fp);

    e = pHead;

    /* the givn last name to find */
    //char input[MAX_LAST_NAME_SIZE] = "zyxel";
    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input0, e)->lastName, input));

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    srand(time(NULL));
    int count = 10;
    int curr = 0;
    cpu_time2 = 0;

    while(count>0) {
        fp = fopen(DICT_FILE, "r");
        int random = rand() % 349900;
        while(curr<349900) {
            fgets(line, sizeof(line), fp);
            if(curr == random) {
                while (line[i] != '\0')
                    i++;
                line[i - 1] = '\0';
                i = 0;

                /* compute the execution time */
                clock_gettime(CLOCK_REALTIME, &start);
#if !defined(OPT2)
                //printf("find %s",findName(line,e)->lastName);
                findName(line,e);
#else
                //printf("find %s",findName(line,hashtable)->lastName);
                findName(line,hashtable);
#endif

                clock_gettime(CLOCK_REALTIME, &end);
                cpu_time2 += diff_in_second(start, end);
                curr = 0;
                break;
            }
            curr++;
        }
        count--;
    }
    fclose(fp);

    FILE *output;
#if defined(OPT1)
    output = fopen("opt1.txt", "a");
#elif defined(OPT2)
    output = fopen("opt2.txt", "a");
#else
    output = fopen("orig.txt", "a");
#endif
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

    if (pHead->pNext) free(pHead->pNext);
    free(pHead);

    return 0;
}
