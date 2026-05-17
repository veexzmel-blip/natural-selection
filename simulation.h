#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <QTimer>
#include <QObject>
#include "grid.hpp"
#include "animalcreator.hpp"
#include "food.hpp"

class Simulation : public QObject {
    Q_OBJECT
public:
    explicit Simulation(QObject* parent = nullptr);
    ~Simulation();

    static QList<Animal*> activeAnimals;

    bool isRunning() const { return running; }
    int getAnimalCount() const;
    float getEnergyBank() const { return Food::energyBank; }
    int getFrameCount() const { return frameCount; }

    static int BASE_ANIMALS;
    static int BASE_ENERGY_BANK;
    static float WIDTH;
    static float HEIGHT;
    void pause();

public slots:
    void start();
    void stop();
    void reset();
    void togglePause();
    void clearWorld();

private slots:

    void update();

signals:
    void updated();
    void started();
    void paused();
    void stopped();
    void reseted();
    void animalCountChanged(int count);
    void plantCountChanged(int count);
    void energyBankChanged(float bank);

private:
    void initWorld();

    QTimer* timer;
    bool running;
    int frameCount;
    int lastAnimalCount;
    int lastPlantCount;
    float lastEnergyBank;
};

#endif // SIMULATION_H
