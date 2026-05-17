#ifndef CAMERA_H
#define CAMERA_H
#include <QObject>

class camera: public QObject
{
    Q_OBJECT
public:
    float BASE_ZOOM;

    float screenWidth = 800.0f;
    float screenHeight = 600.0f;

    camera(float x = 0.0f, float y = 0.0f, float baseZoom = 100.0f, float zoom = 1.0f);
    float x;
    float y;
    float zoom;
    
    float getX()const {return x;}
    float getY()const {return y;}
    float getZoom()const {return zoom;}
    
    void setX(float newX);
    void setY(float newY);
    void setZoom(float newZoom);

    void worldToCameraCords(float &x, float &y);
public slots:

    void zoomIn();
    void zoomOut();
};

#endif // CAMERA_H
