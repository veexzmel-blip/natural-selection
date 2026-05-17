#include "brain.hpp"
#include "animal.hpp"
#include "grid.hpp"
#include "food.hpp"
#include <algorithm>
#include <cmath>
#include <chrono>
#include <QDebug>

int Brain::inputLayer = 64;
int Brain::hiddenLayer = 30;
int Brain::outputLayer = 3;

Brain::Brain(){}

void Brain::inherit(const Brain& parent) {
    this->weights1 = parent.weights1;
    this->weights2 = parent.weights2;
    this->bias1 = parent.bias1;
    this->bias2 = parent.bias2;

    this->inputLayer = parent.inputLayer;
    this->hiddenLayer = parent.hiddenLayer;
    this->outputLayer = parent.outputLayer;
}

void Brain::mutate(float factor){
    for (int i = 0; i < inputLayer; i++) {
        for (int j = 0; j < hiddenLayer; j++) {
            float random = static_cast<float>(rand()) / RAND_MAX;
            if (random < factor) {
                float mutationValue = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
                weights1[i][j] += mutationValue * factor;

                if (weights1[i][j] > 5.0f) weights1[i][j] = 5.0f;
                if (weights1[i][j] < -5.0f) weights1[i][j] = -5.0f;
            }
        }
    }

    for (int i = 0; i < hiddenLayer; i++) {
        for (int j = 0; j < outputLayer; j++) {
            float random = static_cast<float>(rand()) / RAND_MAX;
            if (random < factor) {
                float mutationValue = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
                weights2[i][j] += mutationValue * factor;

                if (weights2[i][j] > 5.0f) weights2[i][j] = 5.0f;
                if (weights2[i][j] < -5.0f) weights2[i][j] = -5.0f;
            }
        }
    }

    for (int j = 0; j < hiddenLayer; j++) {
        float random = static_cast<float>(rand()) / RAND_MAX;
        if (random < factor) {
            float mutationValue = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
            bias1[0][j] += mutationValue * factor;

            if (bias1[0][j] > 5.0f) bias1[0][j] = 5.0f;
            if (bias1[0][j] < -5.0f) bias1[0][j] = -5.0f;
        }
    }

    for (int j = 0; j < outputLayer; j++) {
        float random = static_cast<float>(rand()) / RAND_MAX;
        if (random < factor) {
            float mutationValue = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
            bias2[0][j] += mutationValue * factor;

            if (bias2[0][j] > 5.0f) bias2[0][j] = 5.0f;
            if (bias2[0][j] < -5.0f) bias2[0][j] = -5.0f;
        }
    }
}

void Brain::initRandom() {
    weights1.resize(inputLayer);
    for (int i = 0; i < inputLayer; ++i)
        weights1[i].resize(hiddenLayer);

    weights2.resize(hiddenLayer);
    for (int i = 0; i < hiddenLayer; ++i)
        weights2[i].resize(outputLayer);

    bias1.resize(1);
    bias1[0].resize(hiddenLayer);

    bias2.resize(1);
    bias2[0].resize(outputLayer);

    for (int i = 0; i < inputLayer; i++) {
        for (int j = 0; j < hiddenLayer; j++) {
            weights1[i][j] = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
        }
    }

    for (int i = 0; i < hiddenLayer; i++) {
        for (int j = 0; j < outputLayer; j++) {
            weights2[i][j] = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
        }
    }

    for (int j = 0; j < hiddenLayer; j++) {
        bias1[0][j] = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
    }

    for (int j = 0; j < outputLayer; j++) {
        bias2[0][j] = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
    }
}

static float sigmoid(float x) {
    if (x >= 0) {
        return 1.0f / (1.0f + std::exp(-x));
    } else {
        float exp_x = std::exp(x);
        return exp_x / (1.0f + exp_x);
    }
}

static float tanh(float x) {
    return std::tanh(x);
}

