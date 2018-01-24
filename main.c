#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

int innkeepers, knights, shopkeepers, ladies, seats, drinks, food, gems;
int taken_seats = 0;
bool closed = false;

pthread_mutex_t mutex_resources = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_tavern = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_seats = PTHREAD_MUTEX_INITIALIZER;
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
    srand(time(0));
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

int buy_wares(char *type){
    pthread_mutex_lock(&mutex_resources);
    if(!delete_from_list(first, type)){
        pthread_mutex_unlock(&mutex_resources);
        return 0;
    }
    pthread_mutex_unlock(&mutex_resources);
    return 1;
}

void serve(){
    pthread_mutex_lock(&mutex_seats);
    if(taken_seats>0){
        if(food>0){
            food--;
            printf("Innkeeper served knight with food!\n");
            taken_seats--;
            printf("Knight left tavern!\n");
        }else if(drinks>0){
            drinks--;
            printf("Innkeeper served knight with drink!\n");
            taken_seats--;
            printf("Knight left tavern!\n");
        }
    }
    pthread_mutex_unlock(&mutex_seats);
};

void visit_tavern(){
    pthread_mutex_lock(&mutex_seats);
    while(!closed){
        if(food==0 && drinks==0){
            pthread_mutex_unlock(&mutex_seats);
            printf("Knight left village!\n");
            knights--;
            break;
        }
        if(taken_seats<seats){
            pthread_mutex_unlock(&mutex_seats);
            printf("Knight went to the tavern and waits for innkeeper!\n");
            taken_seats++;
            break;
        }
    }
}

void move_out_knights(){
    pthread_mutex_lock(&mutex_seats);
    if(taken_seats>0){
        printf("%d Knights were not served and left tavern!\n", taken_seats);
        taken_seats=0;
    }
    pthread_mutex_unlock(&mutex_seats);
}

void open_tavern(){
    pthread_mutex_lock(&mutex_tavern);
    closed=false;
    pthread_mutex_unlock(&mutex_tavern);
}

void *tavern(void *vargp){
    pthread_mutex_lock(&mutex_tavern);
    usleep(100000);
    closed=true;
    printf("Tavern closed!\n");
    move_out_knights();
    pthread_mutex_unlock(&mutex_tavern);
    pthread_exit(NULL);
    return NULL;
}

void *knight(void *vargp){
    visit_tavern();
    usleep(100);
    pthread_exit(NULL);
    return NULL;
};

void *innkeeper(void *vargp){
    if(buy_wares("drink")==1){
        drinks++;
        printf("Innkeeper bought drinks!\n");
    }
    if(buy_wares("food")==1){
        food++;
        printf("Innkeeper bought food!\n");
    }
    while(!closed){
        serve();
    }
    printf("Innkeeper left tavern!\n");
    if(knights==0){
        innkeepers--;
        printf("Innkeeper left the village!\n");
    }
    usleep(100);
    pthread_exit(NULL);
    return NULL;
};

void *lady(void *vargp){
    if(shopkeepers==0 || buy_wares("gem")==0){
        ladies--;
        printf("Lady left the village!\n");
    }else{
        printf("Lady bought gem!\n");
    }
    usleep(100);
    pthread_exit(NULL);
    return NULL;
};

void *shopkeeper(void *vargp){
    get_new_wares();
    if(ladies==0 && innkeepers==0){
        shopkeepers--;
        printf("Shopkeeper left the village!\n");
    }
    usleep(100);
    pthread_exit(NULL);
    return NULL;
};

void playDay(int d)
{
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;
    pthread_t tid5;
    for(int i=1; i<=shopkeepers; i++){
        pthread_create(&tid1, NULL, shopkeeper, NULL);
    }
    for(int i=1; i<=innkeepers; i++){
        pthread_create(&tid2, NULL, innkeeper, NULL);
    }
    for(int i=1; i<=knights; i++){
        pthread_create(&tid3, NULL, knight, NULL);
    }
    for(int i=1; i<=ladies; i++){
        pthread_create(&tid4, NULL, lady, NULL);
    }
    pthread_create(&tid5, NULL, tavern, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);
    open_tavern();
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
    drinks = 0;
    food = 0;
    pthread_mutex_init(&mutex_resources, NULL);
    pthread_mutex_init(&mutex_tavern, NULL);
    pthread_mutex_init(&mutex_seats, NULL);
    pthread_mutex_init(&mutex_day, NULL);
    for (int d=1;d<=365;d++){
        if(innkeepers==0 && knights==0 && shopkeepers==0 && ladies==0){
            printf("Your village lasted %d days!\n", d-1);
            break;
        }
        printf("Tavern opened!\n");
        playDay(d);
        if(d==365){
            printf("Your village lasted with: \n");
            printf("%d knights!\n", knights);
            printf("%d ladies!\n", ladies);
            printf("%d innkeepers!\n", innkeepers);
            printf("%d shopkeepers!\n", shopkeepers);
        }
    }
    return 0;
}
