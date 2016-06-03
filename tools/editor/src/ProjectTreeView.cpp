#include "ProjectTreeView.h"
#include "Project.h"
#include "ProjectView.h"
#include "ui_ProjectView.h"
#include <QtWidgets>


ProjectTreeView::ProjectTreeView(QWidget* parent) : QTreeView(parent), 
    _projectView(static_cast<ProjectView*>(parent))
{
    setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
    setSortingEnabled(true);
}

ProjectTreeView::~ProjectTreeView()
{
}

void ProjectTreeView::itemDoubleClicked(const QModelIndex& index)
{
    openSelectedItem();
}

void ProjectTreeView::openFileTriggered()
{
    openSelectedItem();
}

void ProjectTreeView::openSelectedItem()
{
    Project* project = _projectView->getProject();
    ProjectSortFilterProxyModel* sortFilter = _projectView->sortFilter();
    const QModelIndex index = sortFilter->mapToSource(this->selectionModel()->currentIndex());

    QString sceneFilePath = project->filePath(index);
    if (sceneFilePath.endsWith(".scene"))
    {
        _projectView->openScene(sceneFilePath);
    }
}

void ProjectTreeView::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    Ui::ProjectView* ui = _projectView->ui();
    menu.addAction(ui->actionOpen_File);
    menu.addSeparator();
    menu.addAction(ui->actionNew_File);
    menu.addAction(ui->actionNew_Folder);
    menu.addSeparator();
    menu.addAction(ui->actionImport_File);

    menu.exec(event->globalPos());
}


