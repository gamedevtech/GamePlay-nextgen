#include "ProjectSortFilterProxyModel.h"
#include <QFileSystemModel>


ProjectSortFilterProxyModel::ProjectSortFilterProxyModel(QObject* parent) : QSortFilterProxyModel(parent)
{
}

ProjectSortFilterProxyModel::~ProjectSortFilterProxyModel()
{
}

bool ProjectSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{    
    if (sortColumn() == 0) 
    {
        QFileSystemModel* fsm = qobject_cast<QFileSystemModel*>(sourceModel());
        bool asc = sortOrder() == Qt::AscendingOrder ? true : false;
        QFileInfo leftFileInfo = fsm->fileInfo(left);
        QFileInfo rightFileInfo = fsm->fileInfo(right);
        // If DotAndDot move in the beginning
        if (sourceModel()->data(left).toString() == "..")
            return asc;
        if (sourceModel()->data(right).toString() == "..")
            return !asc;
        // Move directories to top
        if (!leftFileInfo.isDir() && rightFileInfo.isDir())
            return !asc;
        if (leftFileInfo.isDir() && !rightFileInfo.isDir())
            return asc;
    }
    return QSortFilterProxyModel::lessThan(left, right);
}