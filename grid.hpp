#ifndef GRID_HPP
#define GRID_HPP

#include "object.hpp"
class Food;
class Animal;

#include "object.hpp"
#include <QVector>
#include <QHash>
#include <QList>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <limits>

class Grid {
private:
    static const int CELL_SIZE = 100;
    static int width;
    static int height;
    static float worldWidth;
    static float worldHeight;

    static QHash<int, QList<Object*>> cells;

    static int getCellKey(int xSect, int ySect);

    static void getSector(float x, float y, int& xSect, int& ySect);

public:

    static float getDistance(Object* obj1, Object* obj2);

    static float getDistanceSq(Object* obj1, Object* obj2);

    static float getAngleTo(Object* from, Object* to);

    static void init(float worldW, float worldH);

    static void addObject(Object* obj);

    static void removeObject(Object* obj);

    static void update(Object* obj);

    static QVector<Object*> getObjectsInRadius(float x, float y, float radius);

    static QVector<Animal*> getAnimalsInRadius(float x, float y, float radius);

    static QVector<Food*> getFoodInRadius(float x, float y, float radius);

    static Food* getNearestFood(float x, float y, float maxRadius);

    static Food* getNearestFoodByType(float x, float y, float maxRadius, const QString& type);

    static void clear();

    static float getWorldWidth() { return worldWidth; }
    static float getWorldHeight() { return worldHeight; }

    static bool isInBounds(float x, float y);

    static void getRandomPosition(float& x, float& y);

    static Animal* getNearestAnimal(float x, float y, float maxRadius);
};
#endif