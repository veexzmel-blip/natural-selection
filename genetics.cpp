#include "genetics.hpp"
#include <cstdlib>
#include <ctime>

QVector<QString> Genetics::species;


void Genetics::calculateStats(Animal* animal) {
    if (!animal) return;

    float cube = std::pow(animal->size, 3.0f);
    float square = std::pow(animal->size, 2.0f);

    animal->lifecare = (std::pow(cube, 0.75f) * 0.05f) + (animal->regeneration * 0.1f);

    animal->maxEnergy = cube * 300.0f + 200.0f;

    animal->maxHp = (cube * 50.0f) + (animal->protection * 50.0f);

    float muscleForce = std::pow(animal->size, 3.5f) * animal->power;

    animal->damage = animal->power * cube * 5.0f;

    animal->bornCare = animal->maxEnergy * 0.4f;

    animal->maxRotationSpeed = 0.5f * (1.0f / (1.0f + (animal->size * 0.5f)));

    animal->maxSpeed = (muscleForce / cube) * 5.0f;

    animal->maxRotationSpeed = 0.6f / (1.0f + animal->size * 0.7f);

    animal->damage = animal->power * cube * 5.0f;

    animal->bornCare = animal->maxEnergy * 0.7f;

}

void Genetics::mutate(Animal* animal){
    double random = static_cast<double>(std::rand()) / RAND_MAX;
    float power = 1.05;
    float invertedPower = 0.95;

    if(random < animal->mutationFactor){
        if(random > animal->mutationFactor /2){
            animal->power *= power * (random / 10 + 1);
        }else{
            animal->power *= invertedPower * (random / 10 + 1);
        }
    }
    random = static_cast<double>(std::rand()) / RAND_MAX;
    if(random < animal->mutationFactor){
        if(random > animal->mutationFactor /2){
            animal->maxAge *= power * (random / 10 + 1);
        }else{
            animal->maxAge *= invertedPower * (random / 10 + 1);
        }
    }
    random = static_cast<double>(std::rand()) / RAND_MAX;
    if(random < animal->mutationFactor){
        if(random > animal->mutationFactor /2){
            animal->mutationFactor *= power * (random / 10 + 1);
        }else{
            animal->mutationFactor *= invertedPower * (random / 10 + 1);
        }
    }
    random = static_cast<double>(std::rand()) / RAND_MAX;
    if(random < animal->mutationFactor){
        if(random > animal->mutationFactor /2){
            animal->size *= power * (random / 10 + 1);
        }else{
            animal->size *= invertedPower * (random / 10 + 1);
        }
    }
    random = static_cast<double>(std::rand()) / RAND_MAX;
    if(random < animal->mutationFactor){
        if(random > animal->mutationFactor /2){
            animal->regeneration *= power * (random / 10 + 1);
        }else{
            animal->regeneration *= invertedPower * (random / 10 + 1);
        }
    }
    animal->brain.mutate(animal->mutationFactor);
    animal->sp = createNewSp();
}

void Genetics::inherit(Animal* parent, Animal* kid){
    kid->maxAge = parent->maxAge;
    kid->power = parent->power;
    kid->protection = parent->protection;
    kid->mutationFactor = parent->mutationFactor;
    kid->regeneration = parent->regeneration;
    kid->size = parent->size;
    kid->bodyEnergy = parent->bornCare;
    kid->sp = parent->sp;
    kid->brain.inherit(parent->brain);
}

QString Genetics::createNewSp() {
    const QString characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    QString result;
    bool isUnique = false;
    while(!isUnique){
        result.clear();
        isUnique = true;
        for (int i = 0; i < 5; ++i) {
            result += characters[rand() % characters.size()];
        }
        for(auto it = Genetics::species.begin(); it != Genetics::species.end(); ++it){
            if(*it == result){
                isUnique = false;
                break;
            }
        }
    }
    species.push_back(result);
    return result;
}