#ifndef GENETICS_HPP
#define GENETICS_HPP

#include "animal.hpp"

#include <QString>
#include <QVector>

class Genetics{
public:

    static void calculateStats(Animal* animal);

    static void mutate( Animal* animal);

    static void inherit(Animal* parent, Animal* kid);

private:

    static QVector<QString> species;

    static QString createNewSp();
};

#endif