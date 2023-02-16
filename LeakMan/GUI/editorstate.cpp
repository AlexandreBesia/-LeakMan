#include "editorstate.h"

#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>

#include "stateid.h"
#include "notGUI/level.h"
#include "notGUI/block.h"

// Public constructor(s)
EditorState::EditorState(StateStack* stateStack, QMainWindow* window)
    : State(stateStack, window)
    , mFilename("")
    , mLevel(new Level(QSize(52, 52), 10, 10))
    , mHoverBlock(nullptr)
    , mListWidget(nullptr)
{
    setMouseTracking(true);

    createActions();
    createMenus();
    createToolbar();

    mScene->addItem(mLevel);
    mHoverBlock = mScene->addPixmap(Block::getTypeInfo(Block::TypeBlock::Ground).pixmap);
    mHoverBlock->setZValue(1.f);

    createListWidget();
    resetMementos();
}

// Private method(s)
void EditorState::createActions()
{
    mActNew = new QAction(tr("&New"), this);
    mActNew->setShortcut(tr("Ctrl+N"));
    connect(mActNew, &QAction::triggered, this, &EditorState::newLevel);

    mActOpen = new QAction(tr("&Open"), this);
    mActOpen->setShortcut(tr("Ctrl+O"));
    connect(mActOpen, &QAction::triggered, this, &EditorState::open);

    mActSave = new QAction(tr("&Save"), this);
    mActSave->setShortcut(tr("Ctrl+S"));
    connect(mActSave, &QAction::triggered, this, &EditorState::save);

    mActSaveAs = new QAction(tr("Save &as"), this);
    mActSaveAs->setShortcut(tr("Ctrl+Alt+S"));
    connect(mActSaveAs, &QAction::triggered, this, &EditorState::saveAs);

    mActQuit = new QAction(tr("&Quit"), this);
    mActQuit->setShortcut(tr("Ctrl+Q"));
    connect(mActQuit, &QAction::triggered, this, &EditorState::cleanup);

    mActUndo = new QAction(tr("&Undo"), this);
    mActUndo->setShortcut(tr("Ctrl+Z"));
    mActUndo->setEnabled(false);
    connect(mActUndo, &QAction::triggered, this, &EditorState::undo);

    mActRedo = new QAction(tr("&Redo"), this);
    mActRedo->setShortcut(tr("Ctrl+Y"));
    mActRedo->setEnabled(false);
    connect(mActRedo, &QAction::triggered, this, &EditorState::redo);
}

