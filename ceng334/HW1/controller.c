#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <unistd.h>
#include <sys/wait.h>
#include "logging.h"
#include "message.h"

#define MINUS_ONE -1
typedef struct obstacle_t
{
    coordinate position;
    int durability;
    int pid;
    int fileno;
} obstacle_t;

typedef struct bomber_t
{
    coordinate position;
    int total_argument_count;
    char **exec_args;
    int pid;
    int fileno;
    int isAlive;
    int distanceFromBomb;
} bomber_t;

typedef struct bomb_t
{
    coordinate position;
    int interval;
    int radius;
    int pid;
    int fileno;
    int isExploded;
} bomb_t;

int i, map_width, map_height, obstacle_count, bomber_count;
int bomb_count = 0;
bomber_t *bomber_list;
obstacle_t *obstacle_list;
bomb_t *bomb_list;
struct pollfd *pfds_bomb;
struct pollfd *pfds;

// int static_bomber_count;

void printMap()
{
    for (int j = 0; j < map_height; j++)
    {
        for (int i = 0; i < map_width; i++)
        {
            int bomber_controller = 0;
            int bomb_controller = 0;
            int obstacle_controller = 0;
            for (int k = 0; k < bomber_count; k++)
            {
                if (bomber_list[k].position.x == i && bomber_list[k].position.y == j && bomber_list[k].isAlive == 1)
                {
                    printf("B ");
                    bomber_controller = 1;
                    break;
                }
            }

            for (int k = 0; k < bomb_count; k++)
            {
                if (bomb_list[k].position.x == i && bomb_list[k].position.y == j && bomb_list[k].isExploded == 0)
                {
                    printf("X ");
                    bomb_controller = 1;
                    break;
                }
            }
            if (bomb_controller)
                continue;
            for (int k = 0; k < obstacle_count; k++)
            {
                if (obstacle_list[k].position.x == i && obstacle_list[k].position.y == j)
                {
                    printf("%d ", obstacle_list[k].durability);
                    obstacle_controller = 1;
                    break;
                }
            }
            if (obstacle_controller)
                continue;
            ;
            if (!bomber_controller)
                printf("_ ");
        }
        printf("\n");
    }
}

void print_location(coordinate loc, char *arg)
{
    printf("%s: x: %d, y: %d\n", arg, loc.x, loc.y);
}

void pipeFunc(int *pid, char *args[], int *fileno)
{
    int fd[2];
    PIPE(fd);
    *pid = fork();
    *fileno = fd[0];
    if (*pid == -1)
    {
        exit(1);
    }
    else if (*pid == 0)
    {
        close(fd[0]);

        dup2(fd[1], STDIN_FILENO);
        dup2(fd[1], STDOUT_FILENO);

        close(fd[1]);
        execvp(args[0], args);
    }
    else
    {
    }
}

void getInput()
{
    int pos_x, pos_y, durability;
    int total_argument_count;
    scanf("%d %d", &map_width, &map_height);
    scanf("%d %d", &obstacle_count, &bomber_count);
    obstacle_list =
        (obstacle_t *)malloc(sizeof(obstacle_t) * obstacle_count);
    bomb_list = (bomb_t *)malloc(sizeof(bomb_t) * 2);
    pfds_bomb = (struct pollfd *)malloc(sizeof(struct pollfd) * 3);
    pfds = (struct pollfd *)malloc(sizeof(struct pollfd) * 3);
    // static_bomber_count = bomber_count;
    coordinate pos = {
        .x = 0,
        .y = 0};
    obstacle_t init = {
        .durability = 0,
        .position = pos};
    for (int i = 0; i < obstacle_count; i++)
    {
        scanf("%d %d", &pos_x, &pos_y);
        obstacle_list[i] = init;
        obstacle_list[i].position.x = pos_x;
        obstacle_list[i].position.y = pos_y;
        scanf("%d", &durability);
        obstacle_list[i].durability = durability;
    }

    bomber_list = (bomber_t *)malloc(sizeof(bomber_t) * bomber_count);

    bomber_t init2 = {
        .position = pos,
        .total_argument_count = 0,
        .exec_args = {0}};

    for (int i = 0; i < bomber_count; i++)
    {
        bomber_list[i] = init2;
        scanf("%d%d", &pos_x, &pos_y);
        bomber_list[i].position.x = pos_x;
        bomber_list[i].position.y = pos_y;
        scanf("%d", &total_argument_count);
        bomber_list[i].total_argument_count = total_argument_count;

        // printf("bomber pos x: %d\n,bomber pos y: %d\n,bomber total_argument_count: %d\n",bomber_list[i]->position.x,bomber_list[i]->position.y,bomber_list[i]->total_argument_count);

        bomber_list[i].exec_args =
            (char **)malloc(sizeof(char *) * (total_argument_count));

        for (int j = 0; j < total_argument_count; j++)
        {
            char *argument = (char *)malloc(sizeof(char) * 128);
            bomber_list[i].exec_args[j] = (char *)malloc(sizeof(char) * 128);
            scanf("%s", argument);
            bomber_list[i].exec_args[j] = argument;
        }
        pipeFunc(&(bomber_list[i].pid), bomber_list[i].exec_args,
                 &bomber_list[i].fileno);
    }
}

