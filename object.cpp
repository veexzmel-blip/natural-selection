#include "object.hpp"

float Object::getX(){
    return x;
}
float Object::getY(){
    return y;
}
void Object::setX(float newx){
    x = newx;
}
void Object::setY(float newy){
    y = newy;
}
int Object::getXSect(){
    return xSector;
}

int Object::getYSect(){
    return ySector;
}

void Object::setXSect(int newx){
    xSector = newx;
}

void Object::setYSect(int newy){
    ySector = newy;
}

