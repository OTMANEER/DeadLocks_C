#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

int N; // nombre des philosophes dans ce cas

#define NBMAXPHILO 10

#define MAXAMANGER 56
// la LEFT du premier philosophe est le dernier philosophe
#define LEFT(x) x ? x - 1 : N - 1
//La RIGHT du 5ème philosophe est le premier philosophe
#define RIGHT(x) (x == N - 1) ? 0 : x + 1 
// Definition de la fonction
int pthread_mutex_init(pthread_mutex_t *mutex,const pthread_mutexattr_t *mutexattr);

/* Lock int pthread_mutex_lock(pthread_mutex_t *mutex);*/
/* Unlock int pthread_mutex_unlock(pthread_mutex_t *mutex);*/
/* Destruct int pthread_mutex_destroy(pthread_mutex_t *mutex);*/

/*
int x;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_lock(&mut);
     --> agir sur x 
pthread_mutex_unlock(&mut);

*/
int etat[NBMAXPHILO];
int quantitemange[NBMAXPHILO];
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void affichage() {
static unsigned count = 0;
int j;
//section critique
pthread_mutex_lock(&mut);
write(0, " ", 1);
// Pour chauqe philosophe
for (j = 0; j < N; j++) {
if (etat[j]==0){ // if etat[j] == 0, alors il discutte 
     write(0," D", 3); // Standard Output, write " "  3 bytes
     }else{
      write(0," M", 3);// Sinon il mange
      }
}
write(0, " \n", 3);
count++;
if ( count > N - 1 ) {
count = 0;
printf("\n");
// Pour chaque philoshope afficher la quantité qui represente le nb de fois qu'il a manger
for(j = 0; j < N; j++) {
printf(" %u", quantitemange[j]); 
}
printf("\n------------\n");
} 
pthread_mutex_unlock(&mut);
// fin de Section critique
}
void manger(int i){
int j;
affichage(); // afficher 
usleep(random() & (1024 * 256 - 1)); // Sleep
affichage(); // affichage
/* fini de manger*/
etat[i] = 0; // il est pret à discutter et son etat vaut 0.
}
void essayer_de_manger(int i){
pthread_mutex_lock(&mut);
/* debut Section critique */
if ( !(etat[LEFT(i)]) && !(etat[RIGHT(i)]) ) {// si  les deux (RIGHT et LEFT sont en train de discutter)
etat[i] = 1; // changer l'etat en (en train de manger avec 1)
}
/* fin Section critique*/
pthread_mutex_unlock(&mut);
if (etat[i]) { // s'il a vraiment mangé (c'est a dire etat vaut 1)
quantitemange[i]++; // La quantité  augmente
if (quantitemange[i] > MAXAMANGER) exit(0);// stop la quantité max est dépasée.
manger(i);
}
}
void discuter(int i){
usleep(random() & (1024 * 256 - 1));
//printf("%d discute\n", i);
}
void * philosophe(void * i) {
    srandom((int) pthread_self());
    while (1) {
    essayer_de_manger((int)i);
    discuter((int) i);
    }
}

void lancer_threads() {
int i;
pthread_t thrd;
// Pour chaque philosophe creer un Thread ==> Un processus qui represente le philosophe
        for (i = 0; i < N; i++) {
                                pthread_create(&thrd, NULL, philosophe, (void *)i);
                                }
}

void usage(char *s){
printf("----------------\n"); 
printf("Le nombre de philosophes doit etre moins à: %u\n", NBMAXPHILO); 
printf("----------------\n");
}
void init(int argc, char * argv[]) {
int i;
if (argc < 2) {
usage(argv[0]);
N = 5;
printf(" 5 Philosophes\n");
} else {
N = atoi(argv[1]); // Convertir la peremiere String to integer
N = N > NBMAXPHILO ? NBMAXPHILO : N; // S'il deppasse le nombre max dans ce cas 5 alors prend Nombre MAx déjà déclaré
}

// Initialiser tout les  états et les quantité mangées à zero.  en attente  start eating
for (i = 0; i < N; i++) {
etat[i] = 0;
quantitemange[i] = 0;
}
}

int main(int argc, char *argv[]){
init(argc, argv); // Initialisation
lancer_threads();
pthread_exit("all threads finished, bye bye");
exit(EXIT_SUCCESS);
}