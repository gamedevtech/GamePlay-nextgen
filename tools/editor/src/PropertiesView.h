#ifndef PROPERTIESVIEW_H
#define PROPERTIESVIEW_H

#include "gameplay.h"
using namespace gameplay;

#include <QWidget>


namespace Ui {
class PropertiesView;
}
class EditorWindow;
class SceneView;

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
     * Sets the editor this view is connected to.
     *
     * @param editor The editor this view is connected to.
     */
    void setEditor(EditorWindow* editor);

    /**
     * Set the scene view for accessing the selected items in the scene.
     *
     * @param sceneView The scene view.
     */
    void setSceneView(SceneView* sceneView);

    /**
     * Handler when the node selected in the scene change.
     */
    void nodeSelectionChanged();

public slots:
    /**
     * Handler when the node name changes.
     *
     * @param name The updated name.
     */
    void nodeNameChanged(const QString& name);


private:
    void setupEditNodeProperties();

    Ui::PropertiesView* _ui;
    EditorWindow* _editor;
    SceneView* _sceneView;
};

#endif
