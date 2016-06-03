#include "EditorWindow.h"
#include "GameView.h"
#include "Project.h"
#include "ProjectWizard.h"
#include "ProjectTreeView.h"
#include "ProjectView.h"
#include "PropertiesView.h"
#include "SceneView.h"
#include "ui_EditorWindow.h"
#include <QtWidgets>

#define EDITOR_WINDOW_TITLE "GamePlay Editor"


EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent), 
    _ui(new Ui::EditorWindow), _projectWizard(nullptr), _projectView(nullptr),
    _gameView(nullptr), _propertiesView(nullptr), _sceneView(nullptr), _scene(nullptr),
    _editMode(EDIT_NODE), _selectionBegin(nullptr), _selectionEnd(nullptr),
    _transformModeButton(nullptr), _shadingButton(nullptr)
{
    _ui->setupUi(this);

    // Initialize the stylesheet
    QFile file(":/gameplay-editor.css");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
         QTextStream stream(&file);
         qApp->setStyleSheet(stream.readAll());
         file.close();
    }

    // Initialize some effects
    QApplication::setEffectEnabled(Qt::UI_FadeMenu);
    QApplication::setEffectEnabled(Qt::UI_FadeTooltip);

    // Initialize the status bar
    QStatusBar* statusBar = this->statusBar();
    statusBar->showMessage(tr("Status: Ok"));

    // Initialize the game view
    _gameView = new GameView(this);
    setCentralWidget(_gameView);

    // Initialize the menu bar
    QMenuBar* menuBar = this->menuBar();
    QMenu* projectMenu = menuBar->addMenu(tr("Project"));
    projectMenu->addAction(_ui->actionNew);
    projectMenu->addAction(_ui->actionOpen);
    projectMenu->addAction(_ui->actionSave);
    projectMenu->addSeparator();
    projectMenu->addAction(_ui->actionPlay);
    projectMenu->addAction(_ui->actionPackage);
    QMenu* editMenu = menuBar->addMenu(tr("Edit"));
    editMenu->addAction(_ui->actionPreferences);
    editMenu->addAction(_ui->actionUndo);
    editMenu->addAction(_ui->actionRedo);
    editMenu->addSeparator();
    editMenu->addAction(_ui->actionCut);
    editMenu->addAction(_ui->actionCopy);
    editMenu->addAction(_ui->actionPaste);
    editMenu->addSeparator();
    editMenu->addAction(_ui->actionDuplicate);
    editMenu->addAction(_ui->actionDelete);
    editMenu->addAction(_ui->actionFrame);
    editMenu->addAction(_ui->actionFind);
    editMenu->addSeparator();
    QMenu* helpMenu = menuBar->addMenu(tr("Help"));
    helpMenu->addAction(_ui->actionHelp);
    helpMenu->addAction(_ui->actionAbout);

    // Initialize the toolbar
    QToolBar* toolBar = this->addToolBar(tr("Top"));
    toolBar->addAction(_ui->actionNew);
    toolBar->addAction(_ui->actionOpen);
    toolBar->addAction(_ui->actionSave);
    toolBar->addSeparator();
    toolBar->addAction(_ui->actionImport);
    toolBar->addAction(_ui->actionAdd);
    toolBar->addSeparator();
    toolBar->addAction(_ui->actionPlay);
    toolBar->addAction(_ui->actionFullscreen);


    // Initialize Add toolbar buttons
    QMenu* addMenu = new QMenu();
    QToolButton* addButton = (QToolButton*)toolBar->widgetForAction(_ui->actionAdd);
    addButton->setPopupMode(QToolButton::InstantPopup);
    addButton->setMenu(addMenu);
    addMenu->addAction(_ui->actionAdd_Node);
    addMenu->addAction(_ui->actionAdd_Cube);
    addMenu->addAction(_ui->actionAdd_Cone);
    addMenu->addAction(_ui->actionAdd_Cylinder);
    addMenu->addAction(_ui->actionAdd_Sphere);
    addMenu->addAction(_ui->actionAdd_Camera);
    addMenu->addAction(_ui->actionAdd_DirectionalLight);
    addMenu->addAction(_ui->actionAdd_PointLight);
    addMenu->addAction(_ui->actionAdd_SpotLight);
    addMenu->addAction(_ui->actionAdd_Terrain);
    addMenu->addAction(_ui->actionAdd_ParticleEmitter);
    addMenu->addAction(_ui->actionAdd_Sprite);
    addMenu->addAction(_ui->actionAdd_Text);
    addMenu->addAction(_ui->actionAdd_Form);
    addMenu->addAction(_ui->actionAdd_Audio);
    addMenu->addAction(_ui->actionAdd_Video);

    // Initialize the left toolbar
    toolBar = this->addToolBar(tr("Left"));
    toolBar->setIconSize(QSize(24,24));
    this->addToolBar(Qt::LeftToolBarArea, toolBar);
    QActionGroup* transformGroup = new QActionGroup(toolBar);
    toolBar->addAction(_ui->actionTransformMove);
    _ui->actionTransformMove->setActionGroup(transformGroup);
    _ui->actionTransformMove->setChecked(true);
    toolBar->addAction(_ui->actionTransformRotate);
    _ui->actionTransformRotate->setActionGroup(transformGroup);
    toolBar->addAction(_ui->actionTransformScale);
    _ui->actionTransformScale->setActionGroup(transformGroup);
    toolBar->addAction(_ui->actionTransformMode);
    toolBar->addSeparator();
    toolBar->addAction(_ui->actionShading);

    // Initialize transform mode toolbar buttons
    QMenu* transformModeMenu = new QMenu();
    _transformModeButton = (QToolButton*)toolBar->widgetForAction(_ui->actionTransformMode);
    _transformModeButton->setPopupMode(QToolButton::InstantPopup);
    _transformModeButton->setMenu(transformModeMenu);
    _transformModeButton->setDefaultAction(_ui->actionTransformMode_World);
    transformModeMenu->addAction(_ui->actionTransformMode_World);
    transformModeMenu->addAction(_ui->actionTransformMode_Local);

    // Initialize Shading toolbar buttons
    QMenu* shadingMenu = new QMenu();
    _shadingButton = (QToolButton*)toolBar->widgetForAction(_ui->actionShading);
    _shadingButton->setPopupMode(QToolButton::InstantPopup);
    _shadingButton->setMenu(shadingMenu);
    _shadingButton->setDefaultAction(_ui->actionShading_Lit);
    shadingMenu->addAction(_ui->actionShading_Lit);
    shadingMenu->addAction(_ui->actionShading_Unlit);
    shadingMenu->addAction(_ui->actionShading_Wireframe);

    // Initialize the docking corners
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    // Initialize the project view
    _projectView= new ProjectView(_ui->projectDockWidget);
    _ui->projectDockWidget->setWidget(_projectView);

    // Initialize the scene view
    _sceneView = new SceneView(_ui->sceneDockWidget);
    _ui->sceneDockWidget->setWidget(_sceneView);

    // Initialize the properties view
    _propertiesView = new PropertiesView(_ui->propertiesDockWidget);
    _ui->propertiesDockWidget->setWidget(_propertiesView);

    // Setup and interface from the panels to state changes in the editor.
    _propertiesView->setEditor(this);
    _sceneView->setEditor(this);
    _gameView->setEditor(this);

    // Initialize the selection begin and end to offscreen which means no selection.
    _selectionBegin = new Vector2(-1, -1);
    _selectionEnd = new Vector2(-1, -1);

    connect(_ui->actionOpen, &QAction::triggered, this, &EditorWindow::actionOpenTriggered);
    connect(_ui->actionNew, &QAction::triggered, this, &EditorWindow::actionNewTriggered);
    connect(_ui->actionFullscreen, &QAction::toggled, this, &EditorWindow::actionFullscreenToggled);
    connect(_ui->actionTransformMode_World, &QAction::triggered, this, &EditorWindow::actionTransformModeWorldTriggered);
    connect(_ui->actionTransformMode_Local, &QAction::triggered, this, &EditorWindow::actionTransformModeLocalTriggered);
    connect(_ui->actionShading_Lit, &QAction::triggered, this, &EditorWindow::actionShadingLitTriggered);
    connect(_ui->actionShading_Unlit, &QAction::triggered, this, &EditorWindow::actionShadingUnlitTriggered);
    connect(_ui->actionShading_Wireframe, &QAction::triggered, this, &EditorWindow::actionShadingWireframeTriggered);
    connect(_projectView, &ProjectView::sceneOpened, this, &EditorWindow::sceneOpened);
    connect(this, &EditorWindow::sceneChanged, _sceneView, &SceneView::sceneChanged);
    connect(this, &EditorWindow::sceneChanged, _gameView, &GameView::sceneChanged);
    connect(this, &EditorWindow::selectionChanged, _sceneView, &SceneView::editorSelectionChanged);
    connect(_ui->actionAdd_Node, &QAction::triggered, _sceneView, &SceneView::actionAddNodeTriggered);
}

