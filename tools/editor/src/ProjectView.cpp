#include "ProjectView.h"
#include "Project.h"
#include "ui_ProjectView.h"
#include <QtWidgets>
#include <QFileSystemModel>

ProjectView::ProjectView(QWidget* parent) : QWidget(parent), 
    _ui(new Ui::ProjectView), _project(nullptr), _sortFilterProxyModel(nullptr)
{
    _ui->setupUi(this);
    _ui->lineEditSearch->addAction(QIcon(":/res/images/search.png"), QLineEdit::LeadingPosition);

    connect(_ui->lineEditSearch, SIGNAL(textChanged(QString)), this, SLOT(searchTextChanged(QString)));
    connect(_ui->projectTreeView, SIGNAL(doubleClicked(QModelIndex)), _ui->projectTreeView, SLOT(itemDoubleClicked(QModelIndex)));
    connect(_ui->actionOpen_File, SIGNAL(triggered(bool)), _ui->projectTreeView, SLOT(openFileTriggered()));
}

ProjectView::~ProjectView()
{
    delete _ui;
    closeProject();
}

Ui::ProjectView* ProjectView::ui()
{
    return _ui;
}

void ProjectView::openProject(const QString& path)
{
    closeProject();
    _project = Project::open(path, _ui->projectTreeView);
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
        
        /*TODO
        / Sort and search filter
        _sortFilterProxyModel = new ProjectSortFilterProxyModel();
        _sortFilterProxyModel->setDynamicSortFilter(true);
        _sortFilterProxyModel->setSourceModel(_project);
        _sortFilterProxyModel->setFilterKeyColumn(0);
        _ui->projectTreeView->setModel(_sortFilterProxyModel);
        _ui->projectTreeView->setSortingEnabled(true);
        _ui->projectTreeView->setRootIndex(_sortFilterProxyModel->mapFromSource(_project->index(path)));
        */
        _ui->projectTreeView->setModel(_project);
        _ui->projectTreeView->setRootIndex(_project->index(path));

        // Header sizing
        _ui->projectTreeView->hideColumn(2);
        _ui->projectTreeView->setColumnWidth(0, 760);
        _ui->projectTreeView->setColumnWidth(1, 120);
        _ui->projectTreeView->setColumnWidth(2, 120);
        QString resFolderPath = path + QString("/") + QString(QLatin1String("res"));
        _ui->projectTreeView->expand(_project->index(resFolderPath));

        // Open the last scene editor for the project.        
        openScene(path+ QString("/") + _project->scene());
    }
}

void ProjectView::closeProject()
{
    SAFE_DELETE(_project);
}

Project* ProjectView::project() const
{
    return _project;
}

void ProjectView::openScene(const QString& path)
{
    emit sceneOpened(path);
}

void ProjectView::searchTextChanged(const QString& text)
{ 
    // TODO: Get this working
    //_sortFilterProxyModel->setFilterFixedString(text);
}
