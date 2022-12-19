
#include <stdio.h>
#include <stdlib.h>
#define PAGESIZE 1024
unsigned char myheap[1048576];
typedef unsigned char BYTE;

typedef struct chunkhead
{
unsigned int size;
unsigned int info;
unsigned char *next,*prev;
}chunkhead;


unsigned char *mymalloc(unsigned int size){
    
    chunkhead *p = (chunkhead*)myheap;
    int max = sizeof(myheap);
    size = size / PAGESIZE;
    size = (size + 1) * PAGESIZE;

    for(;p!= 0; p = p->next){
        if (p->info == 0){
            if (size< p->size){
            chunkhead *current = p;
            int cursize = p->size;
            p->size = size;
            p->info = 1;
            
            chunkhead *splitMem; 
            splitMem = (chunkhead*)(unsigned char *)p + size + sizeof(chunkhead); 
            chunkhead *newp; 
            newp = (chunkhead*)(p->next);
            if(p->next != 0){

                splitMem->next = newp;
                splitMem->size = cursize - size;
                p-> next = (chunkhead*)splitMem;
                splitMem->prev = (chunkhead *)p;
                newp->prev = (chunkhead*)splitMem;
                return ((unsigned char*)p) + sizeof(chunkhead);
            }
            else if(p->next == 0){
                p->next = (chunkhead*)splitMem;
                p= p->next;
                p->info = 0;
                p->prev = current;
                p->size = (unsigned int)(cursize - size - sizeof(chunkhead)); 
                p->next = NULL;
                return ((unsigned char *)current) + sizeof(chunkhead);

            }
            }
            else if (size == p->size){
                p->info = 1;
                return ((unsigned char*)p) + sizeof(chunkhead);
            }


            }
        }

    return 0;
    }

    void analyse()
{
    chunkhead *p = (chunkhead*)myheap;

    int count= 1;
    while (p)
    {
        printf("\nChunk #%d:\n", count);
        printf("Size = %d bytes\n", p->size);
        if (p-> info == 1){
        printf("Occupied\n");}
        else{
        printf("Free\n");}
        printf("Next = %x \n", p->next);
        printf("Prev = %x \n", p->prev);
        p = (chunkhead*)p->next;
        count += 1;
    }
}

void myfree(unsigned char *address){
     chunkhead *p = (chunkhead *) (address - sizeof(chunkhead));
     p->info = 0;    
        
        if ((p->next) && !((chunkhead *)(p->next))->info)
        {
            chunkhead * next = (chunkhead *) p->next;
            p->size += next->size;
            p->next = next->next;
            next->prev = (unsigned char *)p;
        }
        if (p->prev) {
            p = (chunkhead *)p->prev;
        }
        
}

int main()
{
    chunkhead *p = (chunkhead*)myheap; 
    p->info = 0;
    p->prev = 0;
    p->next = 0;
    p->size = sizeof(myheap) - sizeof(chunkhead); 

unsigned char *a,*b,*c;
a = mymalloc(1000);
b = mymalloc(1000);
c = mymalloc(1000);
myfree(b);
myfree(a);
analyse();


}