EditorWindow::~EditorWindow()
{
    delete _ui;
}

const std::string& EditorWindow::getAssetPath() const
{
    return _assetPath;
}

void EditorWindow::setProjectWizard(ProjectWizard* projectWizard)
{
    _projectWizard = projectWizard;    
    connect(_projectWizard, &ProjectWizard::projectOpened, this, &EditorWindow::projectOpened);
}

Project* EditorWindow::getProject() const
{
    return _projectView->getProject();
}

 EditorWindow::EditMode EditorWindow::getEditMode() const
 {
     return _editMode;
 }

Scene* EditorWindow::getScene() const
{
    return _scene;
}

SceneView* EditorWindow::getSceneView() const
{
    return _sceneView;
}

PropertiesView* EditorWindow::getProperiesView() const
{
    return _propertiesView;
}

Vector2* EditorWindow::getSelectionBegin() const
{
    if (_selectionBegin->x < 0 || _selectionBegin->y < 0)
        return nullptr;

    return _selectionBegin;
}

Vector2* EditorWindow::getSelectionEnd() const
{
    if (_selectionEnd->x < 0 || _selectionEnd->y < 0)
        return nullptr;

    return _selectionEnd;
}

void EditorWindow::projectOpened(const QString& path)
{
    _projectView->openProject(path);
    Project* project = _projectView->getProject();
    if (project)
    {
        setWindowTitle(QString(QLatin1String(EDITOR_WINDOW_TITLE)) +
                       QString(QLatin1String(" - ")) +
                       _projectView->getProject()->getName());
    }
}

