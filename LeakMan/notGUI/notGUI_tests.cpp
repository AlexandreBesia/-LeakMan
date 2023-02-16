#include <gtest/gtest.h>
// LOCAL INCLUDES
#include "notGUI/physicscomponent.h"
#include "notGUI/block.h"
#include "notGUI/level.h"
#include "notGUI/levelgrid.h"

// #include <QApplication>

TEST(notGUI, Add)
{
//    // "Must construct a QGuiApplication before a QPixmap"
//    int argc;
//    char **argv;
//    QApplication a( argc, argv);
//
//    // "QPixmap::scaled: Pixmap is a null pixmap"
//    QPixmap Jump("://img/jump_64x64.png"); // doesn't fix the error
//
//    /*
//     * BLOCK
//    */
//    Block* block = new Block(Block::Jump);
//    EXPECT_EQ(block->getTypeInfo(Block::Jump).name, "Jump");
//    delete block;
//
//    /*
//     * LEVEL
//     */
//    Level level = Level(QSize(2,2), 10, 10);
//    level.addBlock(3, 3, Block::Jump);
//    EXPECT_EQ(level.getBlocks(Block::Jump).empty(), false);
//    level.removeBlockAt(3, 3);
//    EXPECT_EQ(level.getBlocks(Block::Jump).empty(), true);

    /*
     * LEVEL GRID
    */
    /*
    LevelGrid grid = LevelGrid(QSize(2, 2), 10, 10);
    // We're looking for the closest tiles in position(50,50).
    // Except the grid is 20,20. So the closest tile is expected to be find at
    // position(20,20), size(2,2), colomn(10), row(10)
    EXPECT_EQ(grid.getClosestTile(QPointF(50,50)), Tile({QPointF(20,20),QSize(2,2),10,10}));

    // alt
    Tile tile1 = grid.getClosestTile(QPointF(50,50));
    Tile tile2 = Tile({QPointF(20,20),QSize(2,2),10,10});
    EXPECT_EQ((tile1), tile2);
    */
}
