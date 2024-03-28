#include "human.h"

Human::Human() {

    dx = 0;
    dy = 0;
    jump = 0;
    stand = true;

    image.load("human9.png");

    rect = image.rect();
    resetState();
}

Human::~Human() {

}

void Human::setDx(int x) {
    dx = x;
}

void Human::setDy(int y) {
    dy = y;
}

int Human::getDx() const {
    return dx;
}

int Human::getDy() const {
    return dy;
}

void Human::move() {

    int x = rect.x() + dx;
    int y = rect.top() + (dy*2);

    rect.moveTo(x, y);
}

void Human::resetState() {

    rect.moveTo(INITIAL_X, INITIAL_Y);
}

QRect Human::getRect() {

    return rect;
}

QImage & Human::getImage() {

    return image;
}