void EditorWindow::sceneOpened(const QString& path)
{
    QByteArray pathByteArray = path.toLatin1();
    Serializer* serializer = Serializer::createReader(pathByteArray.data());
    _scene = static_cast<Scene*>(serializer->readObject(nullptr));

    emit sceneChanged();
}

void EditorWindow::actionOpenTriggered()
{
    _projectWizard->readProjects();
    _projectWizard->show();
}

void EditorWindow::actionNewTriggered()
{
    _projectWizard->show();
    _projectWizard->newProjectPressed();

}

void EditorWindow::actionFullscreenToggled(bool enabled)
{
    // TODO:
}

void EditorWindow::actionTransformModeWorldTriggered()
{

    _transformModeButton->setDefaultAction(_ui->actionTransformMode_World);

    // TODO
}

void EditorWindow::actionTransformModeLocalTriggered()
{
    _transformModeButton->setDefaultAction(_ui->actionTransformMode_Local);
    // TODO:
}


void EditorWindow::actionShadingLitTriggered()
{
    _shadingButton->setDefaultAction(_ui->actionShading_Lit);
    // TODO:
}

void EditorWindow::actionShadingUnlitTriggered()
{

    _shadingButton->setDefaultAction(_ui->actionShading_Unlit);
    // TODO:
}

void EditorWindow::actionShadingWireframeTriggered()
{
    _shadingButton->setDefaultAction(_ui->actionShading_Wireframe);
    // TODO:
}

