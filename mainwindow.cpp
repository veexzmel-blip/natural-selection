#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include "canvas.h"
#include "simulation.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    Canvas *canvas = new Canvas();
    mainLayout->addWidget(canvas, 4);

    QVBoxLayout *settingsLayout = new QVBoxLayout();
    settingsLayout->setAlignment(Qt::AlignTop);
    mainLayout->addLayout(settingsLayout, 1);

    animalCountLabel = new QLabel("Животных: 0", this);
    energyBankLabel = new QLabel("Банк энергии: 0", this);
    frameCountLabel = new QLabel("Кадр: 0", this);

    settingsLayout->addWidget(animalCountLabel);
    settingsLayout->addWidget(energyBankLabel);
    settingsLayout->addWidget(frameCountLabel);

    QFrame *line1 = new QFrame(this);
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
    settingsLayout->addWidget(line1);

    settingsLayout->addWidget(new QLabel("Ширина карты:", this));
    widthSpinBox = new QSpinBox(this);
    widthSpinBox->setRange(1000, 50000);
    widthSpinBox->setValue(static_cast<int>(Simulation::WIDTH));
    widthSpinBox->setSingleStep(500);
    settingsLayout->addWidget(widthSpinBox);

    settingsLayout->addWidget(new QLabel("Высота карты:", this));
    heightSpinBox = new QSpinBox(this);
    heightSpinBox->setRange(1000, 50000);
    heightSpinBox->setValue(static_cast<int>(Simulation::HEIGHT));
    heightSpinBox->setSingleStep(500);
    settingsLayout->addWidget(heightSpinBox);

    settingsLayout->addWidget(new QLabel("Стартовые животные:", this));
    animalsSpinBox = new QSpinBox(this);
    animalsSpinBox->setRange(2, 1000);
    animalsSpinBox->setValue(Simulation::BASE_ANIMALS);
    settingsLayout->addWidget(animalsSpinBox);

    settingsLayout->addWidget(new QLabel("Начальная энергия:", this));
    energySpinBox = new QSpinBox(this);
    energySpinBox->setRange(0, 1000000);
    energySpinBox->setValue(Simulation::BASE_ENERGY_BANK);
    energySpinBox->setSingleStep(5000);
    settingsLayout->addWidget(energySpinBox);

    pauseButton = new QPushButton("Пауза", this);
    resetButton = new QPushButton("Пересоздать мир", this);

    resetButton->setStyleSheet("background-color: #bf360c; color: white; font-weight: bold;");

    settingsLayout->addWidget(pauseButton);
    settingsLayout->addWidget(resetButton);

    Simulation* sim = new Simulation(this);

    QObject::connect(sim, &Simulation::updated, canvas, &Canvas::drawScene);
    QObject::connect(pauseButton, &QPushButton::clicked, sim, &Simulation::togglePause);

    QObject::connect(resetButton, &QPushButton::clicked, this, [this, sim]() {
        Simulation::WIDTH = static_cast<float>(widthSpinBox->value());
        Simulation::HEIGHT = static_cast<float>(heightSpinBox->value());
        Simulation::BASE_ANIMALS = animalsSpinBox->value();
        Simulation::BASE_ENERGY_BANK = energySpinBox->value();
        sim->reset();
    });

    QObject::connect(sim, &Simulation::animalCountChanged, this, [this](int count) {
        animalCountLabel->setText(QString("Животных: %1").arg(count));
    });

    QObject::connect(sim, &Simulation::energyBankChanged, this, [this](float bank) {
        energyBankLabel->setText(QString("Банк энергии: %1").arg(static_cast<int>(bank)));
    });

    QObject::connect(sim, &Simulation::updated, this, [this, sim]() {
        frameCountLabel->setText(QString("Кадр: %1").arg(sim->getFrameCount()));
    });

    sim->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}