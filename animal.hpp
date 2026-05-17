#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include "object.hpp"
#include "brain.hpp"
class Grid;
class AnimalCreator;

#include <QVector>
#include <cmath>
#include <QString>

class Animal : public Object{
public:
    //mutationable characteristics
    float power;
    float size;
    float mutationFactor;
    float regeneration;
    float maxAge;
    float protection;

    //secondary characteristics
    float maxEnergy;
    float maxHp;
    float maxRotationSpeed;
    float maxSpeed;
    float damage;
    float lifecare;
    float bornCare;
    float speedCare;
    float bodyEnergy;
    //

    Brain brain;

    //status
    float energy;
    float hp;
    float angle;
    float targetAngle;
    float speed;
    float age;
    bool  hit;

    QString sp;

    Animal();

    void update();

    void initRandom();

    void move();

    bool isDead();

    bool canReproduce();

    void eat(const QString& type);

    void attak();

    void takeDamage(float damage);

    Object::Type getType() const override { return Object::ANIMAL; }
};

#endif