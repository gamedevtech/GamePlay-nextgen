#include "SceneView.h"
#include "ui_SceneView.h"

#include <QMenu>

SceneView::SceneView(QWidget* parent) : QWidget(parent),
    _ui(new Ui::SceneView), _editor(NULL), _scene(NULL)
{
    _ui->setupUi(this);

    _ui->lineEditSearch->addAction(QIcon(":/res/images/search.png"), QLineEdit::LeadingPosition);

    QMenu* addMenu = new QMenu();

    _ui->toolButtonAdd->setObjectName("addContextMenu");
    _ui->toolButtonAdd->setPopupMode(QToolButton::MenuButtonPopup);
    _ui->toolButtonAdd->setMenu(addMenu);
    addMenu->addAction(_ui->actionAdd_Node);
    addMenu->addAction(_ui->actionAdd_Cube);
    addMenu->addAction(_ui->actionAdd_Cone);
    addMenu->addAction(_ui->actionAdd_Cylinder);
    addMenu->addAction(_ui->actionAdd_Sphere);
    addMenu->addAction(_ui->actionAdd_Camera);
    addMenu->addAction(_ui->actionAdd_DirectionalLight);
    addMenu->addAction(_ui->actionAdd_PointLight);
    addMenu->addAction(_ui->actionAdd_SpotLight);
    addMenu->addAction(_ui->actionAdd_Terrain);
    addMenu->addAction(_ui->actionAdd_ParticleEmitter);
    addMenu->addAction(_ui->actionAdd_Sprite);
    addMenu->addAction(_ui->actionAdd_Text);
    addMenu->addAction(_ui->actionAdd_Form);
    addMenu->addAction(_ui->actionAdd_Audio);
    addMenu->addAction(_ui->actionAdd_Video);
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

    for (Node* node = _scene->getFirstNode(); node != NULL; node = node->getNextSibling())
    {
        QTreeWidgetItem* item = createTreeItem(node);
        _ui->treeWidget->addTopLevelItem(item);
    }
}

QTreeWidgetItem* SceneView::createTreeItem(Node* node)
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

    visitNodeAddItem(node, nodeItem);

    return nodeItem;
}

void SceneView::visitNodeAddItem(Node* parent, QTreeWidgetItem* parentItem)
{
    for (Node* node = parent->getFirstChild(); node != NULL; node = node->getNextSibling())
    {
        QTreeWidgetItem* item = createTreeItem(node);
        parentItem->addChild(item);
    }
}

void SceneView::itemChanged(QTreeWidgetItem* item, int column)
{
    QVariant data = item->data(0, Qt::UserRole);
    Node* node = (Node*) data.toLongLong();
    node->setId((item->text(0)).toLatin1().constData());
}
