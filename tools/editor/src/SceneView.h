#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include "gameplay.h"
using namespace gameplay;
#include "EditorWindow.h"
#include "SceneSortFilterProxyModel.h"
#include <QWidget>
#include <QStandardItem>

namespace Ui {
class SceneView;
}


/**
 * Defines the scene view to display the hierarchy of scene
 * including all node within it.
 */
class SceneView : public QWidget
{
    Q_OBJECT
public:
    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit SceneView(QWidget* parent = nullptr);

    /**
     * Destructor.
     */
    ~SceneView();

    /**
     * Sets the editor this scene view is connected to.
     *
     * @param editor The editor this scene view is connected to.
     */
    void setEditor(EditorWindow* editor);

public slots:
    /**
     * Handler when the scene changes.
     */
    void sceneChanged();

    /**
     * Handler for when the search filter text changes.
     *
     * @param text The changed text.
     */
    void searchTextChanged(const QString& text);

    /**
     * @see QAbstractItemModel::dataChanged
     */
    void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
    
private:
    QStandardItem* createTreeItem(Node* node);

    void visitNodeAddItem(Node* parent, QStandardItem* parentItem);

    Ui::SceneView* _ui;
    EditorWindow* _editor;
    Scene* _scene;
    QStandardItemModel* _model;
    SceneSortFilterProxyModel* _sortFilter;
};

#endif
