#include "SceneTreeView.h"
#include "SceneView.h"
#include "ui_SceneView.h"
#include <QtWidgets>
#include "gameplay.h"
using namespace gameplay;


SceneTreeView::SceneTreeView(QWidget* parent) : QTreeView(parent),
    _sceneView(static_cast<SceneView*>(parent))
{
}

SceneTreeView::~SceneTreeView()
{
}

void SceneTreeView::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    Ui::SceneView* ui = _sceneView->ui();
    menu.addAction(ui->actionDelete_Node);

    menu.exec(event->globalPos());
}

void SceneTreeView::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
        if(event->modifiers() & Qt::ShiftModifier)
        {
            _sceneView->deleteItemsSelected();
        }
        break;
    }
}
