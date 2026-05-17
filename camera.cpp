#include "camera.h"

camera::camera(float x, float y, float baseZoom, float zoom)
    : BASE_ZOOM(baseZoom), screenWidth(800), screenHeight(600), x(x), y(y), zoom(zoom){}

void camera::setX(float newX){ x = newX;}
void camera::setY(float newY){ y = newY;}
void camera::setZoom(float newZoom){ zoom = newZoom;}

void camera::worldToCameraCords(float &wX, float &wY){
    wX = (wX - x) * BASE_ZOOM * zoom;
    wY = (wY - y) * BASE_ZOOM * zoom;
}

void camera::zoomIn() {
    setZoom(zoom * 1.1f);
}

void camera::zoomOut() {
    setZoom(zoom * 0.9f);
}
