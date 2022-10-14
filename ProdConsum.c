// gcc -g -Wall -o ProdConsum ProdConsum.c -lpthread 
// ./ProdConsum
 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define tamanho_fila 3 

int i = 0, j = 0, cont = 0;

sem_t interno;
sem_t enche;
sem_t esvazia;

typedef struct Clock { 
   int p[3];
} Clock;

Clock array[tamanho_fila];

void *produtor(Clock c){ 
    
    Clock clock1 = c;
    
    if (cont == tamanho_fila){
        printf("Fila está cheia!\n");
    }
    
    sem_wait(&esvazia);
    sem_wait(&interno);
    
    array[i] = clock1;
    i = (i+1) % tamanho_fila;
    cont++;
    
    sem_post(&interno);
    sem_post(&enche);
    
}

void *consumidor(){   
    
    if (cont == 0){
        printf("Fila está vazia!\n");
    }
    
    sem_wait(&enche);
    sem_wait(&interno);
    
    Clock clock1 = array[j];
    printf("Relogio (%d,%d,%d), que estava na posicao %d da fila, foi consumido.\n", clock1.p[0], clock1.p[1], clock1.p[2], j);
    j = (j+1) % tamanho_fila;
    cont--;
    
    sem_post(&interno);
    sem_post(&esvazia);
    
}

void *prodClock (){
    
    Clock clock1;

    clock1.p[0] = 1;
    clock1.p[1] = 1;
    clock1.p[2] = 1;
    produtor(clock1);
    clock1.p[0] = 2;
    clock1.p[1] = 2;
    clock1.p[2] = 2;
    produtor(clock1);
    clock1.p[0] = 3;
    clock1.p[1] = 3;
    clock1.p[2] = 3;
    produtor(clock1);
    clock1.p[0] = 4;
    clock1.p[1] = 4;
    clock1.p[2] = 4;
    produtor(clock1);
    
}
 
void *consumClock (){
    
    consumidor();
    consumidor();
    consumidor();
    consumidor();
    
}

int main(void){   

    pthread_t t1, t2;
    
    sem_init(&interno, 0, 1);
    sem_init(&enche, 0, 0);
    sem_init(&esvazia, 0, tamanho_fila);

    pthread_create(&t1, NULL, (void*)prodClock, NULL);
    pthread_create(&t2, NULL, (void*)consumClock, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&esvazia);
    sem_destroy(&enche);
    sem_destroy(&interno);

    return 0;
    
}
