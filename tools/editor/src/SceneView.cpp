#include "SceneView.h"
#include "ui_SceneView.h"

SceneView::SceneView(QWidget* parent) : QWidget(parent),
    _ui(new Ui::SceneView), _editor(NULL), _scene(NULL)
{
    _ui->setupUi(this);
}

SceneView::~SceneView()
{
    delete _ui;
}

void SceneView::setEditor(EditorWindow* editor)
{
    _editor = editor;
}

void SceneView::sceneChanged()
{
    _scene = _editor->scene();
    _ui->treeWidget->clear();
    connect(_ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*, int )), this, SLOT(itemChanged(QTreeWidgetItem*,int)));

    QTreeWidgetItem* sceneItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    sceneItem->setText(0, tr("Scene"));
    sceneItem->setIcon(0, QIcon(":/res/images/scene.png"));
    _ui->treeWidget->addTopLevelItem(sceneItem);

    for (Node* node = _scene->getFirstNode(); node != NULL; node = node->getNextSibling())
    {
        QTreeWidgetItem* nodeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
        QString nodeText;
        const char* nodeId = node->getId();
        if (nodeId && strlen(nodeId) > 0)
            nodeText = QString(nodeId);
        else
            nodeText = QString(tr("Node"));

        nodeItem->setText(0, nodeText);
        nodeItem->setIcon(0, QIcon(":/res/images/scene-node.png"));
        nodeItem->setFlags(nodeItem->flags() | Qt::ItemIsEditable);
        // Associate the node to the item
        nodeItem->setData(0, Qt::UserRole, QVariant::fromValue((qlonglong)node));

        sceneItem->addChild(nodeItem);
        visitNodeAddItem(node, nodeItem);
    }
    sceneItem->setExpanded(true);
}

void SceneView::visitNodeAddItem(Node* parent, QTreeWidgetItem* parentItem)
{
    for (Node* node = parent->getFirstChild(); node != NULL; node = node->getNextSibling())
    {
        QTreeWidgetItem* nodeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
        QString nodeText;
        const char* nodeId = node->getId();
        if (nodeId && strlen(nodeId) > 0)
            nodeText = QString(nodeId);
        else
            nodeText = QString(tr("Node"));

        nodeItem->setText(0, nodeText);
        nodeItem->setIcon(0, QIcon(":/res/images/scene-node.png"));
        nodeItem->setFlags(nodeItem->flags() | Qt::ItemIsEditable);
        // Associate the node to the item
        nodeItem->setData(0, Qt::UserRole, QVariant::fromValue((qlonglong)node));

        parentItem->addChild(nodeItem);
        visitNodeAddItem(node, nodeItem);
    }
}

void SceneView::itemChanged(QTreeWidgetItem* item, int column)
{
    QVariant data = item->data(0, Qt::UserRole);
    Node* node = (Node*) data.toLongLong();
    node->setId((item->text(0)).toLatin1().constData());
}
