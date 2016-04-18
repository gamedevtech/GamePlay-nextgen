#include "SceneView.h"
#include "ui_SceneView.h"
#include <QMenu>

SceneView::SceneView(QWidget* parent) : QWidget(parent),
    _ui(new Ui::SceneView), _editor(nullptr), _scene(nullptr), _model(nullptr), _sortFilter(nullptr)
{
    _ui->setupUi(this);
    _ui->lineEditSearch->addAction(QIcon(":/res/images/search.png"), QLineEdit::LeadingPosition);
    // Add toolbar menu.
    QMenu* addMenu = new QMenu();
    _ui->toolButtonAdd->setPopupMode(QToolButton::InstantPopup);
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

    _ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->treeView->setUniformRowHeights(true);
    _model = new QStandardItemModel();

    // Sort and search filter
    _sortFilter = new SceneSortFilterProxyModel();
    _sortFilter->setDynamicSortFilter(true);
    _sortFilter->setFilterKeyColumn(0);
    _sortFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    _sortFilter->setSourceModel(_model);
    _ui->treeView->setSortingEnabled(false);
    _ui->treeView->setModel(_sortFilter);

    connect(_ui->lineEditSearch, SIGNAL(textChanged(QString)), this, SLOT(searchTextChanged(QString)));
    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataChanged(QModelIndex, QModelIndex)));
}

SceneView::~SceneView()
{
    delete _ui;
    delete _model;
}

void SceneView::setEditor(EditorWindow* editor)
{
    _editor = editor;
}

void SceneView::sceneChanged()
{
    _scene = _editor->scene();
    for (Node* node = _scene->getFirstNode(); node != nullptr; node = node->getNextSibling())
    {
        QStandardItem* item = createTreeItem(node);
        _model->appendRow(item);
    }
}

QStandardItem* SceneView::createTreeItem(Node* node)
{
    QString text;
    const char* id = node->getId();
    if (id && strlen(id) > 0)
        text = QString(id);
    else
        text = QString(tr("Node"));

    QStandardItem* item = new QStandardItem(QIcon(":/res/images/scene-node.png"), text);
    item->setEditable(true);
    // Associate the node to the item
    item->setData(QVariant::fromValue((qlonglong)node), Qt::UserRole + 1);

    // Visit all the child nodes
    visitNodeAddItem(node, item);

    return item;
}

void SceneView::visitNodeAddItem(Node* parent, QStandardItem* parentItem)
{
    for (Node* node = parent->getFirstChild(); node != nullptr; node = node->getNextSibling())
    {
        QStandardItem* item = createTreeItem(node);
        parentItem->appendRow(item);
    }
}

void SceneView::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    QVariant userData = topLeft.data(Qt::UserRole + 1);
    Node* node = (Node*) userData.toLongLong();
    QVariant displayData = topLeft.data(Qt::DisplayRole);
    node->setId(displayData.toString().toLatin1().constData());
}

void SceneView::searchTextChanged(const QString& text)
{
    _sortFilter->setFilterRegExp(text);
}

