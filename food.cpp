#include "food.hpp"
#include "grid.hpp"
#include <QDebug>

float Food::energyBank = 0;
int Food::MEAT_ENERGY = 250;
int Food::PLANT_ENERGY = 100;

Food::Food(const QString& type): type(type){}

void Food::createNew(const QString& type){
    if(type == "plant"){
        while (energyBank >= PLANT_ENERGY){
            Food* newFood = new Food("plant");
            float x;
            float y;
            Grid::getRandomPosition(x, y);
            newFood->x = x;
            newFood->y = y;
            Grid::addObject(newFood);
            Food::energyBank -= Food::PLANT_ENERGY;
        }
    }else{
        while(energyBank >= PLANT_ENERGY){
            Food* newFood = new Food("meat");
            float x;
            float y;
            Grid::getRandomPosition(x, y);
            newFood->x = x;
            newFood->y = y;
            Grid::addObject(newFood);
            Food::energyBank -= Food::MEAT_ENERGY;
        }
    }

}

void Food::createMeat(float x, float y, float energy){
        if (energy <= 0) return;
        int meatCount = static_cast<int>(energy / MEAT_ENERGY);

        if (meatCount == 0 && energy > 0) {
            //qDebug() << energy;
            energyBank += energy;
            return;
        }

        float remainingEnergy = energy - (meatCount * MEAT_ENERGY);
        energyBank += remainingEnergy;

        for (int i = 0; i < meatCount; i++) {
            Food* meat = new Food("meat");

            float angle = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * M_PI;
            float distance = (static_cast<float>(rand()) / RAND_MAX) * 50.0f;
            float offsetX = cos(angle) * distance;
            float offsetY = sin(angle) * distance;

            meat->setX(x + offsetX);
            meat->setY(y + offsetY);

            meat->setX(std::max(0.0f, std::min(meat->getX(), Grid::getWorldWidth() - 0.01f)));
            meat->setY(std::max(0.0f, std::min(meat->getY(), Grid::getWorldHeight() - 0.01f)));

            Grid::addObject(meat);
        }
}


