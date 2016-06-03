#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include "gameplay.h"
using namespace gameplay;

#include "SceneTreeView.h"
#include "SceneSortFilterProxyModel.h"
#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>


namespace Ui {
class SceneView;
}
class EditorWindow;
class SceneTreeView;

/**
 * Defines the scene view to display the hierarchy of scene.
 */
class SceneView : public QWidget
{
    Q_OBJECT
    friend class SceneTreeView;
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
     * Sets the editor this view is connected to.
     *
     * @param editor The editor this view is connected to.
     */
    void setEditor(EditorWindow* editor);

    /**
     * Gets the list of selected items in the scene.
     *
     * @return The list of selected items in the scene.
     */
    std::list<QStandardItem*>* getItemsSelected() const;


public slots:
    /**
     * Handler when the scene changed.
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

protected:
    /**
     * Gets the ui associated with this view.
     *
     * @return The view ui.
     */
    Ui::SceneView* ui();

    /**
     * Deletes the items selected.
     */
    void deleteItemsSelected();

private:
    QStandardItem* createItem(Node* node);
    QStandardItem* createHierarchy(Node* node);
    void visitorAddItem(Node* parent, QStandardItem* parentItem);
    void addToHiearchy(Node* node, QStandardItem* item);

    Ui::SceneView* _ui;
    EditorWindow* _editor;
    Scene* _scene;
    SceneSortFilterProxyModel* _sceneSortFilter;
    QStandardItemModel* _sceneModel;
    std::list<QStandardItem*>* _itemsSelected;
};

#endif
