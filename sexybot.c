#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <math.h>

#define LEADER 1
#define FOLLOWER 0
#define GRABBED_FREE 0
#define GRABBED_STOP 1
#define FREE 2
#define STOP 3
#define GRAB 4
#define PRE_GRAB 5
#define BALL 1
#define NOT_BALL 0
#define DOWN 0
#define UP 1

#define min(a,b) (a<=b?a:b)
#define max(a,b) (a>=b?a:b)

typedef struct status {
    bool status;
    pthread_mutex_t mutex;
} status;

typedef struct state {
    int state;
    pthread_mutex_t mutex;
} state;

typedef struct distance {
    int dist;
    pthread_mutex_t mutex;
} distance;

typedef struct color {
    bool is_ball;
    pthread_mutex_t mutex;
} color;

typedef struct angle {
    int angle;
    pthread_mutex_t mutex;
} angle;

typedef struct plier {
    bool state;
    pthread_mutex_t mutex;
} plier;

typedef struct position {
    int x;
    int y;
    pthread_mutex_t mutex;
} position;

struct state_arguments{
    state robot_state;
    distance robot_dist;
    color robot_color;
    plier robot_plier;
};

double to_radians(double degrees) {
    return degrees * (M_PI / 180.0);
}

int sign(int x){
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

void dodge(position robot_position, angle robot_angle, int target_x, int target_y){
    printf("Damn, I have to dodge !\n"); 
    return;
}

void moteur_avancer(int x, int y){
    printf("Moving toward your ass!\n");
}

void moteur_tourner(int x){
    printf("Turning toward your ass!\n");
}

void *update_dist(){
    while(1){
        printf("I should update the dist !!\n");
    }

    void* useless = NULL;
    return useless;
}

void *update_color(){
    while(1){
        printf("I'm blind right now !!\n");
    }

    void* useless = NULL;
    return useless;
}

void *update_angle(){
    while(1){
        printf("Turn turn turn  !!\n");
    }

    void* useless = NULL;
    return useless;
}

void *update_plier(){
    while(1){
        printf("Were is my arm ?\n");
    }

    void* useless = NULL;
    return useless;
}

void *update_state(void* arguments){
    struct state_arguments *arg = arguments;
    
    while (1){
        pthread_mutex_lock(&arg->robot_state.mutex);
        pthread_mutex_lock(&arg->robot_dist.mutex);
        pthread_mutex_lock(&arg->robot_color.mutex);
        pthread_mutex_lock(&arg->robot_plier.mutex);

        if ((arg->robot_plier.state == DOWN) && arg->robot_dist.dist > 20){
            arg->robot_state.state = GRABBED_FREE;
        } else if ((arg->robot_plier.state == DOWN) && arg->robot_dist.dist < 20){
            arg->robot_state.state = GRABBED_STOP;
        } else if (arg->robot_dist.dist > 20){
            arg->robot_state.state = FREE;
        } else if (arg->robot_color.is_ball != BALL){
            arg->robot_state.state = STOP;
        } else if (arg->robot_dist.dist < 10){
            arg->robot_state.state = GRAB;
        } else{
            arg->robot_state.state = PRE_GRAB;
        }
        printf("State = %d\n", arg->robot_state.state);
        pthread_mutex_unlock(&arg->robot_state.mutex);
        pthread_mutex_unlock(&arg->robot_dist.mutex);
        pthread_mutex_unlock(&arg->robot_color.mutex);
        pthread_mutex_unlock(&arg->robot_plier.mutex);
    }
    void* useless = NULL;
    return useless;
}


int main(int argc, char** argv){
    if (argc < 5){
        printf("Usage: %s <ball_position_x> <ball_position_y> <finish_position_x> <finish_position_y>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int WAITED = 0;
    int BALL_X = atoi(argv[1]);
    int BALL_Y = atoi(argv[2]);
    int FINISH_X = atoi(argv[3]);
    int FINISH_Y = atoi(argv[4]);
    int START = 0;
    int RANK, SIZE, PREV, NEXT;

    status robot_status;
    robot_status.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    state robot_state;
    robot_state.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    distance robot_dist;
    robot_dist.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    color robot_color;
    robot_color.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    angle robot_angle;
    robot_angle.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    plier robot_plier;
    robot_plier.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    position robot_position;
    robot_position.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

    struct state_arguments *arg = malloc(sizeof(struct state_arguments));

    arg->robot_state = robot_state;
    arg->robot_dist = robot_dist;
    arg->robot_color = robot_color;
    arg->robot_plier = robot_plier;

    pthread_t thread_dist, thread_color, thread_angle, thread_plier, thread_state;
    if(pthread_create(&thread_dist, NULL, update_dist, NULL) == -1) {
        perror("Creation of thread thread_dist failed !");
        return EXIT_FAILURE;
    }
    if(pthread_create(&thread_color, NULL, update_color, NULL) == -1) {
        perror("Creation of thread thread_color failed !");
        return EXIT_FAILURE;
    }
    if(pthread_create(&thread_angle, NULL, update_angle, NULL) == -1) {
        perror("Creation of thread thread_angle failed !");
        return EXIT_FAILURE;
    }
    if(pthread_create(&thread_plier, NULL, update_plier, NULL) == -1) {
        perror("Creation of thread thread_plier failed !");
        return EXIT_FAILURE;
    }
    if(pthread_create(&thread_state, NULL, update_state, arg) == -1) {
        perror("Creation of thread thread_state failed !");
        return EXIT_FAILURE;
    }

    while(1){
        if (START == 0){
            bluetooth msg = recuperer_msg();
            if (msg && msg.type == 3){ // START msg
                RANK = msg.rank;
                SIZE = msg.size;
                PREV = msg.prev;
                NEXT = msg.next;
                START = 1;
            }else {
                continue;
            }
        }

        bluetooth_msg = recuperer_msg();
        if (msg && msg.type == 4){ // STOP msg
            START = 0;
            continue;
        } else if(msg && msg.type == 7){ // KICK msg
            if ((RANK - 1) == msg.rank){ 
                PREV = msg.prev;
            } else if ((RANK + 1) == msg.rank){
                NEXT = msg.next
            } else if (RANK = msg.rank){
                START = 0;
                continue;
            }
        }

        pthread_mutex_lock(&robot_status.mutex);
        if (robot_status.status == LEADER){
            pthread_mutex_lock(&robot_state.mutex);
            pthread_mutex_unlock(&robot_status.mutex);

            switch(robot_state.state){
                case FREE:
                    WAITED = 0;
                    pthread_mutex_lock(&robot_position.mutex);
                    pthread_mutex_lock(&robot_angle.mutex);

                    if ((robot_position.y != BALL_Y) && (robot_angle.angle != 0)){
                        moteur_tourner(-1 * sign(robot_angle.angle) * min(5, abs(robot_angle.angle)));
                    } else if(robot_position.y != BALL_Y){
                        moteur_avancer(0,sign(BALL_Y - robot_position.y) * 2);
                    } else if((robot_position.x != BALL_X) && (abs(robot_angle.angle) != 90)){
                        moteur_tourner(sign(BALL_X) * min(5, abs(robot_angle.angle)));
                    } else {
                        moteur_avancer(sign(BALL_X - robot_position.x) *2, 0);
                    }
                    pthread_mutex_unlock(&robot_position.mutex);
                    pthread_mutex_unlock(&robot_angle.mutex);
                    break;

                case STOP:
                    if (WAITED > 20){
                        WAITED = 0;
                        dodge(robot_position, robot_angle, BALL_X, BALL_Y); //TODO
                    } else {
                        sleep(1);
                        WAITED += 1;
                    }
                    break;

                case GRAB:
                    //prendre_balle(); //TODO
                    break;

                case PRE_GRAB:
                    pthread_mutex_lock(&robot_position.mutex);
                    if (robot_position.y != BALL_Y){
                        moteur_avancer(0, sign(BALL_Y - robot_position.y) * 2);
                    } else {
                        moteur_avancer(sign(BALL_X - robot_postition.x) * 2, 0);
                    }
                    pthread_mutex_unlock(&robot_position.mutex);
                    break;

                case GRABBED_FREE:
                    WAITED = 0;
                    pthread_mutex_lock(&robot_position.mutex);
                    pthread_mutex_lock(&robot_angle.mutex);

                    if ((robot_position.y != FINISH_Y) && (robot_angle.angle != 0)){
                        moteur_tourner(-1 * sign(robot_angle.angle) * min(5, abs(robot_angle.angle)));
                    } else if(robot_position.y != FINISH_Y){
                        moteur_avancer(0, sign(FINISH_Y - robot_position.y) * 2);
                    } else if((robot_position.x != FINISH_X) && (abs(robot_angle.angle) != 90)){
                        moteur_tourner(sign(FINISH_X) * min(5, abs(robot_angle.angle)));
                    } else {
                        moteur_avancer(sign(FINISH_X - robot_position.x) * 2, 0);
                    }
                    pthread_mutex_unlock(&robot_position.mutex);
                    pthread_mutex_unlock(&robot_angle.mutex);
                    break;

                case GRABBED_STOP:
                    if (WAITED > 20){
                        WAITED = 0;
                        dodge(robot_position, robot_angle, BALL_X, BALL_Y); //TODO
                    } else {
                        sleep(1);
                        WAITED += 1;
                    }
                    break;

                default:
                    printf("Ohoh something went wrong ! The Robot has an undefined state :(\n");
            }

            pthread_mutex_unlock(&robot_state.mutex);
        } else if (robot_status.status == FOLLOWER){
            bluetooth msg = recuperer_msg();
            if (msg){
                switch (msg.type){
                    case 0: // ACTION msg
                        moteur_avancer(msg.dist * sin(to_radians(msg.angle)), msg.dist * cos(to_radians(msg.angle)));
                    case 2: // LEAD msg
                        sleep(15);
                        //play_zelda();
                        robot_status.status = LEADER;
                }
            }
        } else {
            perror("Unknown robot status !");
            return EXIT_FAILURE;
        }
        pthread_mutex_unlock(&robot_status.mutex);
    }

    return EXIT_SUCCESS;
}
