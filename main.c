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

void playDay(int d)
{
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    printf("Day %d ended!\n", d);
}

int main(int argc, char *argv[])
{
    if (argc < 8){
        printf("Pass parameters innkeepers, knights, shopkeepers, ladies, seats, drinks, food, gems");
        return 0;
    }
    innkeepers = atoi(argv[1]);
    knights = atoi(argv[2]);
    shopkeepers = atoi(argv[3]);
    ladies = atoi(argv[4]);
    seats = atoi(argv[5]);
    drinks = atoi(argv[6]);
    food = atoi(argv[7]);
    gems = atoi(argv[8]);
    pthread_mutex_init(&mutex_resources, NULL);
    pthread_mutex_init(&mutex_knight, NULL);
    pthread_mutex_init(&mutex_shopkeeper, NULL);
    pthread_mutex_init(&mutex_lady, NULL);
    pthread_mutex_init(&mutex_innkeeper, NULL);
    pthread_mutex_init(&mutex_day, NULL);
    for (int d=1;d<=365;d++){
        pthread_mutex_lock(&mutex_day);
        if(innkeepers==0 && knights==0 && shopkeepers==0 && ladies==0){
            printf("Your village lasted %d days!\n", d-1);
            break;
        }
        playDay(d);
        pthread_mutex_unlock(&mutex_day);
    }
    return 0;
}
