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
        while(temp->pNext)
        {
            if (strcasecmp(lastName, temp->lastName) == 0)
            {
                printf("%s found\n",temp->lastName);
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
        entry *temp = hashtable[id];
        while(temp->pNext)
        {
            temp= temp->pNext;
        }
        temp->pNext = e;
    }
    return e;
}

unsigned hash_fun(char key[])
{
    int mod = 900000;
    //int length = sizeof(key[0]);//sizeof(key[0]);
    //int count = strArray.Count(x => x != null); count size ways
    unsigned sum;
    int i;
    for (sum=1, i=0; i < 16&&key[i]>0; i++){
        if(key[i]>0)
		sum *= key[i];
    }
    //printf(" id: %u",sum);
    return (sum % mod)/3;
}
