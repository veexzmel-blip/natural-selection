#ifndef CANVAS_H
#define CANVAS_H
#include <QWidget>
#include <QResizeEvent>
#include "camera.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:

    Canvas();

    float getRatio() const;

    void resizeEvent(QResizeEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
    camera cam;

private:
    float width;
    float height;

    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    QPoint mousePos;

public slots:
    void drawAnimal(QPainter& painter, float x, float y, float size, float rotation, QColor color);
    void drawPlant(QPainter& painter, float x, float y, float size, float rotation);
    void drawMeat(QPainter& painter, float x, float y, float size, float rotation);
    void drawScene();


};

#endif
