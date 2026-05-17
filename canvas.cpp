#define _USE_MATH_DEFINES

#include "canvas.h"
#include "camera.h"
#include "grid.hpp"
#include "animal.hpp"
#include "food.hpp"

#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <cmath>

Canvas::Canvas() : width(0), height(0), cam(3000, 2500, 0.5, 1) {}


float Canvas::getRatio() const {
    if (height != 0) {
        return width / height;
    }
    return 1.0f;
}

void Canvas::resizeEvent(QResizeEvent *event) {
    width = event->size().width();
    height = event->size().height();
    QWidget::resizeEvent(event);
}

void Canvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    painter.fillRect(rect(), QColor(37, 41, 102));

    float radius = (std::max(width, height) / (cam.BASE_ZOOM * cam.zoom)) * 2.0;

    auto objects = Grid::getObjectsInRadius(cam.x, cam.y, radius);

    for (Object* obj : objects) {
        Food* food = dynamic_cast<Food*>(obj);
        if (food) {
            if (food->getTypeFood() == "plant") {
                drawPlant(painter, food->getX(), food->getY(), 5.0f, 0);
            } else if (food->getTypeFood() == "meat") {
                drawMeat(painter, food->getX(), food->getY(), 7.0f, 1);
            }
        }
    }

    for (Object* obj : objects) {
        Animal* animal = dynamic_cast<Animal*>(obj);
        if (animal) {
            drawAnimal(painter, animal->getX(), animal->getY(), animal->size, animal->angle, QColor(217, 180, 72));
        }
    }
}

void Canvas::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0) {
        cam.zoomIn();
    } else {
        cam.zoomOut();
    }
    update();
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mousePos = event->pos();
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->pos() - mousePos;

        float moveX = delta.x() / (cam.BASE_ZOOM * cam.zoom);
        float moveY = delta.y() / (cam.BASE_ZOOM * cam.zoom);

        cam.setX(cam.x - moveX);
        cam.setY(cam.y - moveY);

        mousePos = event->pos();
        update();
    }
}

void Canvas::drawAnimal(QPainter& painter, float x, float y, float size, float rotation, QColor color) {
    float screenX = x;
    float screenY = y;

    cam.worldToCameraCords(screenX, screenY);

    screenX += width / 2;
    screenY += height / 2;

    float scaledSize = size * 15.0f * cam.BASE_ZOOM * cam.zoom;

    painter.save();
    painter.translate(screenX, screenY);
    painter.rotate(rotation * 180.0f / M_PI);

    painter.setBrush(color);
    painter.setPen(QPen(Qt::black, 1));
    painter.drawEllipse(QPointF(0, 0), scaledSize, scaledSize * 0.8f);

    float eyeRadius = scaledSize * 0.22f;
    float pupilRadius = eyeRadius * 0.45f;

    float eyeOffsetX = scaledSize * 0.55f;
    float eyeOffsetY = scaledSize * 0.35f;

    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 1));

    painter.drawEllipse(QPointF(eyeOffsetX, -eyeOffsetY), eyeRadius, eyeRadius);
    painter.drawEllipse(QPointF(eyeOffsetX, eyeOffsetY), eyeRadius, eyeRadius);

    painter.setBrush(Qt::black);
    painter.setPen(Qt::NoPen);

    float offsetX2 = eyeOffsetX + (eyeRadius * 0.2f);
    painter.drawEllipse(QPointF(offsetX2, -eyeOffsetY), pupilRadius, pupilRadius);
    painter.drawEllipse(QPointF(offsetX2, eyeOffsetY), pupilRadius, pupilRadius);

    painter.restore();
}

void Canvas::drawPlant(QPainter& painter, float x, float y, float size, float rotation) {
    float screenX = x;
    float screenY = y;

    cam.worldToCameraCords(screenX, screenY);
    screenX += width / 2;
    screenY += height / 2;

    float scaledSize = size * cam.BASE_ZOOM * cam.zoom;
    float radius = 1.0f * scaledSize;

    painter.setBrush(QBrush(Qt::green));
    painter.setPen(QPen(Qt::darkGreen, 1));

    painter.drawEllipse(QPointF(screenX, screenY), radius, radius * 1.1f);
}

void Canvas::drawMeat(QPainter& painter, float x, float y, float size, float rotation) {
    float screenX = x;
    float screenY = y;

    cam.worldToCameraCords(screenX, screenY);
    screenX += width / 2;
    screenY += height / 2;

    float scaledSize = size * cam.BASE_ZOOM * cam.zoom;
    float radius = 1.0f * scaledSize;

    painter.setBrush(QBrush(Qt::red));
    painter.setPen(QPen(Qt::darkRed, 1));

    painter.drawEllipse(QPointF(screenX, screenY), radius, radius * 0.8f);
}

void Canvas::drawScene() {
    this->update();
}


