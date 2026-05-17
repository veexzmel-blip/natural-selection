#ifndef FOOD_HPP
#define FOOD_HPP
#include "object.hpp"
#include <QString>
class Grid;

class Food : public Object{
private:
    QString type;
public:

    static int MEAT_ENERGY;
    static int PLANT_ENERGY;

    Food(const QString& type);

    static float energyBank;

    static void createNew(const QString& type);

    static void createMeat(float x, float y, float energy);

    QString getTypeFood() const { return type; }

    Object::Type getType() const override { return Object::FOOD; }
};
#endif