od *objectCounter(od *object_data_list, coordinate position, int *count)
{
    int pos_x = position.x;
    int pos_y = position.y;
    int left = 3;
    int right = 3;
    int up = 3;
    int down = 3;
    *count = 0;
    int leftObstacle;
    int rightObstacle;
    int upObstacle;
    int downObstacle;

    for (int i = 0; i < obstacle_count; i++)
    {
        if (obstacle_list[i].durability == 0)
            continue;
        int obs_pos_x = obstacle_list[i].position.x;
        int obs_pos_y = obstacle_list[i].position.y;
        int horizontal_diff = pos_x - obs_pos_x;
        int vertical_diff = pos_y - obs_pos_y;

        if (horizontal_diff > 0 && horizontal_diff <= 3 && vertical_diff == 0)
        {
            if (horizontal_diff - 1 < left)
            {
                left = horizontal_diff - 1;
                leftObstacle = i;
            }
        }

        if (horizontal_diff < 0 && horizontal_diff >= -3 && vertical_diff == 0)
        {
            int neg_diff = -horizontal_diff;
            if (neg_diff - 1 < right)
            {
                right = neg_diff - 1;
                rightObstacle = i;
            }
        }

        if (vertical_diff > 0 && vertical_diff <= 3 && horizontal_diff == 0)
        {
            if (vertical_diff - 1 < up)
            {
                up = vertical_diff - 1;
                upObstacle = i;
            }
        }
        if (vertical_diff < 0 && vertical_diff >= -3 && horizontal_diff == 0)
        {
            int neg_diff = -vertical_diff;
            if (neg_diff - 1 < down)
            {
                down = neg_diff - 1;
                downObstacle = i;
            }
        }
    }
    if (left != 3)
    {
        od new = {.position = obstacle_list[leftObstacle].position, .type = OBSTACLE};
        object_data_list[(*count)++] = new;
    }
    if (right != 3)
    {
        od new = {.position = obstacle_list[rightObstacle].position, .type = OBSTACLE};
        object_data_list[(*count)++] = new;
    }
    if (up != 3)
    {
        od new = {.position = obstacle_list[upObstacle].position, .type = OBSTACLE};
        object_data_list[(*count)++] = new;
    }
    if (down != 3)
    {
        od new = {.position = obstacle_list[downObstacle].position, .type = OBSTACLE};
        object_data_list[(*count)++] = new;
    }

    for (int i = 0; i < bomber_count; i++)
    {
        int bomber_pos_x = bomber_list[i].position.x;
        int bomber_pos_y = bomber_list[i].position.y;
        int horizontal_diff = pos_x - bomber_pos_x;
        int vertical_diff = pos_y - bomber_pos_y;
        if (horizontal_diff == 0 && vertical_diff == 0)
            continue; // himself

        if (horizontal_diff > 0 && horizontal_diff <= left && vertical_diff == 0)
        {
            od new = {.position = bomber_list[i].position, .type = BOMBER};
            object_data_list[(*count)++] = new;
            continue;
        }
        if (horizontal_diff < 0 && -horizontal_diff <= right && vertical_diff == 0)
        {
            od new = {.position = bomber_list[i].position, .type = BOMBER};
            object_data_list[(*count)++] = new;
            continue;
        }

        if (vertical_diff > 0 && vertical_diff <= up && horizontal_diff == 0)
        {
            od new = {.position = bomber_list[i].position, .type = BOMBER};
            object_data_list[(*count)++] = new;
            continue;
        }

        if (vertical_diff < 0 && -vertical_diff <= down && horizontal_diff == 0)
        {
            od new = {.position = bomber_list[i].position, .type = BOMBER};
            object_data_list[(*count)++] = new;
            continue;
        }
    }

    for (int i = 0; i < bomb_count; i++)
    {
        int bomb_pos_x = bomb_list[i].position.x;
        int bomb_pos_y = bomb_list[i].position.y;
        int horizontal_diff = pos_x - bomb_pos_x;
        int vertical_diff = pos_y - bomb_pos_y;
        if (horizontal_diff == 0 && vertical_diff == 0)
        {
            od new = {.position = bomb_list[i].position, .type = BOMB};
            object_data_list[(*count)++] = new;
            continue;
        }

        if (horizontal_diff > 0 && horizontal_diff <= left && vertical_diff == 0)
        {
            od new = {.position = bomb_list[i].position, .type = BOMB};
            object_data_list[(*count)++] = new;
            continue;
        }
        if (horizontal_diff < 0 && -horizontal_diff <= right && vertical_diff == 0)
        {
            od new = {.position = bomb_list[i].position, .type = BOMB};
            object_data_list[(*count)++] = new;
            continue;
        }

        if (vertical_diff > 0 && vertical_diff <= up && horizontal_diff == 0)
        {
            od new = {.position = bomb_list[i].position, .type = BOMB};
            object_data_list[(*count)++] = new;
            continue;
        }

        if (vertical_diff < 0 && -vertical_diff <= down && horizontal_diff == 0)
        {
            od new = {.position = bomb_list[i].position, .type = BOMB};
            object_data_list[(*count)++] = new;
            continue;
        }
    }

    return object_data_list;
}

