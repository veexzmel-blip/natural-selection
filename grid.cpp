#include "grid.hpp"
#include "animal.hpp"
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include "food.hpp"
#include "simulation.h"

int Grid::width = 0;
int Grid::height = 0;
float Grid::worldWidth = 0;
float Grid::worldHeight = 0;
QHash<int, QList<Object*>> Grid::cells;

void Grid::init(float worldW, float worldH) {
    worldWidth = worldW;
    worldHeight = worldH;

    width = static_cast<int>(std::ceil(worldWidth / CELL_SIZE));
    height = static_cast<int>(std::ceil(worldHeight / CELL_SIZE));

    clear();
}

float Grid::getDistance(Object* obj1, Object* obj2) {
    if (!obj1 or !obj2) return -1.0f;

    float dx = obj1->getX() - obj2->getX();
    float dy = obj1->getY() - obj2->getY();

    return std::sqrt(dx * dx + dy * dy);
}

float Grid::getDistanceSq(Object* obj1, Object* obj2) {
    if (!obj1 or !obj2) return -1.0f;

    float dx = obj1->getX() - obj2->getX();
    float dy = obj1->getY() - obj2->getY();

    return dx * dx + dy * dy;
}

float Grid::getAngleTo(Object* from, Object* to) {
    if (!from || !to) return 0.0f;

    float dx = to->getX() - from->getX();
    float dy = to->getY() - from->getY();

    return std::atan2(dy, dx);
}

int Grid::getCellKey(int xSect, int ySect) {
    return ySect * width + xSect;
}

void Grid::getSector(float x, float y, int& xSect, int& ySect) {
    x = std::max(0.0f, std::min(x, worldWidth - 0.01f));
    y = std::max(0.0f, std::min(y, worldHeight - 0.01f));

    xSect = static_cast<int>(x / CELL_SIZE);
    ySect = static_cast<int>(y / CELL_SIZE);

    xSect = std::min(xSect, width - 1);
    ySect = std::min(ySect, height - 1);
}

void Grid::addObject(Object* obj) {
    if (!obj) return;

    if(obj->getType() == Object::ANIMAL){
        Simulation::activeAnimals.append(static_cast<Animal*>(obj));
    }

    float x = obj->getX();
    float y = obj->getY();

    if (!isInBounds(x, y)) {
        return;
    }

    int xSect, ySect;
    getSector(x, y, xSect, ySect);

    obj->setXSect(xSect);
    obj->setYSect(ySect);

    int key = getCellKey(xSect, ySect);
    cells[key].push_back(obj);
}

void Grid::removeObject(Object* obj) {
    if (!obj) return;

    if(obj->getType() == Object::ANIMAL){
        Simulation::activeAnimals.removeOne(static_cast<Animal*>(obj));
    }

    int xSect = obj->getXSect();
    int ySect = obj->getYSect();
    int key = getCellKey(xSect, ySect);

    auto it = cells.find(key);
    if (it != cells.end()) {
        it->removeOne(obj);

        if (it->isEmpty()) {
            cells.erase(it);
        }
    }
}

void Grid::update(Object* obj) {
    if (!obj) return;

    float x = obj->getX();
    float y = obj->getY();

    if (!isInBounds(x, y)) {
        if (x < 0) obj->setX(0);
        if (x >= worldWidth) obj->setX(worldWidth - 0.01f);
        if (y < 0) obj->setY(0);
        if (y >= worldHeight) obj->setY(worldHeight - 0.01f);

        x = obj->getX();
        y = obj->getY();
    }

    int newXSect, newYSect;
    getSector(x, y, newXSect, newYSect);

    int oldXSect = obj->getXSect();
    int oldYSect = obj->getYSect();

    if (newXSect != oldXSect || newYSect != oldYSect) {
        int oldKey = getCellKey(oldXSect, oldYSect);
        auto it = cells.find(oldKey);
        if (it != cells.end()) {
            it->removeOne(obj);
            if (it->isEmpty()) {
                cells.erase(it);
            }
        }

        obj->setXSect(newXSect);
        obj->setYSect(newYSect);
        int newKey = getCellKey(newXSect, newYSect);
        cells[newKey].push_back(obj);
    }
}

