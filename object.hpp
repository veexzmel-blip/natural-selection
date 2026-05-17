#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object{
protected:
    float x;
    float y;
    int xSector;
    int ySector;

public:

    enum Type {ANIMAL, FOOD };

    float getX();
    float getY();
    int getXSect();
    int getYSect();

    virtual Type getType() const = 0;

    void setX(float newx);
    void setY(float newy);
    void setXSect(int newx);
    void setYSect(int newy);

    virtual ~Object() = default;
};
#endif