void Brain::think(Animal *animal) {
    if (!animal) return;
    QVector<float> input = prompt(animal);

    float hidden[30] = {0.0f};
    float output[3] = {0.0f};

    for (int j = 0; j < 30; ++j) {
        float sum = 0.0f;
        for (int i = 0; i < 64; ++i) {
            sum += input[i] * weights1[i][j];
        }
        sum += bias1[0][j];
        hidden[j] = std::max(sum, 0.0f);
    }

    for (int j = 0; j < 3; ++j) {
        float sum = 0.0f;
        for (int i = 0; i < 30; ++i) {
            sum += hidden[i] * weights2[i][j];
        }
        sum += bias2[0][j];
        output[j] = std::max(sum, 0.0f);
    }

    animal->targetAngle += tanh(output[0]) * M_PI;
    animal->speed = sigmoid(output[1]);

    if (output[2] > 0.7f) {
        animal->hit = true;
    } else {
        animal->hit = false;
    }
}

QVector<float> Brain::prompt(Animal *animal) {
    QVector<float> input;

    if (!animal) {
        input.resize(64);
        for(int i = 0; i < 64; i++){
            input.push_back(0.0f);
        }
        return input;
    }

    input.push_back(animal->energy / animal->maxEnergy);
    input.push_back(animal->hp / animal->maxHp);

    float nAngle = std::atan2(std::sin(animal->angle), std::cos(animal->angle)) / M_PI;
    input.push_back(nAngle);
    input.push_back(animal->damage / 50.0f);

    float visionRadius = 250.0f;

    auto getAngle = [animal](Object* other) {
        float absAngle = Grid::getAngleTo(animal, other);
        return std::atan2(std::sin(absAngle - animal->angle), std::cos(absAngle - animal->angle)) / M_PI;
    };

    QVector<Object*> nearbyObjects = Grid::getObjectsInRadius(animal->getX(), animal->getY(), visionRadius);

    QVector<Food*> foods;
    QVector<Animal*> allAnimals;
    foods.reserve(nearbyObjects.size());
    allAnimals.reserve(nearbyObjects.size());

    for (Object* obj : nearbyObjects) {
        if (!obj) continue;

        if (obj->getType() == Object::FOOD) {
            foods.push_back(static_cast<Food*>(obj));
        } else if (obj->getType() == Object::ANIMAL) {
            if (obj != animal) {
                allAnimals.push_back(static_cast<Animal*>(obj));
            }
        }
    }

    int foodCount = std::min<int>(foods.size(), 6);
    if (foodCount > 0) {
        std::nth_element(foods.begin(), foods.begin() + foodCount, foods.end(), [animal](Food* a, Food* b) {
            return Grid::getDistanceSq(animal, a) < Grid::getDistanceSq(animal, b);
        });
    }

    for (int i = 0; i < 6; ++i) {
        if (i < foodCount && foods[i] != nullptr) {
            Food* food = foods[i];
            input.push_back(getAngle(food));
            input.push_back(Grid::getDistance(animal, food) / visionRadius);
            input.push_back((food->getTypeFood() == "plant") ? 1.0f : -1.0f);
        } else {
            input.push_back(0.0f);
            input.push_back(1.0f);
            input.push_back(0.0f);
        }
    }

    int animalCount = std::min<int>(allAnimals.size(), 6);
    if (animalCount > 0) {
        std::nth_element(allAnimals.begin(), allAnimals.begin() + animalCount, allAnimals.end(), [animal](Animal* a, Animal* b) {
            return Grid::getDistanceSq(animal, a) < Grid::getDistanceSq(animal, b);
        });
    }

    for (int i = 0; i < 6; ++i) {
        if (i < animalCount && allAnimals[i] != nullptr) {
            Animal* other = allAnimals[i];
            input.push_back(getAngle(other));
            input.push_back(Grid::getDistance(animal, other) / visionRadius);
            input.push_back((animal->sp == other->sp) ? 1.0f : -1.0f);
            input.push_back(other->energy / other->maxEnergy);
            input.push_back(other->hp / other->maxHp);
            input.push_back(other->power / 10.0f);
            input.push_back(other->size / 10.0f);
        } else {
            input.push_back(0.0f);
            input.push_back(1.0f);
            for (int j = 0; j < 5; ++j) {
                input.push_back(0.0f);
            }
        }
    }
    return input;
}
