#ifndef BRAIN_HPP
#define BRAIN_HPP

class Animal;
#include <QVector>
#include <cmath>
class Food;
class Grid;

class Brain{
private:
    static int inputLayer;
    static int hiddenLayer;
    static int outputLayer;

    QVector<QVector<float>> weights1;
    QVector<QVector<float>> weights2;

    QVector<QVector<float>> bias1;
    QVector<QVector<float>> bias2;

    static QVector<float> prompt(Animal *animal);

public:

    Brain();

    void init(Brain parent);

    void inherit(const Brain& parent);

    void mutate(float factor);

    void initRandom();

    void think(Animal *animal);

};
#endif