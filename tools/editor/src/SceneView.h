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

class SceneView : public QWidget
{
    Q_OBJECT
public:
    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit SceneView(QWidget* parent = 0);

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
    
private:
    void visitNodeAddItem(Node* parent, QTreeWidgetItem* parentItem);

    Ui::SceneView* _ui;
    EditorWindow* _editor;
    Scene* _scene;
};

#endif
