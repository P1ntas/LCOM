#include "spaceShip.h"

Spaceship* create_spaceship(int xpos, int ypos, float angle) {
    Spaceship *spaceship = (Spaceship *)malloc(sizeof(Spaceship));

    if (spaceship == NULL) return NULL;

    spaceship->xpos = xpos;
    spaceship->ypos = ypos;
    spaceship->angle = angle;
    spaceship->speed = 0;
    spaceship->speedFactor = 1; // responsavel pela aceleracao da nave
    spaceship->slowFactor = -1; // por default, a nave desacelera -1

    return spaceship;
}

void update_spaceship(Spaceship* spaceship) {
    spaceship->xpos += cos(spaceship->angle) * spaceship->speed;
    spaceship->ypos += sin(spaceship->angle) * spaceship->speed;

    if (spaceship->speed - spaceship->slowFactor < 0) spaceship->speed = 0;
    else spaceship->speed -= spaceship->slowFactor;
    
    /*
    if (spaceship->xpos > mode_info.XResolution) spaceship->xpos -= mode_info.XResolution;
    else if (spaceship->xpos < 0) spaceship->xpos += mode_info.XResolution;

    if (spaceship->ypos > mode_info.YResolution) spaceship->ypos -= mode_info.YResolution;
    else if (spaceship->ypos < 0) spaceship->ypos += mode_info.YResolution;
    */
}

void destroy_spaceship(Spaceship* spaceship) {
    if (spaceship == NULL) return;

    free(spaceship);
    spaceship = NULL;
}

void rotate_spaceship(Spaceship* spaceship, Direction direction){
    if (direction == LEFT) spaceship->angle -= 1;
    else if (direction == RIGHT) spaceship->angle += 1;
}

void accelerate_spaceship(Spaceship* spaceship) {
    spaceship->speed += spaceship->speedFactor;
}
