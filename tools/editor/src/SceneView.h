#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include "gameplay.h"
using namespace gameplay;
#include "EditorWindow.h"
#include <QWidget>
#include <QTreeWidgetItem>

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
     * @see QTreeWidget::itemChanged
     */
    static void itemChanged(QTreeWidgetItem* item, int column);
    
private:
    QTreeWidgetItem* createTreeItem(Node* node);

    void visitNodeAddItem(Node* parent, QTreeWidgetItem* parentItem);

    Ui::SceneView* _ui;
    EditorWindow* _editor;
    Scene* _scene;
};

#endif