coordinate moveBomber(coordinate target, int index)
{
    int target_pos_x = target.x; // 5
    int target_pos_y = target.y; // 4
    coordinate bomber_position = bomber_list[index].position;
    int bomber_pos_x = bomber_position.x; // 5
    int bomber_pos_y = bomber_position.y; // 5

    int horizontal_diff = target_pos_x - bomber_pos_x; // 0
    int vertical_diff = target_pos_y - bomber_pos_y;   //-1

    if (abs(horizontal_diff) > 1 || abs(vertical_diff) > 1)
        return bomber_position;

    if (abs(horizontal_diff) > 0 && abs(vertical_diff) > 0)
        return bomber_position;

    if (target_pos_x < 0 || target_pos_y < 0 || target_pos_x >= map_width || target_pos_y >= map_height)
        return bomber_position;

    for (int i = 0; i < bomber_count; i++)
    {
        if (target_pos_x == bomber_list[i].position.x && target_pos_y == bomber_list[i].position.y)
            return bomber_position;
    }

    for (i = 0; i < obstacle_count; i++)
    {
        if (target_pos_x == obstacle_list[i].position.x && target_pos_y == obstacle_list[i].position.y)
            return bomber_position;
    }

    bomber_list[index].position = target;

    return target;
}

int plantBomb(coordinate pos)
{
    for (int i = 0; i < bomb_count; i++)
    {
        if (bomb_list[i].position.x == pos.x && bomb_list[i].position.y == pos.y)
            return 0;
    }

    return 1;
}

int aliveCount()
{
    int count = 0;
    for (int i = 0; i < bomber_count; i++)
    {
        if (bomber_list[i].isAlive == 1)
            count++;
    }
    return count;
}

