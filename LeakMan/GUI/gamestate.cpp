#include "gamestate.h"

// Public constructor(s)

/**
 * @brief GameState::GameState Displays the game
 * @param stateStack
 * @param window
 */
GameState::GameState(StateStack* stateStack, QMainWindow* window)
    : State(stateStack, window)
{
    // Gets previous state camera information

    CalibrationState *calibrationState = dynamic_cast<CalibrationState*>(requestState(StateID::Calibration));
    int nbCamera = calibrationState->getNbCamera();

    requestStateRemove(StateID::Calibration);
    requestStatePush(StateID::LevelWindow);

    cptLastColor = 40; // 40 * 25 = 1000 = 1 second between 2 actions of a block
    levelState = nullptr;
    lblDeath = new QLabel("Nombre de mort : 0", this);
    lblDeath->move(54, 10);
    lblDeath->setStyleSheet("font-weight: bold;font-size:12pt;");

    w.setNbCamera(nbCamera);
    w.show();
}

// Public virtual method(s)
/**
 * @brief GameState::update Displays the number of death
 * @param dt
 * @return
 */
bool GameState::update(qreal dt)
{
    colorColliding();
    LevelState *levelState = dynamic_cast<LevelState *>(requestState(StateID::LevelWindow));

    if(levelState)
    {
        lblDeath->setText("Nombre de mort : " + QString::number(levelState->getDeath() - 1));
        lblDeath->adjustSize();
    }

    return true;
}

/**
 * @brief GameState::quit Return to main menu
 */
void GameState::quit()
{
    requestStateRemove(StateID::Game);
    requestStateRemove(StateID::LevelWindow);
    requestStatePush(StateID::Menu);
}

/**
 * @brief GameState::colorColliding Gets the colors percieved and add color bloc actions
 */
void GameState::colorColliding()
{
    if(true)
    {
        if(levelState == nullptr)
        {
            // get the level if not done already
            levelState = dynamic_cast<LevelState*>(requestState(StateID::LevelWindow));

            QVector<Block*> blocks = levelState->getLevel()->getBlocks();
            invalidPositions.reserve(blocks.size());
            for (Block* block : blocks) {
                invalidPositions.emplace_back(block->pos());
            }
        }
        else
        {
            // get the resources needed
            QRectF rectCollisions = levelState->stickmanGeometry();
            QPointF levelSize = levelState->levelSize();
            QImage img = w.getImg();
            QPointF playerPosition = QPointF(-rectCollisions.x() / levelSize.x(), -rectCollisions.y() / levelSize.y());

            int pixelsPerColor[] = {0, 0, 0, 0}; // set to zero, the will be incremented for each pixel of the color

            for(int i = img.width() * playerPosition.x(); i < img.width() * (playerPosition.x() + 0.1); i++)
            {
                for(int j = img.height() * playerPosition.y(); j < img.height() * (playerPosition.y() + 0.1); j++)
                {
                    // each pixel treated here
                    if(img.valid(i, j))
                    {
                        // get the pixel
                        QColor color = img.pixelColor(i,j);
                        color = color.convertTo(QColor::Hsv);

                        // check if the pixel is of one of our colors, increment the related counter
                        if(w.isRed(color)) // red
                        {
                            pixelsPerColor[0]++;
                        }
                        else if(w.isGreen(color)) // green
                        {
                            pixelsPerColor[1]++;
                        }
                        else if(w.isBlue(color)) // blue
                        {
                            pixelsPerColor[2]++;
                        }
                        else if(w.isYellow(color)) // yellow
                        {
                            pixelsPerColor[3]++;
                        }
                    }

                }
            }

            // then check if collision with one of the colors
            for(int i = 0; i < 4; i++)
            {
                Block* block = nullptr;
                if(pixelsPerColor[i] > 1000)
                {
                    qDebug() << i << " détecté!!!";
                    Block::TypeBlock typeBlock;
                    switch(i)
                    {
                    case 0:

                        // red action
                         typeBlock = Block::TypeBlock::Jump;
                        break;

                    case 1:
                        // green action
                        typeBlock = Block::TypeBlock::Run;
                        break;

                    case 2:
                        // blue action
                        typeBlock = Block::TypeBlock::Reverse;
                        break;

                    case 3:
                        // yellow action
                        typeBlock = Block::TypeBlock::Climb;
                        break;

                    default:
                        // nothing
                        break;
                    }
                    // simulate a collision
                    block = new Block(typeBlock);
                    block->applyEffect(*levelState->getStickman());
                    cptLastColor = 0; // collision just made, so we don't want another collision in the next frames

                    if (block) {
                        delete block;
                    }
                    break; // out of the for loop, we don't want multiple colors
                }
            }
        }
    }
    else
    {
        cptLastColor++;
    }
}

