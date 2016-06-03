#include "PropertiesView.h"
#include "ui_PropertiesView.h"
#include "EditorWindow.h"
#include "SceneView.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>


PropertiesView::PropertiesView(QWidget* parent) : QWidget(parent), 
    _ui(new Ui::PropertiesView), _editor(nullptr), _sceneView(nullptr)
{
    _ui->setupUi(this);    
    _ui->scrollAreaProperties->setVisible(false);

    connect(_ui->lineEditNodeName, &QLineEdit::textChanged, this, &PropertiesView::nodeNameChanged);
}

PropertiesView::~PropertiesView()
{
    delete _ui;
}

void PropertiesView::setEditor(EditorWindow* editor)
{
    _editor = editor;
    _sceneView = _editor->getSceneView();
}

void PropertiesView::nodeSelectionChanged()
{
    std::list<QStandardItem*>* itemsSelected = _editor->getSceneView()->getItemsSelected();
    if (itemsSelected->size() == 0 || itemsSelected->size() > 1)
    {
        _ui->scrollAreaProperties->setVisible(false);
    }
    else
    {
        if (itemsSelected->size() == 1)
        {
            QStandardItem* item = itemsSelected->front();
            _ui->scrollAreaProperties->setVisible(true);
            _ui->lineEditNodeName->setText(item->text());
        }
    }
}

void PropertiesView::nodeNameChanged(const QString& name)
{
    std::list<QStandardItem*>* itemsSelected = _editor->getSceneView()->getItemsSelected();
    if (itemsSelected->size() == 1)
    {
        // Update the text
        QStandardItem* itemSelected = itemsSelected->front();
        itemSelected->setText(name);
        // Update the associate node id
        QVariant userData = itemSelected->data(Qt::UserRole + 1);
        Node* nodeSelected = (Node*)userData.toLongLong();
        QByteArray itemSelectedByteArray = name.toLatin1();
        nodeSelected->setId(itemSelectedByteArray.constData());
    }
}
