#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include "gameplay.h"
using namespace gameplay;

#include "SceneView.h"
#include <QMainWindow>
#include <QToolButton>

namespace Ui {
class EditorWindow;
}
class ProjectWizard;
class ProjectView;
class GameView;
class PropertiesView;
class Project;


/**
 * Defines the main editor window and toolbars.
 */
class EditorWindow : public QMainWindow
{
    Q_OBJECT
public:

    enum EditMode
    {
        EDIT_NODE,
        EDIT_MODEL
    };

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit EditorWindow(QWidget* parent = nullptr);
    
    /**
     * Destructor.
     */
    ~EditorWindow();

    /**
     * Gets the asset path for the project.
     *
     * @return The asset path.
     */
    const std::string& getAssetPath() const;

    /**
     * Sets the project wizard.
     *
     * @param projectWizard
     */
    void setProjectWizard(ProjectWizard* projectWizard);

    /**
     * Gets the opened project.
     *
     * @return The currently open project.
     */
    Project* getProject() const;

    /**
     * Gets the edit mode the editor is currently in.
     *
     * @return The edit mode the editor is currently in.
     */
    EditMode getEditMode() const;

    /**
     * Gets the opened scene.
     *
     * @return The open scene.
     */
    Scene* getScene() const;

    /**
     * Gets the scene view.
     *
     * @return The scene view.
     */
    SceneView* getSceneView() const;

    /**
     * Gets the properties view.
     * @return The properties view.
     */
    PropertiesView* getProperiesView() const;

    /**
     * Gets the begin screen position for a selection or nullptr for no selection.
     *
     * @return The begin screen position for a selection or nullptr for no selection.
     */
    Vector2* getSelectionBegin() const;

    /**
     * Gets the end screen position for a drag selection or nullptr for no drag selection.
     *
     * @return The screen end point for a drag selection or nullptr for no drag selection.
     */
    Vector2* getSelectionEnd() const;

public slots:
    /**
     * Handler for opening a project.
     *
     * @param path The path to the project opened.
     */
    void projectOpened(const QString& path);

    /**
     * Handler for opening a scene.
     *
     * @param path The path to the scene opened.
     */
    void sceneOpened(const QString& path);

    /**
     * Action open triggered from menu.
     */
    void actionOpenTriggered();

    /**
     * Action new triggered from menu.
     */
    void actionNewTriggered();

    /**
     * Action when actionFullscreen is toggled.
     *
     * @param enabled true if enabled
     */
    void actionFullscreenToggled(bool enabled);

    /**
     * Action when transform mode world is triggered.
     */
    void actionTransformModeWorldTriggered();

    /**
     * Action when transform mode local is triggered.
     */
    void actionTransformModeLocalTriggered();

    /**
     * Action when shading lit is triggered.
     */
    void actionShadingLitTriggered();

    /**
     * Action when shading unlit is triggered.
     */
    void actionShadingUnlitTriggered();

    /**
     * Action when shading wireframe is triggered.
     */
    void actionShadingWireframeTriggered();

signals:
    /**
     * Signal emitted when the project changes.
     */
    void projectChanged();

    /**
     * Signal emitted when the scene changes.
     */
    void sceneChanged();

    /**
     * Signal emitted when selection begin or end points change or either become deselected.
     */
    void selectionChanged();

private:
    Ui::EditorWindow* _ui;
    GameView* _gameView;
    ProjectWizard* _projectWizard;
    ProjectView* _projectView;
    PropertiesView *_propertiesView;
    SceneView*_sceneView;
    Scene* _scene;
    EditMode _editMode;
    Vector2* _selectionBegin;
    Vector2* _selectionEnd;
    QToolButton* _transformModeButton;
    QToolButton* _shadingButton;
    std::string _assetPath;
};

#endif
