#include "ProjectView.h"
#include "Project.h"
#include "ui_ProjectView.h"
#include "EditorWindow.h"
#include <QtWidgets>
#include <QFileSystemModel>

ProjectView::ProjectView(QWidget* parent) : QWidget(parent), 
    _ui(new Ui::ProjectView), _project(nullptr), _sortFilter(nullptr)
{
    _ui->setupUi(this);

    _ui->lineEditSearch->addAction(QIcon(":/res/images/search.png"), QLineEdit::LeadingPosition);

    connect(_ui->lineEditSearch, &QLineEdit::textChanged, this, &ProjectView::searchTextChanged);
    connect(_ui->treeView, &QTreeView::doubleClicked, _ui->treeView, &ProjectTreeView::itemDoubleClicked);
    connect(_ui->actionOpen_File, &QAction::triggered, _ui->treeView, &ProjectTreeView::openFileTriggered);
}

ProjectView::~ProjectView()
{
    delete _ui;
    closeProject();
}

void ProjectView::openProject(const QString& path)
{
    closeProject();
    _project = Project::open(path, _ui->treeView);
    if (_project)
    {
        _project->setRootPath(path);

        // Name filter for file extension the project is aware of
        QStringList nameFilters;
        nameFilters << "*.config" << "*.scene" << "*.material" << "*.font" <<
                       "*.physics" << "*.particles" << "*.form" << "*.theme" <<  "*.ui" <<
                       "*.fbx" << "*.blend" <<  "*.ttf" << "*.png" << "*.psd" << 
                       "*.wav" << "*.ogg" << "*.mp3" << "*.mp4" <<
                       "*.js" << "*.ts" <<  "*.cpp" << "*.h";        
        _project->setNameFilters(nameFilters);
        _project->setNameFilterDisables(false);
        
        // Sort and search filter
        _sortFilter = new ProjectSortFilterProxyModel();
        _sortFilter->setDynamicSortFilter(true);
        _sortFilter->setFilterKeyColumn(0);
        _sortFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
        _sortFilter->setSourceModel(_project);
        _ui->treeView->setSortingEnabled(true);
        _ui->treeView->setModel(_sortFilter);
        _ui->treeView->setRootIndex(_sortFilter->mapFromSource(_project->index(path)));

        // Header sizing
        _ui->treeView->hideColumn(2);
        _ui->treeView->setColumnWidth(0, 600);
        _ui->treeView->setColumnWidth(1, 120);
        _ui->treeView->setColumnWidth(2, 120);

        // Expand the '/res' folder
        QString resFolderPath = path + QString("/") + QString(QLatin1String("res"));
        _ui->treeView->expand(_sortFilter->mapFromSource(_project->index(resFolderPath)));

        // Open the last scene editor for the project.        
        openScene(path+ QString("/") + _project->getScenePath());
    }
}

void ProjectView::closeProject()
{
    SAFE_DELETE(_project);
}

Project* ProjectView::getProject() const
{
    return _project;
}

ProjectSortFilterProxyModel* ProjectView::sortFilter() const
{
    return _sortFilter;
}

Ui::ProjectView* ProjectView::ui()
{
    return _ui;
}

void ProjectView::openScene(const QString& path)
{
    emit sceneOpened(path);
}

void ProjectView::searchTextChanged(const QString& text)
{ 
    _sortFilter->setFilterRegExp(text);
}
