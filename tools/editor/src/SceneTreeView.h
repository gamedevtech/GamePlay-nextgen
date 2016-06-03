#ifndef SCENETREEVIEW_H
#define SCENETREEVIEW_H

#include <QTreeView>

class SceneView;


/**
 * Defines the scene treeview
 */
class SceneTreeView : public QTreeView
{
    Q_OBJECT
public:
    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit SceneTreeView(QWidget* parent = nullptr);

    /**
     * Destructor.
     */
    ~SceneTreeView();

protected:
    /**
     * Handler fo context menu events.
     *
     * @param event The context menu event.
     */
    void contextMenuEvent(QContextMenuEvent* event);

    /**
     * Handler when the key is pressed.
     *
     * @param event The key event.
     */
    void keyPressEvent(QKeyEvent* event);

private:

    SceneView* _sceneView;
};

#endif
