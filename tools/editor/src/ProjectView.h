#ifndef PROJECTVIEW_H
#define PROJECTVIEW_H

#include "EditorWindow.h"
#include "ProjectSortFilterProxyModel.h"
#include <QWidget>

namespace Ui {
class ProjectView;
}
class Project;


/**
 * Defines a project view showing files visible in the project directory.
 */
class ProjectView : public QWidget
{
    Q_OBJECT
    friend class ProjectTreeView;
public:
    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit ProjectView(QWidget* parent = nullptr);

    /**
     * Destructor.
     */
    ~ProjectView();

    /**
     * Opens a project at the specified path.
     *
     * @param path The project path.
     */
    void openProject(const QString& path);

    /**
     * Closes the currently open project.
     */
    void closeProject();

    /**
     * Gets the project that is open.
     *
     * @return The open project.
     */
    Project* project() const;

public slots:
    /**
     * Handler for when the search filter text changes.
     *
     * @param text The changed text.
     */
    void searchTextChanged(const QString& text);

signals:
    /**
     * Signal emmited with a new scene has been opened.
     *
     * @param path The path of the scene being opened.
     */
    void sceneOpened(const QString& path);

protected:
    /**
    * Gets the ui associated with this view.
    *
    * @return The view ui.
    */
    Ui::ProjectView* ui();

    /**
    * Opens a scene at the specified path.
    *
    * @param path The path of the scene to be opened.
    */
    void openScene(const QString& path);

private:
    Ui::ProjectView* _ui;
    Project* _project;
    ProjectSortFilterProxyModel* _sortFilterProxyModel;
};

#endif
