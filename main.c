#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

int innkeepers, knights, shopkeepers, ladies, seats, drinks, food, gems;

pthread_mutex_t mutex_resources = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_knight = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_shopkeeper = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_lady = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_innkeeper = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_day = PTHREAD_MUTEX_INITIALIZER;

typedef struct element {
   struct element *next;
   char type[6];
 } wares ;

int throwDice(){
    return rand()%6+1;
}

void visit_lady();
void fight();
void buy_wares();
void serve();

void *knight(void *vargp);
void *innkeeper(void *vargp);
void *lady(void *vargp);
void *shopkeeper(void *vargp);

int main(int argc, char *argv[])
{
    return 0;
}
