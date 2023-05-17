#include "spaceShip.h"

int create_spaceship(int x, int y, SpaceShip* spaceship) {
    spaceship = (SpaceShip*) malloc(sizeof(SpaceShip));

    if (spaceship == NULL) return 1;

    spaceship->x = x;           // position
    spaceship->y = y;           // position
    spaceship->direction = UP;  // direction
    spaceship->speed = 0;       // speed
    spaceship->speedFactor = 1; // thruster
    spaceship->slowFactor = 1;  // slow
    spaceship->width = 60;
    spaceship->height = 40;

    return 0;
}

void update_spaceship() {
    if (spaceship == NULL) return;

    // ship moves forward

    int move[2];

    getVector(spaceship->direction, move);

    printf("x: %d, y: %d\n", spaceship->x, spaceship->y);

    spaceship->x += move[0] * spaceship->speed;
    spaceship->y += move[1] * spaceship->speed;

    // slow the ship down

    if (spaceship->speed - spaceship->slowFactor < 0) spaceship->speed = 0;
    else spaceship->speed -= spaceship->slowFactor;
    
    /*
    if (spaceship->xpos > mode_info.XResolution) spaceship->xpos -= mode_info.XResolution;
    else if (spaceship->xpos < 0) spaceship->xpos += mode_info.XResolution;

    if (spaceship->ypos > mode_info.YResolution) spaceship->ypos -= mode_info.YResolution;
    else if (spaceship->ypos < 0) spaceship->ypos += mode_info.YResolution;
    */
}

void destroy_spaceship() {
    if (spaceship == NULL) return;

    free(spaceship);
    spaceship = NULL;
}

void rotate_spaceship(uint8_t scancode){
    if (spaceship == NULL) return;

    switch (scancode) {
        case A_KEY:
            switch (spaceship->direction) {
                case UP:
                    spaceship->direction = UP_LEFT;
                    break;
                case UP_LEFT:
                    spaceship->direction = LEFT;
                    break;
                case LEFT:
                    spaceship->direction = DOWN_LEFT;
                    break;
                case DOWN_LEFT:
                    spaceship->direction = DOWN;
                    break;
                case DOWN:
                    spaceship->direction = DOWN_RIGHT;
                    break;
                case DOWN_RIGHT:
                    spaceship->direction = RIGHT;
                    break;
                case RIGHT:
                    spaceship->direction = UP_RIGHT;
                    break;
                case UP_RIGHT:
                    spaceship->direction = UP;
                    break;
                default:
                    break;
            }
            break;
        case D_KEY:
            switch (spaceship->direction) {
                case UP:
                    spaceship->direction = UP_RIGHT;
                    break;
                case UP_RIGHT:
                    spaceship->direction = RIGHT;
                    break;
                case RIGHT:
                    spaceship->direction = DOWN_RIGHT;
                    break;
                case DOWN_RIGHT:
                    spaceship->direction = DOWN;
                    break;
                case DOWN:
                    spaceship->direction = DOWN_LEFT;
                    break;
                case DOWN_LEFT:
                    spaceship->direction = LEFT;
                    break;
                case LEFT:
                    spaceship->direction = UP_LEFT;
                    break;
                case UP_LEFT:
                    spaceship->direction = UP;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void accelerate_spaceship() {
    spaceship->speed += spaceship->speedFactor;
}
/*
bool ship_collides_ast(SpaceShip* spaceship, Asteroid* asteroid) {
    if (spaceship->x + spaceship->width < asteroid->x) return false;
    if (spaceship->x > asteroid->x + asteroid->width) return false;
    if (spaceship->y + spaceship->height < asteroid->y) return false;
    if (spaceship->y > asteroid->y + asteroid->height) return false;

    return true;
}*/
