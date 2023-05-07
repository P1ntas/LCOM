#include "spaceShip.h"

SpaceShip* create_spaceship(int x, int y, float angle) {
    SpaceShip *spaceship = (SpaceShip *)malloc(sizeof(SpaceShip));

    if (spaceship == NULL) return NULL;

    spaceship->x = x;
    spaceship->y = y;
    spaceship->angle = angle;
    spaceship->speed = 0;
    spaceship->speedFactor = 1; // responsavel pela aceleracao da nave
    spaceship->slowFactor = -1; // por default, a nave desacelera -1
    spaceship->width = 64;
    spaceship->height = 64;

    return spaceship;
}

void update_spaceship(SpaceShip* spaceship) {
    spaceship->x += cos(spaceship->angle) * spaceship->speed;
    spaceship->y += sin(spaceship->angle) * spaceship->speed;

    if (spaceship->speed - spaceship->slowFactor < 0) spaceship->speed = 0;
    else spaceship->speed -= spaceship->slowFactor;
    
    /*
    if (spaceship->xpos > mode_info.XResolution) spaceship->xpos -= mode_info.XResolution;
    else if (spaceship->xpos < 0) spaceship->xpos += mode_info.XResolution;

    if (spaceship->ypos > mode_info.YResolution) spaceship->ypos -= mode_info.YResolution;
    else if (spaceship->ypos < 0) spaceship->ypos += mode_info.YResolution;
    */
}

void destroy_spaceship(SpaceShip* spaceship) {
    if (spaceship == NULL) return;

    free(spaceship);
    spaceship = NULL;
}

void rotate_spaceship(SpaceShip* spaceship, Direction direction){
    if (direction == LEFT) spaceship->angle -= 1;
    else if (direction == RIGHT) spaceship->angle += 1;
}

void accelerate_spaceship(SpaceShip* spaceship) {
    spaceship->speed += spaceship->speedFactor;
}

bool ship_collides_ast(SpaceShip* spaceship, Asteroid* asteroid) {
    if (spaceship->x + spaceship->width < asteroid->x) return false;
    if (spaceship->x > asteroid->x + asteroid->width) return false;
    if (spaceship->y + spaceship->height < asteroid->y) return false;
    if (spaceship->y > asteroid->y + asteroid->height) return false;

    return true;
}