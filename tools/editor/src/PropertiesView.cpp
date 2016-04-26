#include "PropertiesView.h"
#include "ui_PropertiesView.h"


PropertiesView::PropertiesView(QWidget* parent) : QWidget(parent), 
    _ui(new Ui::PropertiesView), _editor(nullptr), _sceneView(nullptr)
{
    _ui->setupUi(this);
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


    if (_editor->getEditMode() == EditorWindow::EDIT_NODE)
    {

    }
}

