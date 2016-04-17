#ifndef SCENESORTFILTERPROXYMODEL_H
#define SCENESORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>


/**
 * Defines the sorting filter used for the scene hierarchy.
 */
class SceneSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    /**
     * Constructor.
     *
     * @param parent The parent object.
     */
    explicit SceneSortFilterProxyModel(QObject* parent = nullptr);

    /**
     * Destructor.
     */
    ~SceneSortFilterProxyModel();

    /**
     * @see QSortFilterProxyModel::filterAcceptsRow
     */
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;
};

#endif