void bombExplode(coordinate position, int radius)
{
    int cnt = aliveCount();
    // printf("BOMBER COUNT IN BOMB EXPLIDE IS: %d\n", cnt);
    if (cnt == 1)
        return;
    int local_bomber_count = cnt;
    int pos_x = position.x;
    int pos_y = position.y;
    int left = radius;
    int right = radius;
    int up = radius;
    int down = radius;
    int leftObstacle;
    int rightObstacle;
    int upObstacle;
    int downObstacle;

    for (int i = 0; i < obstacle_count; i++)
    {
        if (obstacle_list[i].durability == 0)
            continue;
        int obs_pos_x = obstacle_list[i].position.x;
        int obs_pos_y = obstacle_list[i].position.y;
        int horizontal_diff = pos_x - obs_pos_x;
        int vertical_diff = pos_y - obs_pos_y;

        if (horizontal_diff > 0 && horizontal_diff <= radius && vertical_diff == 0)
        {
            if (horizontal_diff - 1 < left)
            {
                left = horizontal_diff - 1;
                leftObstacle = i;
            }
        }

        if (horizontal_diff < 0 && horizontal_diff >= -radius && vertical_diff == 0)
        {
            int neg_diff = -horizontal_diff;
            if (neg_diff - 1 < right)
            {
                right = neg_diff - 1;
                rightObstacle = i;
            }
        }

        if (vertical_diff > 0 && vertical_diff <= radius && horizontal_diff == 0)
        {
            if (vertical_diff - 1 < up)
            {
                up = vertical_diff - 1;
                upObstacle = i;
            }
        }
        if (vertical_diff < 0 && vertical_diff >= -radius && horizontal_diff == 0)
        {
            int neg_diff = -vertical_diff;
            if (neg_diff - 1 < down)
            {
                down = neg_diff - 1;
                downObstacle = i;
            }
        }
    }
    if (left != radius)
    {
        if (obstacle_list[leftObstacle].durability > 0)
        {
            obstacle_list[leftObstacle].durability--;
        }
    }
    if (right != radius)
    {
        if (obstacle_list[rightObstacle].durability > 0)
        {
            obstacle_list[rightObstacle].durability--;
        }
    }
    if (up != radius)
    {
        if (obstacle_list[upObstacle].durability > 0)
        {
            obstacle_list[upObstacle].durability--;
        }
    }
    if (down != radius)
    {
        if (obstacle_list[downObstacle].durability > 0)
        {
            obstacle_list[downObstacle].durability--;
        }
    }

    for (int i = 0; i < bomber_count; i++)
    {
        int bomber_pos_x = bomber_list[i].position.x;
        int bomber_pos_y = bomber_list[i].position.y;
        int horizontal_diff = pos_x - bomber_pos_x;
        int vertical_diff = pos_y - bomber_pos_y;

        if (horizontal_diff == 0 && vertical_diff == 0)
        {
            bomber_list[i].isAlive = 0;
            bomber_list[i].distanceFromBomb = 0;
            local_bomber_count--;
            continue; // bomb under the bomber
        }
        else if (horizontal_diff > 0 && horizontal_diff <= left && vertical_diff == 0)
        {
            bomber_list[i].isAlive = 0;
            local_bomber_count--;
            bomber_list[i].distanceFromBomb = horizontal_diff;
            continue;
        }
        else if (horizontal_diff < 0 && -horizontal_diff <= right && vertical_diff == 0)
        {
            bomber_list[i].isAlive = 0;
            local_bomber_count--;
            bomber_list[i].distanceFromBomb = -horizontal_diff;
            continue;
        }

        else if (vertical_diff > 0 && vertical_diff <= up && horizontal_diff == 0)
        {
            bomber_list[i].isAlive = 0;
            local_bomber_count--;
            bomber_list[i].distanceFromBomb = vertical_diff;

            continue;
        }

        else if (vertical_diff < 0 && -vertical_diff <= down && horizontal_diff == 0)
        {
            bomber_list[i].isAlive = 0;
            local_bomber_count--;
            bomber_list[i].distanceFromBomb = -vertical_diff;

            continue;
        }
        if (local_bomber_count == 1)
            break;
        if (local_bomber_count == 0)
        {
            // printf("LOCAL BOMBER COUNT IS 0\n");
            int max_distance = -1;
            int ind;
            for (int i = 0; i < bomber_count; i++)
            {
                if (max_distance < bomber_list[i].distanceFromBomb)
                {
                    max_distance = bomber_list[i].distanceFromBomb;
                    ind = i;
                }
            }
            bomber_list[ind].isAlive = 1;
            break;
        }
    }
}

void markBomb(int index)
{
    bomb_list[index].isExploded = 1;
}

void deleteBomb()
{
    while (1)
    {
        int control = 0;
        for (int i = 0; i < bomb_count; i++)
        {
            if (bomb_list[i].isExploded == 1)
            {
                for (int j = i; j < bomb_count - 1; j++)
                {
                    bomb_list[j] = bomb_list[j + 1];
                    pfds_bomb[j] = pfds_bomb[j + 1];
                }
                bomb_count--;
                bomb_list = (bomb_t *)realloc(bomb_list, sizeof(bomb_t) * (bomb_count));
                pfds_bomb = (struct pollfd *)realloc(pfds_bomb, sizeof(struct pollfd) * bomb_count);

                control = 1;
                break;
            }
        }
        if (!control)
            break;
    }
}

