#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_opt2.h"

entry *findName(char lastName[],entry *hashtable[])
{
    unsigned id = hash_fun(lastName);
    if(hashtable[id])
    {
        entry *temp = hashtable[id];
        while(temp)
        {
            if (strcasecmp(lastName, temp->lastName) == 0)
            {
                return temp;
            }
            temp= temp->pNext;
        }
        printf("miss");
    }
    printf("GG");
    return NULL;

}

entry *append(char lastName[], entry *hashtable[])
{
    /* allocate memory for the new entry and put lastName */
    entry *e;
    e = (entry *) malloc(sizeof(entry));
    strcpy(e->lastName, lastName);
    e->pNext = NULL;
    
    unsigned id = hash_fun(lastName);

    if(!hashtable[id])
    {
        hashtable[id]=e;
        //printf("%d  ",id);
    }
    else
    {
        e->pNext = hashtable[id]->pNext;
        hashtable[id]->pNext = e;
    }
    return e;
}

unsigned hash_fun(char key[]) //djb2 hash function
{
    int mod = TABLE_SIZE;
    unsigned hash = 5381;
    int i=0;
    while (key[i]>0)
    {
        hash = ((hash << 5) + hash) + key[i++]; /* hash * 33 + c */
    }
    return hash % mod ;
}
