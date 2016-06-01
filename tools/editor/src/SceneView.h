#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include "gameplay.h"
using namespace gameplay;
#include "EditorWindow.h"
#include "SceneSortFilterProxyModel.h"
#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>

namespace Ui {
class SceneView;
}


/**
 * Defines the scene view to display the hierarchy of scene.
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

    /**
     * Gets the list of selected items in the scene.
     *
     * @return The list of selected items in the scene.
     */
    std::list<QStandardItem*>* getSelectedItems() const;

public slots:
    /**
     * Handler when the scene changes.
     */
    void sceneChanged();

    /**
     * Handler for when the editor selection points change.
     */
    void editorSelectionChanged();

    /**
     * @see QItemSelectionModel::selectionChanged
     */
    void modelSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

    /**
     * @see QAbstractItemModel::dataChanged
     */
    void modelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

    /**
     * Handler for when the search filter text changes.
     *
     * @param text The changed text.
     */
    void searchTextChanged(const QString& text);

    /**
     * Handlers for when an action to add a node is triggered.
     */
    void actionAddNodeTriggered();

signals:

    /**
     * Signal emitted when the  nodes selected have changed.
     */
    void sceneSelectionChanged();

private:
    QStandardItem* createItem(Node* node);
    QStandardItem* createHierarchy(Node* node);
    void visitorAddItem(Node* parent, QStandardItem* parentItem);
    void addToHiearchy(Node* node, QStandardItem* item);

    Ui::SceneView* _ui;
    EditorWindow* _editor;
    Scene* _scene;
    QStandardItemModel* _sceneModel;
    SceneSortFilterProxyModel* _sortFilter;
    std::list<QStandardItem*>* _selectedItems;
};


#endif
