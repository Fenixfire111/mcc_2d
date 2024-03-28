#include <QPainter>
#include <QApplication>
#include "game.h"
#include <iostream>

Game::Game(QWidget *parent) : QWidget(parent), blocks(ROWS_OF_BLOCKS) {
    for (int i = 0; i < ROWS_OF_BLOCKS; i++) {
        for (int j = 0; j < COLUMNS_OF_BLOCKS; j++) {
            Block block(j*50, i*50);
            if (i < 12) { //cost
                block.setDestroyed(true);
            }
            blocks[i].push_back(block);
        }
    }

    human = std::make_unique<Human>();

    worldDx = 0;
    correctionX = 0;
    timerId = startTimer(DELAY);
}

Game::~Game() {

}

void Game::paintEvent(QPaintEvent *e) {

    Q_UNUSED(e);

    QPainter painter(this);

    drawObjects(&painter);
}

void Game::drawObjects(QPainter *painter) {

    for (int i = 0; i < ROWS_OF_BLOCKS; i++) {
        for (int j = 0; j < COLUMNS_OF_BLOCKS; j++) {
            if (!blocks[i][j].isDestroyed()) {
                painter->drawImage(blocks[i][j].getRect(), blocks[i][j].getImage());
            }
        }
    }

    painter->drawImage(human->getRect(), human->getImage());
}

void Game::moveObjects() {
    human->move();
    moveWorld();
}

void Game::timerEvent(QTimerEvent *event) {

    Q_UNUSED(event);

    checkCollision();
    moveObjects();

    if (human->jump > 0) {
        --human->jump;
    }

    repaint();
}

void Game::mousePressEvent(QMouseEvent *event) {
    auto [xB, yB] = convertPixelsToCoordinates(event->pos().x(), event->pos().y());

    auto [xH1, yH1] = convertPixelsToCoordinates(human->getRect().left(), human->getRect().top());
    auto [xH2, yH2] = convertPixelsToCoordinates(human->getRect().left(), human->getRect().bottom());
    auto [xH3, yH3] = convertPixelsToCoordinates(human->getRect().right(), human->getRect().top());
    auto [xH4, yH4] = convertPixelsToCoordinates(human->getRect().right(), human->getRect().bottom());

    if (checkСoordinates(xB, yB)) {
        if (event->button() == Qt::LeftButton) {
            blocks[yB][xB].setDestroyed(true);
        } else if (event->button() == Qt::RightButton &&
                   !(xB ==xH1 && yB == yH1) && !(xB ==xH2 && yB == yH2) &&
                   !(xB ==xH3 && yB == yH3) && !(xB ==xH4 && yB == yH4)){
            blocks[yB][xB].setDestroyed(false);
        }
    }
}

void Game::keyPressEvent(QKeyEvent *event) {

    int dx = 0;
    int dy = 0;

    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) {
        if (human->getRect().left() < 350) {
            worldDx = 1;
        } else {
            dx = -1;
            human->setDx(dx);
        }
    }else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) {
        if (human->getRect().right() > 700) {
            worldDx = -1;
        } else {
            dx = 1;
            human->setDx(dx);
        }
    }else if (event->key() == Qt::Key_Space || event->key() == Qt::Key_W) {
        dy = -1;
        human->setDy(dy);
        if (human->jump < 1 && human->stand) {
            human->jump = 35;
            human->stand = false;
        }
    }else if (event->key() == Qt::Key_Escape) {
        qApp->exit();
        human->setDx(dx);
    } else {
        QWidget::keyPressEvent(event);
    }
}

void Game::keyReleaseEvent(QKeyEvent *event) {
    int dx = 0;

    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) {
        dx = 0;
        human->setDx(dx);
        worldDx = 0;
    }else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) {
        dx = 0;
        human->setDx(dx);
        worldDx = 0;
    }
}

void Game::checkCollision() {

    int humanLeft = human->getRect().left();
    int humanHeight = human->getRect().height();
    int humanWidth = human->getRect().width();
    int humanTop = human->getRect().top();

    QPoint pointRightTop(humanLeft + humanWidth - 12, humanTop);
    QPoint pointLeftTop(humanLeft + 11, humanTop);
    QPoint pointLeftBottom(humanLeft + 11, humanTop + humanHeight - 1);
    QPoint pointRightBottom(humanLeft + humanWidth - 12, humanTop + humanHeight - 1);

    auto [xBL1, yBL1] = convertPixelsToCoordinates(pointLeftTop.x(), pointLeftTop.y());
    auto [xBL2, yBL2] = convertPixelsToCoordinates(pointLeftBottom.x(), pointLeftBottom.y());

    if (checkСoordinates(xBL1, yBL1) && checkСoordinates(xBL2, yBL2)) {
        if((!(blocks[yBL1][xBL1].isDestroyed()) || !(blocks[yBL2][xBL2].isDestroyed()))
            && (human->getDx() < 0 || worldDx > 0)) {
            human->setDx(0);
            worldDx = 0;
        }
    }

    auto [xBL3, yBL3] = convertPixelsToCoordinates(pointRightTop.x(), pointRightTop.y());
    auto [xBL4, yBL4] = convertPixelsToCoordinates(pointRightBottom.x(), pointRightBottom.y());

    if (checkСoordinates(xBL3, yBL3) && checkСoordinates(xBL4, yBL4)) {
        if((!(blocks[yBL3][xBL3].isDestroyed()) || !(blocks[yBL4][xBL4].isDestroyed()))
            && (human->getDx() > 0 || worldDx < 0)) {
            human->setDx(0);
            worldDx = 0;
        }
    }


    auto [xB3, yB3] = convertPixelsToCoordinates(pointLeftTop.x() + 1, pointLeftTop.y() - 1); //шире
    auto [xB4, yB4] = convertPixelsToCoordinates(pointRightTop.x() - 1, pointRightTop.y() - 1);

    if (checkСoordinates(xB3, yB3) && checkСoordinates(xB4, yB4)) {
        if(!(blocks[yB3][xB3].isDestroyed()) || !(blocks[yB4][xB4].isDestroyed())) {
            human->setDy(0);
            human->jump = 0;
        }
    }

    auto [xB1, yB1] = convertPixelsToCoordinates(pointLeftBottom.x() + 1, pointLeftBottom.y() + 1);
    auto [xB2, yB2] = convertPixelsToCoordinates(pointRightBottom.x() - 1, pointRightBottom.y() + 1);

    if (human->jump < 1) {
        if (checkСoordinates(xB1, yB1) && checkСoordinates(xB2, yB2)) {
            if(blocks[yB1][xB1].isDestroyed() && blocks[yB2][xB2].isDestroyed()) {
                human->setDy(1);
            }

            if(!(blocks[yB1][xB1].isDestroyed()) || !(blocks[yB2][xB2].isDestroyed())) {
                human->setDy(0);
                human->stand = true;
            }
        }
    }
}

bool Game::checkСoordinates(int x,int y) {
    return (x >= 0 && x < COLUMNS_OF_BLOCKS && y >= 0 && y < ROWS_OF_BLOCKS);
}

std::pair<int, int> Game::convertPixelsToCoordinates(int xp, int yp, bool enableCorrections) {
    if (enableCorrections) {
        return {(xp - correctionX) / 50, yp / 50};
    }
    return {xp / 50, yp / 50};
}

void Game::moveWorld() {
    for (int i = 0; i < ROWS_OF_BLOCKS; i++) {
        for (int j = 0; j < COLUMNS_OF_BLOCKS; j++) {
            blocks[i][j].move(worldDx);
        }
    }

    correctionX += worldDx;
}