void removeBomber()
{
    for (int i = 0; i < bomber_count; i++)
    {
        // printf("BOMBER pid:  %d isAlive: %d\n", bomber_list[i].pid, bomber_list[i].isAlive);
    }
    while (1)
    {
        int control = 0;
        for (int i = 0; i < bomber_count; i++)
        {
            if (bomber_list[i].isAlive == 0)
            {
                for (int j = i; j < bomber_count - 1; j++)
                {
                    bomber_list[j] = bomber_list[j + 1];
                    pfds[j] = pfds[j + 1];
                }
                bomber_count--;
                bomber_list = (bomber_t *)realloc(bomber_list, sizeof(bomber_t) * (bomber_count));
                pfds = (struct pollfd *)realloc(pfds, sizeof(struct pollfd) * bomber_count);

                control = 1;
                break;
            }
        }
        if (!control)
            break;
    }
}

void getStarted()
{

    int x;
    int wr;
    pfds = (struct pollfd *)realloc(pfds, sizeof(struct pollfd) * bomber_count);

    for (int i = 0; i < bomber_count; i++)
    {
        bomber_list[i].isAlive = 1;
        pfds[i].events = POLLIN;
        pfds[i].fd = bomber_list[i].fileno;
        // printf("fileno bomber: %d\n", pfds[i].fd);
        bomber_list[i].distanceFromBomb = MINUS_ONE;
    }

    while (1)
    {

        if (bomber_count == 0 && bomb_count == 0)
            break;

        int activity_bomb = poll(pfds_bomb, bomb_count, 1);
        for (int i = 0; i < bomb_count; i++)
        {
            im incoming_message;
            if (pfds_bomb[i].revents & POLLIN)
            {
                // printf("bomb count: %d\n", bomb_count);

                x = read(pfds_bomb[i].fd, &incoming_message, sizeof(im));

                if (incoming_message.type == BOMB_EXPLODE)
                {

                    imp incoming_message_print = {.m = &incoming_message, .pid = bomb_list[i].pid};
                    print_output(&incoming_message_print, NULL, NULL, NULL);
                    bombExplode(bomb_list[i].position, bomb_list[i].radius);
                    markBomb(i); // marks the exploding bomb sending index

                    int stat;
                    waitpid(bomb_list[i].pid, &stat, WUNTRACED);
                    close(bomb_list[i].fileno);

                    for (int j = i; j < bomb_count - 1; j++)
                    {
                        bomb_list[j] = bomb_list[j + 1];
                        pfds_bomb[j] = pfds_bomb[j + 1];
                    }
                    bomb_count--;
                    bomb_list = (bomb_t *)realloc(bomb_list, sizeof(bomb_t) * (bomb_count + 5));
                    pfds_bomb = (struct pollfd *)realloc(pfds_bomb, sizeof(struct pollfd) * (bomb_count + 5));
                    i--;
                }
            }
        }
        // change the bomb list here

        int activity = poll(pfds, bomber_count, 1);
        int isNotAliveFlag = 0;

        for (int i = 0; i < bomber_count; i++)
        {

            // print_location(bomber_list[i].position, "BOMBER POS:");
            im incoming_message;
            if (pfds[i].revents & POLLIN)
            {

                x = read(pfds[i].fd, &incoming_message, sizeof(im));
                imp incoming_message_print = {.m = &incoming_message, .pid = bomber_list[i].pid};
                print_output(&incoming_message_print, NULL, NULL, NULL);

                if (bomber_list[i].isAlive)
                {
                    if (bomber_count == 1)
                    {

                        om outgoing_message = {
                            .type = BOMBER_WIN};
                        send_message(pfds[i].fd, &outgoing_message);
                        omp outgoing_message_print = {.m = &outgoing_message, .pid = bomber_list[i].pid};
                        print_output(NULL, &outgoing_message_print, NULL, NULL);
                        bomber_count--;
                        int stat;
                        waitpid(bomber_list[i].pid, &stat, WUNTRACED);
                        close(bomber_list[i].fileno);
                        break;
                    }
                    if (incoming_message.type == BOMBER_START)
                    {

                        om outgoing_message = {
                            .data.new_position = bomber_list[i].position,
                            .type = BOMBER_LOCATION};

                        wr =
                            write(pfds[i].fd, &outgoing_message,
                                  sizeof(outgoing_message));
                        omp outgoing_message_print = {.m = &outgoing_message, .pid = bomber_list[i].pid};

                        print_output(NULL, &outgoing_message_print, NULL, NULL);
                    }

                    else if (incoming_message.type == BOMBER_SEE)
                    {
                        // printf("bomber see \n");

                        od *object_data_list = (od *)malloc(sizeof(od) * 100); // realloc!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1

                        int *cnt = (int *)malloc(sizeof(int));

                        *cnt = 0;

                        object_data_list = objectCounter(object_data_list, bomber_list[i].position, cnt);

                        om outgoing_message = {.type = BOMBER_VISION, .data.object_count = *cnt};

                        send_message(pfds[i].fd, &outgoing_message);
                        if (*cnt != 0)
                            send_object_data(pfds[i].fd, *cnt, object_data_list);

                        omp outgoing_message_print = {.m = &outgoing_message, .pid = bomber_list[i].pid};
                        print_output(NULL, &outgoing_message_print, NULL, object_data_list);
                    }
                    else if (incoming_message.type == BOMBER_MOVE)
                    {

                        coordinate target = incoming_message.data.target_position;

                        coordinate where_to_move = moveBomber(target, i);

                        om outgoing_message = {.type = BOMBER_LOCATION, .data.new_position = where_to_move};

                        send_message(pfds[i].fd, &outgoing_message);

                        omp outgoing_message_print = {.m = &outgoing_message, .pid = bomber_list[i].pid};
                        print_output(NULL, &outgoing_message_print, NULL, NULL);
                    }
                    else if (incoming_message.type == BOMBER_PLANT)
                    {
                        // printf("bomber plant \n");

                        long interval = incoming_message.data.bomb_info.interval;
                        int radius = incoming_message.data.bomb_info.radius;
                        coordinate targetPos = bomber_list[i].position;
                        char arg[128];
                        sprintf(arg, "%ld", interval);
                        char **arguments = (char **)malloc(sizeof(char *) * 2);
                        arguments[0] = "./bomb";
                        arguments[1] = arg;

                        int isPlanted = plantBomb(targetPos);
                        if (isPlanted)
                        {
                            // printf("is planted\n");

                            bomb_count++;
                            // printf("BOMB COUNT!: %d\n", bomb_count);

                            bomb_list = (bomb_t *)realloc(bomb_list, sizeof(bomb_t) * (bomb_count + 5));
                            pfds_bomb = (struct pollfd *)realloc(pfds_bomb, sizeof(struct pollfd) * (bomb_count + 5));
                            bomb_list[bomb_count - 1].interval = interval;
                            bomb_list[bomb_count - 1].position = targetPos;
                            bomb_list[bomb_count - 1].radius = radius;

                            pipeFunc(&bomb_list[bomb_count - 1].pid, arguments, &bomb_list[bomb_count - 1].fileno);

                            pfds_bomb[bomb_count - 1].events = POLLIN;
                            pfds_bomb[bomb_count - 1].fd = bomb_list[bomb_count - 1].fileno;

                            // print_location(targetPos, "WHERE TO PUT BOMB");
                            // printf("fileno bomb!: %d\n", bomb_list[i].fileno);
                        }
                        om outgoing_message = {.type = BOMBER_PLANT_RESULT, .data.planted = isPlanted};
                        send_message(pfds[i].fd, &outgoing_message);
                        omp outgoing_message_print = {.m = &outgoing_message, .pid = bomber_list[i].pid};
                        print_output(NULL, &outgoing_message_print, NULL, NULL);
                    }
                }
                else // bomber is not alive
                {
                    // printf("else bomber dead \n");

                    // printf("BOMBER PID: %d IS NOT ALIVE\n", bomber_list[i].pid);
                    om outgoing_message = {.type = BOMBER_DIE};
                    send_message(pfds[i].fd, &outgoing_message);
                    omp outgoing_message_print = {.m = &outgoing_message, .pid = bomber_list[i].pid};
                    print_output(NULL, &outgoing_message_print, NULL, NULL);

                    int stat;
                    waitpid(bomber_list[i].pid, &stat, WUNTRACED);
                    close(bomber_list[i].fileno);
                    isNotAliveFlag = 1;

                    for (int j = i; j < bomber_count - 1; j++)
                    {
                        bomber_list[j] = bomber_list[j + 1];
                        pfds[j] = pfds[j + 1];
                    }
                    bomber_count--;
                    bomber_list = (bomber_t *)realloc(bomber_list, sizeof(bomber_t) * (bomber_count + 5));
                    pfds = (struct pollfd *)realloc(pfds, sizeof(struct pollfd) * (bomber_count + 5));
                    i--;
                }
            }

            // if (isNotAliveFlag)
        }
        // printf("BOMBER CUNT :%d\n", bomber_count);
        // printf("BOMB CUNT :%d\n", bomb_count);
        usleep(1000);
        // printf("\n");
        // printMap();
        // printf("\n\n");
        // sleep(1);
    }
}

int main()
{
    getInput();
    getStarted();

    return 0;
};