QVector<Object*> Grid::getObjectsInRadius(float x, float y, float radius) {
    QVector<Object*> result;

    if (!isInBounds(x, y)) return result;

    int centerXSect, centerYSect;
    getSector(x, y, centerXSect, centerYSect);

    int cellRadius = static_cast<int>(std::ceil(radius / CELL_SIZE)) + 1;

    int startX = std::max(0, centerXSect - cellRadius);
    int endX = std::min(width - 1, centerXSect + cellRadius);
    int startY = std::max(0, centerYSect - cellRadius);
    int endY = std::min(height - 1, centerYSect + cellRadius);

    float radiusSq = radius * radius;

    for (int sy = startY; sy <= endY; ++sy) {
        for (int sx = startX; sx <= endX; ++sx) {
            int key = getCellKey(sx, sy);
            auto it = cells.find(key);
            if (it != cells.end()) {
                for (Object* obj : it.value()) {
                    float dx = obj->getX() - x;
                    float dy = obj->getY() - y;
                    float distSq = dx * dx + dy * dy;

                    if (distSq <= radiusSq) {
                        result.push_back(obj);
                    }
                }
            }
        }
    }

    return result;
}

QVector<Animal*> Grid::getAnimalsInRadius(float x, float y, float radius) {
    QVector<Animal*> result;

    auto objects = getObjectsInRadius(x, y, radius);
    for (Object* obj : objects) {
        Animal* animal = dynamic_cast<Animal*>(obj);
        if (animal) {
            result.push_back(animal);
        }
    }

    return result;
}


void Grid::clear() {
    cells.clear();
}

bool Grid::isInBounds(float x, float y) {
    return x >= 0 && x < worldWidth && y >= 0 && y < worldHeight;
}

void Grid::getRandomPosition(float& x, float& y) {
    x = static_cast<float>(std::rand()) / RAND_MAX * worldWidth;
    y = static_cast<float>(std::rand()) / RAND_MAX * worldHeight;
}


QVector<Food*> Grid::getFoodInRadius(float x, float y, float radius) {
    QVector<Food*> result;

    auto objects = getObjectsInRadius(x, y, radius);
    for (Object* obj : objects) {
        Food* food = dynamic_cast<Food*>(obj);
        if (food) {
            float dx = food->getX() - x;
            float dy = food->getY() - y;
            float distance = std::sqrt(dx * dx + dy * dy);
            result.push_back(food);

        }
    }

    return result;
}

Food* Grid::getNearestFoodByType(float x, float y, float maxRadius, const QString& type) {
    QVector<Object*> objects = getObjectsInRadius(x, y, maxRadius);

    Food* nearest = nullptr;
    float minDistSq = maxRadius * maxRadius;

    for (Object* obj : objects) {
        Food* food = dynamic_cast<Food*>(obj);
        if (food && food->getTypeFood() == type) {
            float dx = obj->getX() - x;
            float dy = obj->getY() - y;
            float distSq = dx * dx + dy * dy;

            if (distSq < minDistSq) {
                minDistSq = distSq;
                nearest = food;
            }
        }
    }

    return nearest;
}

Animal* Grid::getNearestAnimal(float x, float y, float maxRadius) {
    auto objects = getObjectsInRadius(x, y, maxRadius);

    Animal* nearest = nullptr;
    float minDistSq = maxRadius * maxRadius;

    for (Object* obj : objects) {
        Animal* animal = dynamic_cast<Animal*>(obj);
        if (animal) {
            float dx = animal->getX() - x;
            float dy = animal->getY() - y;
            float distSq = dx * dx + dy * dy;

            if (distSq < minDistSq) {
                minDistSq = distSq;
                nearest = animal;
            }
        }
    }

    return nearest;
}