void EditorState::createMenus()
{
    QMenu* fileMenu = mWindow->menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(mActNew);
    fileMenu->addAction(mActOpen);
    fileMenu->addSeparator();
    fileMenu->addAction(mActSave);
    fileMenu->addAction(mActSaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(mActQuit);

    QMenu* editMenu = mWindow->menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(mActUndo);
    editMenu->addAction(mActRedo);
}

void EditorState::createToolbar()
{
    mToolbar = new QToolBar(tr(""));
    mWindow->addToolBar(Qt::ToolBarArea::RightToolBarArea, mToolbar);
}

void EditorState::createListWidget()
{
    mListWidget = new QListWidget();
    const auto& blockInfo = Block::getBlockInfo();

    QListWidgetItem* currentItem = nullptr;
    for (const auto&[type, info] : blockInfo) {
        QListWidgetItem* item = new QListWidgetItem(QIcon(info.pixmap.scaledToWidth(12)), info.name);
        item->setData(Qt::UserRole, type);
        mListWidget->addItem(item);

        if (type == Block::TypeBlock::Ground) {
            currentItem = item;
        }
    }
    mListWidget->setCurrentItem(currentItem);
    updateHoverRect(currentItem);
    connect(mListWidget, &QListWidget::currentItemChanged, this, &EditorState::updateHoverRect);

    mToolbar->addWidget(mListWidget);
}

void EditorState::saveMemento()
{
    ++mMementoIndex;
    for (qsizetype i = mMementoIndex; i < mBlockMementos.size(); ++i) {
        mBlockMementos.remove(i);
    }

    saveBlockMementos();
    mActUndo->setEnabled(true);
}

void EditorState::saveBlockMementos()
{
    QVector<Block*> blocks = mLevel->getBlocks();
    LevelGrid* levelGrid = mLevel->getGrid();

    mBlockMementos.emplace_back();
    for (Block* block : blocks) {
        Tile tile = levelGrid->getClosestTile(block->pos());
        mBlockMementos.back().emplace_back(BlockMemento({ QPoint(tile.column, tile.row), block->getType() }));
    }
}

void EditorState::applyMemento()
{
    QVector<Block*> blocks = mLevel->getBlocks();
    LevelGrid* levelGrid = mLevel->getGrid();

    for (Block* block : blocks) {
        mLevel->removeBlockAt(block->pos().x(), block->pos().y());
    }

    QVector<BlockMemento>& blockMementos = mBlockMementos[mMementoIndex];
    for (const BlockMemento& memento : blockMementos) {
        QPointF pos = levelGrid->getTilePosition(memento.positionIndices.x(), memento.positionIndices.y());
        mLevel->addBlock(pos.x(), pos.y(), memento.identifier);
    }
}

void EditorState::resetMementos()
{
    mBlockMementos.clear();
    mMementoIndex = 0;

    saveBlockMementos();
}

// Private virtual method(s)
void EditorState::mouseMoveEvent(QMouseEvent *event)
{
    const Tile CLOSEST_TILE = mLevel->getGrid()->getClosestTile(event->pos());
    if (CLOSEST_TILE.column != -1 && CLOSEST_TILE.row != -1) {
        mHoverBlock->setPos(CLOSEST_TILE.position);
    }
}

void EditorState::mousePressEvent(QMouseEvent* event)
{
    Tile tile = mLevel->getGrid()->getClosestTile(event->pos());
    if (tile.column != -1 && tile.row != -1) {
        if (event->button() == Qt::MouseButton::LeftButton) {
            const qint32 IDENTIFIER = mListWidget->currentItem()->data(Qt::UserRole).toUInt();
            mLevel->removeBlockAt(tile.position.x(), tile.position.y());
            mLevel->addBlock(tile.position.x(), tile.position.y(), IDENTIFIER);
            saveMemento();
        }
        else if (event->button() == Qt::MouseButton::RightButton) {
            mLevel->removeBlockAt(tile.position.x(), tile.position.y());
            saveMemento();
        }
    }
}

// Private slot(s)
void EditorState::updateHoverRect(QListWidgetItem* current)
{
    if (current) {
        const QSize& tileSize = mLevel->getGrid()->getTileSize();
        Block::TypeInfo& typeInfo = Block::getTypeInfo(current->data(Qt::UserRole).toUInt());
        if (typeInfo.pixmap.size() != tileSize) {
            typeInfo.pixmap = typeInfo.pixmap.scaled(tileSize.width(), tileSize.height());
        }
        mHoverBlock->setPixmap(typeInfo.pixmap);
        mHoverBlock->setOpacity(0.25);
    }
}

void EditorState::newLevel()
{
    mFilename = "";

    mScene->removeItem(mLevel);
    mLevel = new Level(QSize(52, 52), 10, 10);
    mScene->addItem(mLevel);

    resetMementos();
}

void EditorState::open()
{
    const QString FILENAME = QFileDialog::getOpenFileName(this, tr("Open"), "./", tr("LeakMan level (*.lml)"));
    if (!FILENAME.isEmpty()) {
        mFilename = FILENAME;

        mScene->removeItem(mLevel);
        mLevel = Level::deserializeFrom(FILENAME);
        mScene->addItem(mLevel);

        resetMementos();
    }
}

void EditorState::save()
{
    if (mFilename.isEmpty()) {
        saveAs();
    }
    else {
        QFile data(mFilename);
        if (!data.open(QFile::WriteOnly | QFile::Truncate)) {
            QMessageBox::critical(this, tr("Error"), tr("Failed to open level file"));
        }
        QDataStream out(&data);
        out << *mLevel;
        data.close();
    }
}

void EditorState::saveAs()
{
    const QString FILENAME = QFileDialog::getSaveFileName(this, tr("Save as"), "./", tr("LeakMan level (*.lml)"));
    if (!FILENAME.isEmpty()) {
        mFilename = FILENAME;
        save();
    }
}

void EditorState::cleanup()
{
    requestStateRemove(StateID::Editor);
    mWindow->menuBar()->deleteLater();
    mToolbar->deleteLater();
    mScene->deleteLater();
    requestStatePush(StateID::Menu);
}

void EditorState::undo()
{
    if (mMementoIndex != 0) {
        --mMementoIndex;
        applyMemento();

        mActRedo->setEnabled(true);
    }

    mActUndo->setEnabled(mMementoIndex != 0);
}

void EditorState::redo()
{
    if (mMementoIndex + 1 != mBlockMementos.size()) {
        ++mMementoIndex;
        applyMemento();

        mActUndo->setEnabled(true);
    }

    mActRedo->setEnabled(mMementoIndex + 1 != mBlockMementos.size());
}
