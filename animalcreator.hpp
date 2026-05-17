#ifndef ANIMAL_CREATOR_HPP
#define ANIMAL_CREATOR_HPP
class Animal;

class AnimalCreator{
public:

    static Animal* createBase();

    static Animal* createSame(Animal * parent);

    static Animal* createMutant(Animal * parent);

    static void death(Animal * animal);
};

#endif