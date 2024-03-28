#include <QPainter>
#include <QApplication>
#include "game.h"

Game::Game(QWidget *parent)
    : QWidget(parent) {

    int k = 0;

    for (int i = 0; i < ROWS_OF_BLOCKS; i++) {
        for (int j = 0; j < COLUMNS_OF_BLOCKS; j++) {
            Block block(j*50, i*50);
            if (i < 12) {
                block.setDestroyed(true);
            }
            blocks.push_back(block);
            k++;
        }
    }

    human = new Human;

    timerId = startTimer(DELAY);
}

Game::~Game() {
    delete human;
}

void Game::paintEvent(QPaintEvent *e) {

    Q_UNUSED(e);

    QPainter painter(this);

    drawObjects(&painter);
}

void Game::drawObjects(QPainter *painter) {

    for (int i=0; i<N_OF_BLOCKS; i++) {
        if (!blocks[i].isDestroyed()) {
            painter->drawImage(blocks[i].getRect(), blocks[i].getImage());
        }
    }

    painter->drawImage(human->getRect(), human->getImage());
}

void Game::moveObjects() {
    human->move();
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
            blocks[yB*20 + xB].setDestroyed(true);
        } else if (event->button() == Qt::RightButton &&
                   !(xB ==xH1 && yB == yH1) && !(xB ==xH2 && yB == yH2) &&
                   !(xB ==xH3 && yB == yH3) && !(xB ==xH4 && yB == yH4)) {
            blocks[yB*20 + xB].setDestroyed(false);
        }
    }
}

void Game::keyPressEvent(QKeyEvent *event) {

    int dx = 0;
    int dy = 0;

    if (event->key() == Qt::Key_Left) {
        dx = -1;
        human->setDx(dx);
    }else if (event->key() == Qt::Key_Right) {
        dx = 1;
        human->setDx(dx);
    }else if (event->key() == Qt::Key_Space) {
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

void Game::keyReleaseEvent(QKeyEvent *e) {

    int dx = 0;

    switch (e->key()) {
    case Qt::Key_Left:
        dx = 0;
        human->setDx(dx);
        break;

    case Qt::Key_Right:
        dx = 0;
        human->setDx(dx);
        break;
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

    for (int i=0; i<N_OF_BLOCKS; i++) {
        if (!blocks[i].isDestroyed()) {
            if(blocks[i].getRect().contains(pointRightTop) && human->getDx() > 0) {
                human->setDx(0);
            }

            if(blocks[i].getRect().contains(pointRightBottom) && human->getDx() > 0) {
                human->setDx(0); // pizda
            }

            if(blocks[i].getRect().contains(pointLeftBottom) && human->getDx() < 0) {
                human->setDx(0);
            }

            if(blocks[i].getRect().contains(pointLeftTop) && human->getDx() < 0) {
                human->setDx(0);
            }
        }
    } // перебор массива

    auto [xB3, yB3] = convertPixelsToCoordinates(pointLeftTop.x() + 1, pointLeftTop.y() - 1); //шире
    auto [xB4, yB4] = convertPixelsToCoordinates(pointRightTop.x() - 1, pointRightTop.y() - 1);

    if (checkСoordinates(xB3, yB3) && checkСoordinates(xB4, yB4)) {
        if(!(blocks[yB3*20 + xB3].isDestroyed()) || !(blocks[yB4*20 + xB4].isDestroyed())) {
            human->setDy(0);
            human->jump = 0;
        }
    }

    auto [xB1, yB1] = convertPixelsToCoordinates(pointLeftBottom.x() + 1, pointLeftBottom.y() + 1);
    auto [xB2, yB2] = convertPixelsToCoordinates(pointRightBottom.x() - 1, pointRightBottom.y() + 1);

    if (human->jump < 1) {
        if (checkСoordinates(xB1, yB1) && checkСoordinates(xB2, yB2)) {
            if(blocks[yB1*20 + xB1].isDestroyed() && blocks[yB2*20 + xB2].isDestroyed()) {
                human->setDy(1);
            }

            if(!(blocks[yB1*20 + xB1].isDestroyed()) || !(blocks[yB2*20 + xB2].isDestroyed())) {
                human->setDy(0);
                human->stand = true;
            }
        }
    }
}

bool Game::checkСoordinates(int x,int y) {
    return (x >= 0 && x < 20 && y >= 0 && y < 15);
}

std::pair<int, int> Game::convertPixelsToCoordinates(int xp,int yp) {
    int x = xp / 50;
    int y = yp / 50;
    return {x,y};
}
