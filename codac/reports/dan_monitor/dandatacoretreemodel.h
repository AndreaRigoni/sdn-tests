#ifndef DANDATACORETREEMODEL_H
#define DANDATACORETREEMODEL_H

#include <qabstractitemmodel.h>

class danDataCoreTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit danDataCoreTreeModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const ;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) ;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const ;
    QModelIndex parent(const QModelIndex &index) const ;

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const ;

    // Fetch data dynamically:
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const ;

    bool canFetchMore(const QModelIndex &parent) const ;
    void fetchMore(const QModelIndex &parent) ;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const ;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) ;

    Qt::ItemFlags flags(const QModelIndex& index) const ;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) ;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) ;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) ;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) ;

private:
};

#endif // DANDATACORETREEMODEL_H
