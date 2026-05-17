#include "animal.hpp"
#include "grid.hpp"
#include "animalCreator.hpp"
#include "food.hpp"
#include <cstdlib>
#include <QDebug>

Animal::Animal() :
    power(1.0f),
    size(1.0f),
    mutationFactor(0.1f),
    regeneration(1.0f),
    protection(1.0f),

    age(0.0f),
    maxAge(1000.0f),
    hp(100.0f),
    maxHp(100.0f),


    maxEnergy(1000.0f),
    energy(500.0f),
    bodyEnergy(300.0f),

    angle(static_cast<float>(rand() % 360) * M_PI / 180.0f),
    targetAngle(angle),
    speed(0.0f),

    hit(false)
{}

void Animal::update(){
    brain.think(this);
    move();
    eat("plant");
    eat("meat");
    energy -= lifecare;
    Food::energyBank += lifecare;
    hp += regeneration * 1;
    age += 0.01;
    attak();
    Grid::update(this);
    if(canReproduce()){
        int random = rand() % 3 + 1;
        if(random != 1){
            Animal* newAnimal = AnimalCreator::createMutant(this);
            newAnimal->setX(x+20);
            newAnimal->setY(y-10);
            Grid::addObject(newAnimal);
        }
        else{
            Animal* newAnimal = AnimalCreator::createSame(this);
            newAnimal->setX(x+20);
            newAnimal->setY(y-10);
            Grid::addObject(newAnimal);
        }
    }
}

void Animal::initRandom() {
    this->size = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / 1.5f);
    this->power = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / 1.5f);
    this->regeneration = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / 1.5f);
    this->protection = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / 1.5f);

    this->mutationFactor = 0.05f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / 0.20f);

    this->age = 0.0f;
    this->hp = this->maxHp;
    this->energy = this->maxEnergy / 2.0f;
}

void Animal::move() {
    float angleDiff = targetAngle - angle;

    angleDiff = std::atan2(std::sin(angleDiff), std::cos(angleDiff));

    if (std::abs(angleDiff) > maxRotationSpeed) {
        angle += (angleDiff > 0 ? 1.0f : -1.0f) * maxRotationSpeed;
    } else {
        angle = targetAngle;
    }

    angle = std::atan2(std::sin(angle), std::cos(angle));
    if (angle < 0) {
        angle += 2.0f * M_PI;
    }

    x += std::cos(angle) * speed * maxSpeed;
    y += std::sin(angle) * speed * maxSpeed;
}

void Animal::takeDamage(float damage){
    hp -= damage;
}

void Animal::attak(){
    if(hit == 1){
        Animal * target = Grid::getNearestAnimal(this->getX(), this->getY(), 10);
        if(target){
            target->takeDamage(damage);
        }
    }
    energy -= 1;
    Food::energyBank += 1;
}

bool Animal::isDead(){
    if(hp < 1 || energy < 1 || age >= maxAge){
        return 1;
    }else{
        return 0;
    }
    qDebug() << "animal death";
}

void Animal::eat(const QString& type){
    Food *newFood = Grid::getNearestFoodByType(x, y, 100, type);

    if (!newFood){
        return;
    }

    if(newFood->getTypeFood() == "plant"){
        energy += Food::PLANT_ENERGY;
        Grid::removeObject(newFood);
        if(energy > maxEnergy){
            float e = energy - maxEnergy;
            Food::energyBank += e;
            energy -= e;
        }
    }else{
        energy += Food::MEAT_ENERGY;
        Grid::removeObject(newFood);
        if(energy > maxEnergy){
            float e = energy - maxEnergy;
            Food::energyBank += e;
            energy -= e;
        }
    }
}

bool Animal::canReproduce(){
    if(energy > bornCare + 200){
        return 1;
    }else{
        return 0;
    }
}