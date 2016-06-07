#include "PropertiesView.h"
#include "ui_PropertiesView.h"
#include "EditorWindow.h"
#include "SceneView.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVariant>


PropertiesView::PropertiesView(QWidget* parent) : QWidget(parent), 
    _ui(new Ui::PropertiesView), _editor(nullptr), _sceneView(nullptr)
{
    _ui->setupUi(this);    
    _ui->scrollAreaProperties->setVisible(false);

    connect(_ui->lineEditNodeName, &QLineEdit::editingFinished, this, &PropertiesView::nodeNameChanged);
    connect(_ui->doubleSpinBoxNodePositionX, &QDoubleSpinBox::editingFinished, this, &PropertiesView::nodePositionChanged);
    connect(_ui->doubleSpinBoxNodePositionY, &QDoubleSpinBox::editingFinished, this, &PropertiesView::nodePositionChanged);
    connect(_ui->doubleSpinBoxNodePositionZ, &QDoubleSpinBox::editingFinished, this, &PropertiesView::nodePositionChanged);
    connect(_ui->doubleSpinBoxNodeRotationX, &QDoubleSpinBox::editingFinished, this, &PropertiesView::nodeRotationChanged);
    connect(_ui->doubleSpinBoxNodeRotationY, &QDoubleSpinBox::editingFinished, this, &PropertiesView::nodeRotationChanged);
    connect(_ui->doubleSpinBoxNodeRotationZ, &QDoubleSpinBox::editingFinished, this, &PropertiesView::nodeRotationChanged);
    connect(_ui->doubleSpinBoxNodeScaleX, &QDoubleSpinBox::editingFinished, this, &PropertiesView::nodeScaleChanged);
    connect(_ui->doubleSpinBoxNodeScaleY, &QDoubleSpinBox::editingFinished, this, &PropertiesView::nodeScaleChanged);
    connect(_ui->doubleSpinBoxNodeScaleZ, &QDoubleSpinBox::editingFinished, this, &PropertiesView::nodeScaleChanged);
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

            QVariant userData = item->data(Qt::UserRole + 1);
            Node* node = (Node*)userData.toLongLong();
            _ui->doubleSpinBoxNodePositionX->setValue(node->getTranslationX());
            _ui->doubleSpinBoxNodePositionY->setValue(node->getTranslationY());
            _ui->doubleSpinBoxNodePositionZ->setValue(node->getTranslationZ());

            Vector3 rotation;
            node->getRotation().toAxisAngle(&rotation);
            _ui->doubleSpinBoxNodeRotationX->setValue(rotation.x);
            _ui->doubleSpinBoxNodeRotationY->setValue(rotation.y);
            _ui->doubleSpinBoxNodeRotationZ->setValue(rotation.z);

            _ui->doubleSpinBoxNodeScaleX->setValue(node->getScaleX());
            _ui->doubleSpinBoxNodeScaleY->setValue(node->getScaleY());
            _ui->doubleSpinBoxNodeScaleZ->setValue(node->getScaleZ());
        }
    }
}

void PropertiesView::nodeNameChanged()
{
    std::list<QStandardItem*>* itemsSelected = _editor->getSceneView()->getItemsSelected();
    if (itemsSelected->size() < 1)
        return;

    // Update the text
    QStandardItem* itemSelected = itemsSelected->front();
    QString name = _ui->lineEditNodeName->text();
    itemSelected->setText(name);

    // Update the associate node id
    QVariant userData = itemSelected->data(Qt::UserRole + 1);
    Node* nodeSelected = (Node*)userData.toLongLong();
    QByteArray itemSelectedByteArray = name.toLatin1();
    nodeSelected->setId(itemSelectedByteArray.constData());
}

void PropertiesView::nodePositionChanged()
{
    std::list<QStandardItem*>* itemsSelected = _editor->getSceneView()->getItemsSelected();
    if (itemsSelected->size() < 1)
        return;

    float positionX = (float)_ui->doubleSpinBoxNodePositionX->value();
    float positionY = (float)_ui->doubleSpinBoxNodePositionY->value();
    float positionZ = (float)_ui->doubleSpinBoxNodePositionZ->value();

    QStandardItem* item = itemsSelected->front();
    QVariant userData = item->data(Qt::UserRole + 1);
    Node* node = (Node*)userData.toLongLong();
    node->setTranslation(positionX, positionY, positionZ);
}

void PropertiesView::nodeRotationChanged()
{
    std::list<QStandardItem*>* itemsSelected = _editor->getSceneView()->getItemsSelected();
    if (itemsSelected->size() < 1)
        return;

    float rotationX = (float)_ui->doubleSpinBoxNodeRotationX->value();
    float rotationY = (float)_ui->doubleSpinBoxNodeRotationY->value();
    float rotationZ = (float)_ui->doubleSpinBoxNodeRotationZ->value();

    QStandardItem* item = itemsSelected->front();
    QVariant userData = item->data(Qt::UserRole + 1);
    Node* node = (Node*)userData.toLongLong();
}

void PropertiesView::nodeScaleChanged()
{
    std::list<QStandardItem*>* itemsSelected = _editor->getSceneView()->getItemsSelected();
    if (itemsSelected->size() < 1)
        return;

    float scaleX = (float)_ui->doubleSpinBoxNodeRotationX->value();
    float scaleY = (float)_ui->doubleSpinBoxNodeRotationY->value();
    float scaleZ = (float)_ui->doubleSpinBoxNodeRotationZ->value();

    QStandardItem* item = itemsSelected->front();
    QVariant userData = item->data(Qt::UserRole + 1);
    Node* node = (Node*)userData.toLongLong();
    node->setScale(Vector3(scaleX, scaleY, scaleZ));
}
