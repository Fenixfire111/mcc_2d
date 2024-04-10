#include "block.h"
#include <vector>

namespace {
const std::vector<std::string> BLOCK_IMAGE_PATHS= {
    "","images/grass_block.jpg", "images/dirt.jpg", "images/stone.jpg",
    "images/bedrock.jpg", "images/cobblestone.png", "images/tree.png"};
}


Block::Block() {
    destroyed = true;
    rect = image.rect();
    rect.translate(0, 0);
}

Block::Block(int x, int y, BlockType type) {
    destroyed = true;
    if (type != BlockType::EMPTY) {
        image.load(BLOCK_IMAGE_PATHS[static_cast<int>(type)].c_str());
        destroyed = false;
    }
    rect.setRect(x, y, SIZE, SIZE);
}

Block::~Block() {

}

QRect Block::getRect() {

    return rect;
}

void Block::setRect(QRect rct) {

    rect = rct;
}

QImage & Block::getImage() {

    return image;
}

BlockType Block::getType() {
    return type;
}

bool Block::isDestroyed() {

    return destroyed;
}

void Block::setDestroyed(bool destr) {

    destroyed = destr;
}

void Block::setType(BlockType blockType) {
    type = blockType;
    if (type != BlockType::EMPTY) {
        image.load(BLOCK_IMAGE_PATHS[static_cast<int>(type)].c_str());
    } else {
        destroyed = true;
    }
}

void Block::move(int dx, int dy) {
    int x = rect.x() + dx;
    int y = rect.top() + dy;

    rect.moveTo(x, y);
}

