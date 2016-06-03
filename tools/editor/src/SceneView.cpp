#include "SceneView.h"
#include "ui_SceneView.h"
#include "EditorWindow.h"
#include "PropertiesView.h"
#include <QtWidgets>
#include <QItemSelectionRange>

#define NODE_NEW "Node"


SceneView::SceneView(QWidget* parent) : QWidget(parent),
    _ui(new Ui::SceneView), _editor(nullptr),
    _scene(nullptr), _sceneSortFilter(nullptr),
    _sceneModel(nullptr), _itemsSelected(nullptr)
{
    _ui->setupUi(this);

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

    _ui->lineEditSearch->addAction(QIcon(":/res/images/search.png"), QLineEdit::LeadingPosition);

    _sceneModel = new QStandardItemModel();
    // Sort and search filter
    _sceneSortFilter = new SceneSortFilterProxyModel();
    _sceneSortFilter->setDynamicSortFilter(true);
    _sceneSortFilter->setFilterKeyColumn(0);
    _sceneSortFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    _sceneSortFilter->setSourceModel(_sceneModel);

    _itemsSelected = new std::list<QStandardItem*>();

    _ui->treeView->setUniformRowHeights(true);
    _ui->treeView->setSortingEnabled(false);
    _ui->treeView->setModel(_sceneSortFilter);

    connect(_ui->lineEditSearch, &QLineEdit::textChanged, this, &SceneView::searchTextChanged);
    connect(_sceneModel, &QStandardItemModel::dataChanged, this, &SceneView::modelDataChanged);
    connect(_ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SceneView::modelSelectionChanged);
    connect(_ui->actionAdd_Node, &QAction::triggered, this, &SceneView::actionAddNodeTriggered);
    connect(_ui->actionDelete_Node, &QAction::triggered, this, &SceneView::deleteItemsSelected);
}

SceneView::~SceneView()
{
    delete _ui;
    delete _sceneModel;
    _itemsSelected->clear();
    delete _itemsSelected;
}

void SceneView::setEditor(EditorWindow* editor)
{
    _editor = editor;
}

std::list<QStandardItem*>* SceneView::getItemsSelected() const
{
    return _itemsSelected;
}

void SceneView::sceneChanged()
{
    _scene = _editor->getScene();
    for (Node* node = _scene->getFirstNode(); node != nullptr; node = node->getNextSibling())
    {
        QStandardItem* item = createHierarchy(node);
        _sceneModel->appendRow(item);
    }
}

void SceneView::editorSelectionChanged()
{
    Vector2* selectionBegin = _editor->getSelectionBegin();
    Vector2* selectedEnd = _editor->getSelectionEnd();

    // TODO
    // Project the point or region against all the nodes in the scene
    // updating the selected nodes list.
}

void SceneView::modelSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    for (int i = 0; i < deselected.size(); i++)
    {
        QModelIndex index = deselected.at(i).topLeft();
        QStandardItem* item = _sceneModel->itemFromIndex(_sceneSortFilter->mapToSource(index));
        _itemsSelected->remove(item);
    }
    for (int i = 0; i < selected.size(); i++)
    {
        QModelIndex index = selected.at(i).topLeft();
        QStandardItem* item = _sceneModel->itemFromIndex(_sceneSortFilter->mapToSource(index));
        _itemsSelected->push_back(item);
    }
    _editor->getProperiesView()->nodeSelectionChanged();
}

void SceneView::modelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    QVariant userData = topLeft.data(Qt::UserRole + 1);
    Node* node = (Node*) userData.toLongLong();
    QVariant displayData = topLeft.data(Qt::DisplayRole);
    node->setId(displayData.toString().toLatin1().constData());
    _editor->getProperiesView()->nodeSelectionChanged();
}

Ui::SceneView* SceneView::ui()
{
    return _ui;
}

void SceneView::deleteItemsSelected()
{
    while(!_ui->treeView->selectionModel()->selectedIndexes().isEmpty())
    {
        QModelIndex index = _ui->treeView->selectionModel()->selectedIndexes().first();

        // Delete the node hosted in the user data
        QStandardItem* itemSelected = _sceneModel->itemFromIndex(_sceneSortFilter->mapToSource(index));
        QVariant userData = itemSelected->data(Qt::UserRole + 1);
        Node* nodeSelected = (Node*)userData.toLongLong();

        // Delete the item selected
        if (!_sceneModel->removeRow(index.row(), index.parent()))
        {
            printf("failed");
        }
    }
    _itemsSelected->clear();
}

QStandardItem* SceneView::createItem(Node* node)
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

    return item;
}

QStandardItem* SceneView::createHierarchy(Node* node)
{
    // Clone the node into a item for the model.
    QStandardItem* item = createItem(node);
    // Visit all the child nodes adding items for each.
    visitorAddItem(node, item);

    return item;
}

void SceneView::visitorAddItem(Node* parent, QStandardItem* parentItem)
{
    for (Node* node = parent->getFirstChild(); node != nullptr; node = node->getNextSibling())
    {
        QStandardItem* item = createHierarchy(node);
        parentItem->appendRow(item);
    }
}

void SceneView::addToHiearchy(Node* node, QStandardItem* item)
{
    // If there is no nodes selected the just add to the scene
    if (_itemsSelected->size() == 0)
    {
        _sceneModel->appendRow(item);
        _scene->addNode(node);
    }
    else
    {
        // If any nodes are selected then add to the first selected and expand the parent
        QStandardItem* itemSelected = _itemsSelected->front();
        itemSelected->appendRow(item);
        _ui->treeView->setExpanded(_sceneSortFilter->mapFromSource(itemSelected->index()), true);
        QVariant userData = itemSelected->data(Qt::UserRole + 1);
        Node* nodeSelected = (Node*)userData.toLongLong();
        nodeSelected->addChild(node);
        _ui->treeView->selectionModel()->setCurrentIndex(_sceneSortFilter->mapFromSource(itemSelected->index()),
                                                         QItemSelectionModel::ClearAndSelect);
    }
}

void SceneView::searchTextChanged(const QString& text)
{
    _sceneSortFilter->setFilterRegExp(text);
}

void SceneView::actionAddNodeTriggered()
{
    // Create an empty node
    Node* node = Node::create(NODE_NEW);
    // Create an empty item.
    QStandardItem* item = createItem(node);
    // Add the node to the scene hierarchy
    addToHiearchy(node, item);
}
