#ifndef PROPERTIESVIEW_H
#define PROPERTIESVIEW_H

#include "gameplay.h"
using namespace gameplay;
#include "EditorWindow.h"
#include "SceneView.h"
#include <QWidget>

namespace Ui {
class PropertiesView;
}


/**
 * Defines a view for diplaying and editing scene objects.
 */
class PropertiesView : public QWidget
{
    Q_OBJECT
public:
    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit PropertiesView(QWidget* parent = nullptr);

    /**
     * Destructor.
     */
    ~PropertiesView();

    /**
     * Sets the editor this scene view is connected to.
     *
     * @param editor The editor this scene view is connected to.
     */
    void setEditor(EditorWindow* editor);

    /**
     * Set the scene view for accessing the selected items in the scene.
     *
     * @param sceneView The scene view.
     */
    void setSceneView(SceneView* sceneView);

public slots:
    /**
     * Handler when the node selected in the scene change.
     */
    void sceneSelectionChanged();

private:

    Ui::PropertiesView* _ui;
    EditorWindow* _editor;
    SceneView* _sceneView;
};

#endif
