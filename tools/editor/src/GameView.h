#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "gameplay.h"
using namespace gameplay;

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QKeyEvent>

class EditorWindow;


/**
 * The main game view use to intialize, update and render game scenes.
 */
class GameView : public QOpenGLWidget, protected QOpenGLFunctions, public Game
{
    Q_OBJECT
public:
    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit GameView(QWidget* parent = nullptr);

    /**
     * Destructor.
     */
    ~GameView();

    /**
     * Sets the editor window this game view uses.
     *
     * @param editor The editor window this game view uses.
     */
    void setEditor(EditorWindow* editor);

public slots:
    /**
     * Slots handler for resource path changing.
     *
     * @param path The new resource path.
     */
    void resourcePathChanged(const QString& path);

    /**
     * Handler when the scene changes.
     */
    void sceneChanged();

protected:
    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);
    
    /**
     * @see QOpenGLWidget::initializeGL
     */
    void initializeGL();

    /**
     * @see QOpenGLWidget::resizeGL
     */
    void resizeGL(int width, int height);

    /**
     * @see QOpenGLWidget::paintGL
     */
    void paintGL();

    /**
     * @see QWidget::mousePressEvent
     */
    void mousePressEvent(QMouseEvent* event);

    /**
     * @see QWidget::mouseReleaseEvent
     */
    void mouseReleaseEvent(QMouseEvent* event);

    /**
     * @see QWidget::keyPressEvent
     */
    void keyPressEvent(QKeyEvent* event);

    /**
     * @see QWidget::keyReleaseEvent
     */
    void keyReleaseEvent(QKeyEvent* event);

    /**
     * Draws the active scene.
     *
     * @param node The node to draw.
     * @return true if the node was drawn, false if not.
     */
    bool drawScene(Node* node);

private:
    EditorWindow* _editor;
    Scene* _scene;
    bool _wireframe;
};

#endif
