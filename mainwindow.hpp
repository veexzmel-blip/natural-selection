#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    QLabel *animalCountLabel;
    QLabel *energyBankLabel;
    QLabel *frameCountLabel;

    QPushButton *pauseButton;
    QPushButton *resetButton;

    QSpinBox *widthSpinBox;
    QSpinBox *heightSpinBox;
    QSpinBox *animalsSpinBox;
    QSpinBox *energySpinBox;
};
#endif // MAINWINDOW_H