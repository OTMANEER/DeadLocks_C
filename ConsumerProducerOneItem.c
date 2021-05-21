#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h> 
#define MaxItems 5 // Maximum items
#define buffer 5 // Size of the buffer

sem_t VIDE;
sem_t PLEINE;
int indiceProducteur= 0; 
int indiceConsommateur = 0;
int message[buffer];
pthread_mutex_t mutex;

void *producteur(void *idProducteur)
{   
    int item; 
        item = rand(); // Numero d'item  exemple.
        sem_wait(&VIDE);
        pthread_mutex_lock(&mutex);
        message[indiceProducteur] = item;
        printf("Le Producteur %d: produit l Item %d sur %d\n", *((int *)idProducteur),message[indiceProducteur],indiceProducteur);
        indiceProducteur= (indiceProducteur+1)%buffer;
        pthread_mutex_unlock(&mutex);
        sem_post(&PLEINE); 
}

void *consommateur(void *idConsommateur)
{   
    
        sem_wait(&PLEINE);
        pthread_mutex_lock(&mutex);
        int item =message[indiceConsommateur];
        printf("Le consommateur %d: prend l Item %d de %d\n",*((int *)idConsommateur),item, indiceConsommateur);
        indiceConsommateur = (indiceConsommateur+1)%buffer;
        pthread_mutex_unlock(&mutex);
        sem_post(&VIDE); 
}

int main()
{   

    pthread_t pro[5],con[5];
    pthread_t producteur,consommateur;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&VIDE,0,buffer);
    sem_init(&PLEINE,0,0);

    int tab[5] = {1,2}; 
 
        pthread_create(&producteur, NULL, (void *)producteur, (void *)&tab[0]); 
        pthread_create(&consommateur , NULL, (void *)consommateur, (void *)&tab[0]);
      
        pthread_join(producteur , NULL);  
        pthread_join(consommateur, NULL); 

    pthread_mutex_destroy(&mutex);
    sem_destroy(&VIDE);
    sem_destroy(&PLEINE); 
    return 0;
    
}