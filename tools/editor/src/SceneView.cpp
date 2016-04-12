#include "SceneView.h"
#include "ui_SceneView.h"

SceneView::SceneView(QWidget* parent) : QWidget(parent),
    _ui(new Ui::SceneView),
    _editor(static_cast<EditorWindow*>(parent)),
    _scene(NULL)
{
    _ui->setupUi(this);
}

SceneView::~SceneView()
{
    delete _ui;
}

void SceneView::sceneChanged()
{
    _ui->treeWidget->clear();
    _scene = _editor->scene();

    // Scene ite (AKA root node)
    QTreeWidgetItem* sceneItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    QString sceneText(tr("Scene"));
    sceneItem->setText(0, sceneText);
    sceneItem->setIcon(0, QIcon(":/res/images/scene.png"));
    _ui->treeWidget->addTopLevelItem(sceneItem);

    /*
    for (Node* node = _scene->getFirstNode(); node != NULL; node = node->getNextSibling())
    {
        QTreeWidgetItem* nodeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
        QString nodeText(tr("Node"));
        nodeItem->setText(0, nodeText);
        nodeItem->setIcon(0, QIcon(":/res/images/scene-node.png"));
        sceneItem->addChild(nodeItem);

        x1x1xxqvisitNodeAddItem(node, nodeItem);
    }*/
}

void SceneView::visitNodeAddItem(Node* parent, QTreeWidgetItem* parentItem)
{
    for (Node* child = parent->getFirstChild(); child != NULL; child = child->getNextSibling())
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(QTreeWidgetItem::Type);
        QString nodeText(tr("Node"));
        item->setText(0, nodeText);
        item->setIcon(0, QIcon(":/res/images/scene-node.png"));
        parentItem->addChild(item);

        visitNodeAddItem(child, item);
    }
}
