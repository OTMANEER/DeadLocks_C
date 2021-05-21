 
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
 

#define MaxItems 10  
#define taille 10

sem_t VIDE;
sem_t PLEINE;
int in = 0; 
int out = 0;
int message[taille];
pthread_mutex_t mutex;

void *Producteur(void *ProIndex)
{   
    int item;
    for(int i = 0; i < MaxItems; i++) {
        item = rand(); // Produce an random item
        sem_wait(&VIDE);
        pthread_mutex_lock(&mutex);
        message[in] = item;
        printf("Producteur %d: Insert Item %d at %d\n", *((int *)ProIndex),message[in],in);
        in = (in+1)%taille;
        pthread_mutex_unlock(&mutex);
        sem_post(&PLEINE);
    }
}
void *Consommateur(void *ConIndex)
{   
    for(int i = 0; i < MaxItems; i++) {
        sem_wait(&PLEINE);
        pthread_mutex_lock(&mutex);
        int item = message[out];
        printf("Consommateur %d: Remove Item %d from %d\n",*((int *)ConIndex),item, out);
        out = (out+1)%taille;
        pthread_mutex_unlock(&mutex);
        sem_post(&VIDE);
    }
}

int main()
{   

    pthread_t producteurs[10],consommateurs[10];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&VIDE,0,taille);
    sem_init(&PLEINE,0,0);

    int a[10] = {1,2,3,4,5,6,7,8,9,10};
    for(int i = 0; i < 10; i++) {
        pthread_create(&producteurs[i], NULL, (void *)Producteur, (void *)&a[i]);
    }
    for(int i = 0; i < 10; i++) {
        pthread_create(&consommateurs[i], NULL, (void *)Consommateur, (void *)&a[i]);
    }
    for(int i = 0; i < 10; i++) {
        pthread_join(producteurs[i], NULL);
    }
    for(int i = 0; i < 10; i++) {
        pthread_join(consommateurs[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&VIDE);
    sem_destroy(&PLEINE);

    return 0;
    
}