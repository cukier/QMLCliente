#include "filterproxymodel.h"

FilterProxyModel::FilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setSortOrder(false);
}

void FilterProxyModel::setFilterString(QString string)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterFixedString(string);
}

void FilterProxyModel::setSortOrder(bool checked)
{
    if(checked)
        sort(0, Qt::DescendingOrder);
    else
        sort(0, Qt::AscendingOrder);
}
