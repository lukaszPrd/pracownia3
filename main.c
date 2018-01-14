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
   char *type;
 } wares ;

wares *first;

 void add_to_list (wares *el, char *type)
 {
   wares *wsk, *new_item;
   wsk = el;
   while (wsk->next != NULL)
     {
     wsk = wsk->next;
     }
   new_item =(wares*) malloc (sizeof(wares));
   new_item->type = type;
   new_item->next = NULL;
   wsk->next = new_item;
 }

bool delete_from_list(wares *el, char element[6])
{
   wares *wsk=el;
   while (wsk->next != NULL)
   {
     if (wsk->next->type == element)
     {
       wares *del=wsk->next;
       wsk->next = del->next;
       free(del);
       return true;
     }
     else
     {
       wsk = wsk->next;
     }
   }
   return false;
}

int throwDice(){
    return rand()%6+1;
}

void get_new_wares(){
    pthread_mutex_lock(&mutex_resources);
    int val = throwDice();
    char *type;
    switch(val){
    case 1:
    case 2:
        type = "gem";
        break;
    case 3:
    case 4:
        type = "food";
        break;
    default:
        type = "drink";
        break;
    }
    for(int i=val;i>0;i--){
        add_to_list(first, type);
    }
    printf("Shopkeeper gained %d %s!\n", val, type);
    pthread_mutex_unlock(&mutex_resources);
}

bool buy_wares(char *type){
    pthread_mutex_lock(&mutex_resources);
    int val = throwDice();
    for(int i=val;i>0;i--){
        if(!delete_from_list(first, type)){
            pthread_mutex_unlock(&mutex_resources);
            return false;
        }
    }
    pthread_mutex_unlock(&mutex_resources);
    return true;
}

void serve();

void *knight(void *vargp){
    usleep(500);
    pthread_exit(NULL);
    return NULL;
};

void *innkeeper(void *vargp){
    usleep(500);
    pthread_exit(NULL);
    return NULL;
};

void *lady(void *vargp){
    if(!buy_wares("gem")){
        ladies--;
        printf("Lady left the village!\n");
    }else{
        printf("Lady bought gem!\n");
    }
    usleep(500);
    pthread_exit(NULL);
    return NULL;
};

void *shopkeeper(void *vargp){
    get_new_wares();
    usleep(500);
    pthread_exit(NULL);
    return NULL;
};

void playDay(int d)
{
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;
    for(int i=1; i<=innkeepers; i++){
        pthread_create(&tid1, NULL, innkeeper, NULL);
    }
    for(int i=1; i<=knights; i++){
        pthread_create(&tid2, NULL, knight, NULL);
    }
    for(int i=1; i<=shopkeepers; i++){
        pthread_create(&tid3, NULL, shopkeeper, NULL);
    }
    for(int i=1; i<=ladies; i++){
        pthread_create(&tid4, NULL, lady, NULL);
    }
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
    first =(wares*) malloc (sizeof(wares));
    first->type = "gem";
    first->next = NULL;
    for (int i=gems;i>0;i--) {
        add_to_list(first,"gem");
    }
    for (int i=food;i>0;i--) {
        add_to_list(first,"food");
    }
    for (int i=drinks;i>0;i--) {
        add_to_list(first,"drink");
    }
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
