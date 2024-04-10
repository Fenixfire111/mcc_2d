#include <QPainter>
#include <QApplication>
#include <iostream>
#include "game.h"

Game::Game(QWidget *parent) : QWidget(parent), inventory(), blocks(ROWS_OF_BLOCKS) {
    StandardWorldGeneration();

    human = std::make_unique<Human>();

    worldDx = 0;
    worldDy = 0;
    correctionX = 0;
    correctionY = 0;
    timerId = startTimer(DELAY);
}

Game::~Game() {

}

void Game::StandardWorldGeneration() {
    for (int i = 0; i < ROWS_OF_BLOCKS; i++) {
        for (int j = 0; j < COLUMNS_OF_BLOCKS; j++) {
            if (i < GROUND_LINE) {
                Block block(j*Block::getSize() - NORMALIZATION_X, i*Block::getSize() - NORMALIZATION_Y, BlockType::EMPTY);
                blocks[i].push_back(block);
            } else if (i >= GROUND_LINE && i < GROUND_LINE + 1) {
                Block block(j*Block::getSize() - NORMALIZATION_X, i*Block::getSize() - NORMALIZATION_Y, BlockType::GRASS_BLOCK);
                blocks[i].push_back(block);
            } else if (i >= GROUND_LINE + 1 && i < GROUND_LINE + 4) {
                Block block(j*Block::getSize() - NORMALIZATION_X, i*Block::getSize() - NORMALIZATION_Y, BlockType::DIRT);
                blocks[i].push_back(block);
            } else if (i >= GROUND_LINE + 4 && i < GROUND_LINE + 48) {
                Block block(j*Block::getSize() - NORMALIZATION_X, i*Block::getSize() - NORMALIZATION_Y, BlockType::STONE);
                blocks[i].push_back(block);
            } else if (i >= GROUND_LINE + 48) {
                Block block(j*Block::getSize() - NORMALIZATION_X, i*Block::getSize() - NORMALIZATION_Y, BlockType::BEDROCK);
                blocks[i].push_back(block);
            }
        }
    }
}

void Game::paintEvent(QPaintEvent *e) {

    Q_UNUSED(e);

    QPainter painter(this);

    drawObjects(&painter);
}

void Game::drawObjects(QPainter *painter) {

    for (int i = 0; i < ROWS_OF_BLOCKS; ++i) {
        for (int j = 0; j < COLUMNS_OF_BLOCKS; ++j) {
            if (!blocks[i][j].isDestroyed()) {
                painter->drawImage(blocks[i][j].getRect(), blocks[i][j].getImage());
            }
        }
    }

    painter->drawImage(human->getRect(), human->getImage());

    for (int i = 0; i < inventory.GetCountCells(); ++i) {
        painter->drawImage(inventory.At(i).GetRect(), inventory.At(i).GetImage());
    }
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
                   !(xB ==xH3 && yB == yH3) && !(xB ==xH4 && yB == yH4)) {
            if (inventory.GetMainCell().GetBlockType() != BlockType::EMPTY) {
                blocks[yB][xB].setDestroyed(false);
                blocks[yB][xB].setType(inventory.GetMainCell().GetBlockType());
            }
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
    } else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) {
        if (human->getRect().right() > 700) {
            worldDx = -1;
        } else {
            dx = 1;
            human->setDx(dx);
        }
    } else if (event->key() == Qt::Key_Space || event->key() == Qt::Key_W) {
        if (human->jump < 1 && human->stand) {
            human->jump = 40;
            human->stand = false;
            if (human->getRect().top() < 100) {
                worldDy = 1;
                human->setDy(0);
            } else {
                dy = -1;
                human->setDy(dy);
                worldDy = 0;
            }
        }
    } else if (event->key() == Qt::Key_Escape) {
        qApp->exit();
        human->setDx(dx);
    } else if (event->key() == Qt::Key_1) {
        inventory.SetMainCell(0);
    } else if (event->key() == Qt::Key_2) {
        inventory.SetMainCell(1);
    } else if (event->key() == Qt::Key_3) {
        inventory.SetMainCell(2);
    } else if (event->key() == Qt::Key_4) {
        inventory.SetMainCell(3);
    } else if (event->key() == Qt::Key_5) {
        inventory.SetMainCell(4);
    } else if (event->key() == Qt::Key_6) {
        inventory.SetMainCell(5);
    } else if (event->key() == Qt::Key_7) {
        inventory.SetMainCell(6);
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
            worldDy = 0;
        }
    }

    auto [xB1, yB1] = convertPixelsToCoordinates(pointLeftBottom.x() + 1, pointLeftBottom.y() + 1);
    auto [xB2, yB2] = convertPixelsToCoordinates(pointRightBottom.x() - 1, pointRightBottom.y() + 1);

    if (human->jump < 1) {
        if (checkСoordinates(xB1, yB1) && checkСoordinates(xB2, yB2)) {
            if(blocks[yB1][xB1].isDestroyed() && blocks[yB2][xB2].isDestroyed()) {
                if (human->getRect().bottom() > 550) {
                    worldDy = -1;
                    human->setDy(0);
                } else {
                    human->setDy(1);
                    worldDy = 0;
                }

            }

            if(!(blocks[yB1][xB1].isDestroyed()) || !(blocks[yB2][xB2].isDestroyed())) {
                human->setDy(0);
                worldDy = 0;
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
        return {(xp + NORMALIZATION_X - correctionX) / 50, (yp + NORMALIZATION_Y - correctionY) / 50};
    }
    return {xp / 50, yp / 50};
}

void Game::moveWorld() {
    for (int i = 0; i < ROWS_OF_BLOCKS; i++) {
        for (int j = 0; j < COLUMNS_OF_BLOCKS; j++) {
            blocks[i][j].move(worldDx, 2*worldDy);
        }
    }

    correctionX += worldDx;
    correctionY += 2*worldDy;
}
