#pragma once

#include <QListWidget>

#include "state.h"
#include "../notGUI/level.h"

class EditorState : public State
{
private:
    // Private class(es)
    struct BlockMemento
    {
        QPoint positionIndices;
        quint32 identifier;
    };

public:
    // Public constructor(s)
    EditorState(StateStack* stateStack, QMainWindow* window);

private:
    // Private method(s)
    void createActions();
    void createMenus();
    void createToolbar();
    void createListWidget();

    void saveMemento();
    void saveBlockMementos();
    void applyMemento();
    void resetMementos();

    // Private virtual method(s)
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
private slots:
    // Private slot(s)
    void updateHoverRect(QListWidgetItem* current);
    void newLevel();
    void open();
    void save();
    void saveAs();
    void cleanup();
    void undo();
    void redo();

private:
    // Private member(s)
    QString              mFilename;
    Level*               mLevel;
    QGraphicsPixmapItem* mHoverBlock;
    QListWidget*         mListWidget;

    QVector<QVector<BlockMemento>> mBlockMementos;
    qsizetype           mMementoIndex;

    QAction *mActNew, *mActOpen, *mActSave, *mActSaveAs, *mActQuit, *mActUndo, *mActRedo;
    QToolBar* mToolbar;
};
