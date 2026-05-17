#include "animalCreator.hpp"
#include "animal.hpp"
#include "genetics.hpp"
#include "grid.hpp"
#include "food.hpp"
#include <QDebug>

Animal* AnimalCreator::createBase() {
    Animal* newAnimal = new Animal();
    newAnimal->initRandom();
    Genetics::calculateStats(newAnimal);
    newAnimal->brain.initRandom();
    newAnimal->energy = newAnimal->maxEnergy * 0.7f;
    return newAnimal;
}

Animal* AnimalCreator::createSame(Animal * parent){
    Animal* newAnimal = new Animal();
    Genetics::inherit(parent, newAnimal);
    Genetics::calculateStats(newAnimal);
    parent->energy -= parent->bornCare + newAnimal->maxEnergy / 4;
    newAnimal->energy = newAnimal->maxEnergy / 4;
    newAnimal->hp = newAnimal->maxHp;
    return newAnimal;
}

Animal* AnimalCreator::createMutant(Animal * parent){
    Animal* newAnimal = new Animal();
    Genetics::inherit(parent, newAnimal);
    Genetics::mutate(newAnimal);
    Genetics::calculateStats(newAnimal);
    parent->energy -= parent->bornCare + newAnimal->maxEnergy / 4;
    newAnimal->energy = newAnimal->maxEnergy / 4;
    newAnimal->hp = newAnimal->maxHp;
    return newAnimal;
}

void AnimalCreator::death(Animal * animal){
    Food::energyBank += animal->energy;
    Food::createMeat(animal->getX(), animal->getY(), animal->bodyEnergy);
    Grid::removeObject(animal);
}