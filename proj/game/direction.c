#include "direction.h"

void getVector(Direction direction, int *vector) {
    switch (direction) {
        case UP:
            vector[0] = 0;
            vector[1] = -1;
            break;
        case DOWN:
            vector[0] = 0;
            vector[1] = 1;
            break;
        case LEFT:
            vector[0] = -1;
            vector[1] = 0;
            break;
        case RIGHT:
            vector[0] = 1;
            vector[1] = 0;
            break;
        case UP_LEFT:
            vector[0] = -1;
            vector[1] = -1;
            break;
        case UP_RIGHT:
            vector[0] = 1;
            vector[1] = -1;
            break;
        case DOWN_LEFT:
            vector[0] = -1;
            vector[1] = 1;
            break;
        case DOWN_RIGHT:
            vector[0] = 1;
            vector[1] = 1;
            break;
        default:
            printf("Invalid direction.\n");
    }
}
