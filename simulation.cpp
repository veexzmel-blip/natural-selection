#include "simulation.h"

#include "grid.hpp"
#include "animalcreator.hpp"
#include "food.hpp"
#include "animal.hpp"

#include <QDebug>

int Simulation::BASE_ANIMALS = 80;
int Simulation::BASE_ENERGY_BANK = 15000;
float Simulation::WIDTH = 10000;
float Simulation::HEIGHT = 8500;
QList<Animal*> Simulation::activeAnimals;

Simulation::Simulation(QObject* parent)
    : QObject(parent)
    , running(false)
    , frameCount(0)
    , lastAnimalCount(0)
    , lastPlantCount(0)
    , lastEnergyBank(0)
{
    timer = new QTimer(this);
    timer->setInterval(16);
    connect(timer, &QTimer::timeout, this, &Simulation::update);
}

void Simulation::start() {
    if (running) return;

    if (frameCount == 0) {
        initWorld();
    }

    running = true;
    timer->start();
    emit started();
}

void Simulation::pause() {
    if (!running) return;

    running = false;
    timer->stop();
    emit paused();
    qDebug() << "Simulation paused";
}

void Simulation::stop() {
    if (timer->isActive()) {
        timer->stop();
    }
    running = false;
    clearWorld();
    frameCount = 0;
    emit stopped();
}

void Simulation::reset() {
    bool wasRunning = running;
    if (running) {
        timer->stop();
        running = false;
    }

    clearWorld();

    frameCount = 0;
    Food::energyBank = BASE_ENERGY_BANK;

    initWorld();

    if (wasRunning) {
        running = true;
        timer->start();
    }

    emit reseted();
    emit updated();
    qDebug() << "Simulation reset";
}

void Simulation::togglePause() {
    if (running) {
        pause();
    } else {
        start();
    }
}

void Simulation::update() {
    frameCount++;

    if (frameCount % 3 != 1) {
        QList<Animal*> snapshot = activeAnimals;

        for (Animal* animal : snapshot) {
            if (activeAnimals.contains(animal)) {
                animal->update();
            }
        }
    }

    if (frameCount % 5 == 0) {
        Food::createNew("plant");
    }
    if (frameCount % 2 == 1) {
        QList<Animal*> toDelete;
        for (Animal* animal : activeAnimals) {
            if (animal && animal->isDead()) {
                toDelete.append(animal);
            }
        }

        for (Animal* animal : toDelete) {
            AnimalCreator::death(animal);
        }
    }

    if (activeAnimals.size() != lastAnimalCount) {
        lastAnimalCount = activeAnimals.size();
        emit animalCountChanged(lastAnimalCount);
    }

    if (std::abs(Food::energyBank - lastEnergyBank) > 0.1f) {
        lastEnergyBank = Food::energyBank;
        emit energyBankChanged(lastEnergyBank);
    }

    emit updated();

    emit updated();
    if(frameCount % 120 == 0){
        qDebug() << "animals:" << activeAnimals.size();
    }

}

void Simulation::clearWorld() {
    auto objects = Grid::getObjectsInRadius(
        Grid::getWorldWidth() / 2,
        Grid::getWorldHeight() / 2,
        std::max(Grid::getWorldWidth(), Grid::getWorldHeight())
        );

    for (Object* obj : objects) {
        delete obj;
    }

    Grid::clear();
    Food::energyBank = 0;

    activeAnimals.clear();
}

void Simulation::initWorld() {
    Grid::init(WIDTH, HEIGHT);

    for (int i = 0; i < BASE_ANIMALS; i++) {
        Animal* animal = AnimalCreator::createBase();
        float x, y;
        Grid::getRandomPosition(x, y);
        animal->setX(x);
        animal->setY(y);
        animal->angle = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * M_PI;

        Grid::addObject(animal);
    }

    Food::energyBank = BASE_ENERGY_BANK;
    Food::createNew("plant");

    qDebug() << "World initialized with" << BASE_ANIMALS << "animals";
}

Simulation::~Simulation() {
    stop();
}