/**
 * @brief GameState::colorCollidingDebug Gets the colors percieved, add color bloc actions and
 * color the correct bloc in the grid
 */
void GameState::colorCollidingDebug()
{
    if(levelState == nullptr) // if no level got right now
    {
        // get one and get information about blocks of the level
        levelState = dynamic_cast<LevelState*>(requestState(StateID::LevelWindow));

        QVector<Block*> blocks = levelState->getLevel()->getBlocks();
        invalidPositions.reserve(blocks.size());
        for (Block* block : blocks) {
            invalidPositions.emplace_back(block->pos());
        }
    }
    else
    {
        int nbBlocksX = 10, nbBlocksY = 10; // fix values, 10x10 level

        // get the resources we need
        QPointF levelSize = levelState->levelSize();
        QImage img = w.getImg();

        // for each column
        for(int i = 0; i < nbBlocksX; i++)
        {
            // for each block in that column
            for(int j = 0; j < nbBlocksY; j++)
            {
                // each block treated here

                int pixelsPerColor[] = {0, 0, 0, 0};

                for(int coordX = img.width() * i / nbBlocksX; coordX < img.width() * (i + 1) / nbBlocksX; coordX++)
                {
                    for(int coordY = img.height() * j / nbBlocksY; coordY < img.height() * (j + 1) / nbBlocksY; coordY++)
                    {
                        // each pixel of the block is treated here
                        if(img.valid(coordX, coordY)) // avoid out of range and time loss
                        {
                            QColor color = img.pixelColor(coordX, coordY);
                            color = color.convertTo(QColor::Hsv);

                            // increment de counter of the color if the pixel is in the range of the color
                            if(w.isRed(color)) // red
                            {
                                pixelsPerColor[0]++;
                            }
                            else if(w.isGreen(color)) // green
                            {
                                pixelsPerColor[1]++;
                            }
                            else if(w.isBlue(color)) // blue
                            {
                                pixelsPerColor[2]++;
                            }
                            else if(w.isYellow(color)) // yellow
                            {
                                pixelsPerColor[3]++;
                            }
                        }
                    }
                }
                // all the pixels of the block have been treated, now check if there's enough color pixels to make an event

                uint32_t typeBlock = Block::TypeBlock::Climb;
                for(int iColor = 0; iColor < 4; iColor++) // for each counter
                {
                    if(pixelsPerColor[iColor] > 1000) // if there's at least 1000 pixels of the color
                    {
                        QString strColor = "";
                        switch(iColor) // get the color of the pixels
                        {
                        case 0:
                            // red action
                            strColor = "red";
                             typeBlock = Block::TypeBlock::Jump;
                            break;

                        case 1:
                            // green action
                            strColor = "green";
                            typeBlock = Block::TypeBlock::Run;
                            break;

                        case 2:
                            // blue action
                            strColor = "blue";
                            typeBlock = Block::TypeBlock::Reverse | Block::TypeBlock::Ground;
                            break;

                        case 3:
                            // yellow action
                            strColor = "yellow";

                            typeBlock = Block::TypeBlock::Climb;
                            break;

                        default:
                            // nothing
                            break;
                        }
                        qDebug() <<strColor + " x:" + QString::number(i) + ", y:" + QString::number(j);

                        break; // out of the for loop, we don't want multiple colors
                    }
                }

                // get the position of the block to go
                LevelGrid* grid = levelState->getLevel()->getGrid();
                QPointF closestPos = grid->getClosestTile(QPointF(i * levelSize.x() / nbBlocksX, j * levelSize.y() / nbBlocksY)).position;

                if (std::find(invalidPositions.begin(), invalidPositions.end(), closestPos) == invalidPositions.end())
                {
                    if(typeBlock == Block::TypeBlock::Climb)
                    {
                        // if no color, remove the block
                        levelState->getLevel()->removeBlockAt(closestPos.x(), closestPos.y());
                    }
                    else
                    {
                        // if color, replace the current block by the color block
                        levelState->getLevel()->removeBlockAt(closestPos.x(), closestPos.y());
                        levelState->getLevel()->addBlock(closestPos.x(), closestPos.y(), typeBlock);
                    }
                }
            }
        }
    }
}
