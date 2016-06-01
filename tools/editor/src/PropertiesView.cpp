#include "PropertiesView.h"
#include "ui_PropertiesView.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>


PropertiesView::PropertiesView(QWidget* parent) : QWidget(parent), 
    _ui(new Ui::PropertiesView), _editor(nullptr), _sceneView(nullptr)
{
    _ui->setupUi(this);

    setupEditNodeProperties();
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

void PropertiesView::sceneSelectionChanged()
{
    std::list<QStandardItem*>* selectedItems = _editor->getSceneView()->getSelectedItems();
    if  (selectedItems->size() == 0 || selectedItems->size() > 1)
        _ui->scrollAreaProperties->setEnabled(false);
    else
        _ui->scrollAreaProperties->setEnabled(true);
}

void PropertiesView::setupEditNodeProperties()
{

}
