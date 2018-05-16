
#include <iostream>

#include <qabstractitemmodel.h>
#include <qvariant.h>


#include <dan/dan_Admin.h>
#include <dan/dan_Source.h>
#include <dan/dan_DataCore.h>

#include "ddcmodel.h"

extern "C" {
#include "dan_DataCore_st.h"
#include "dan_Source_st.h"
#include "dan_Subscriber_st.h"
#include "dan_Admin_st.h"
}



static int32_t isRunning(const char *progName, char *streamerName)
{
    char comm[2000];
    char finalName[DAN_MAX_STR];
    char instances[1000];
    int32_t ret=0;

    if((progName==NULL) || (!strcmp(progName,""))) {
        strcpy(finalName,"NULL");
    }
    else {
        strcpy(finalName,progName);
    }

    sprintf(comm, "/usr/bin/pgrep -f \".*danStreamer_.*%s %s\"",finalName,streamerName);
    FILE* app = popen(comm, "r");
    if (app) {
        if(fgets(instances, sizeof(instances), app) !=NULL ) {
            ret=1;
        }
        pclose(app);
    }
    return ret;
}


////////////////////////////////////////////////////////////////////////////////
//  TREE ITEM  /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


DDCTreeModelItem::DDCTreeModelItem(const QVector<QVariant> &data, DDCTreeModelItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}



DDCTreeModelItem::~DDCTreeModelItem()
{
    qDeleteAll(m_childItems);
}

void DDCTreeModelItem::appendChild(DDCTreeModelItem *item)
{
    m_childItems.append(item);
}

DDCTreeModelItem *DDCTreeModelItem::child(int row)
{
    return m_childItems.value(row);
}

int DDCTreeModelItem::childCount() const
{
    return m_childItems.count();
}

int DDCTreeModelItem::columnCount() const
{
    return m_itemData.count();
}

QVariant DDCTreeModelItem::data(int column) const
{
    return m_itemData.value(column);
}

DDCTreeModelItem *DDCTreeModelItem::parentItem()
{
    return m_parentItem;
}

int DDCTreeModelItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<DDCTreeModelItem*>(this));

    return 0;
}

//! [7]
bool   DDCTreeModelItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
          DDCTreeModelItem *item = new DDCTreeModelItem(data, this);
       m_childItems.insert(position, item);
    }

    return true;
}
//! [7]

//! [8]
bool   DDCTreeModelItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > m_itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData.insert(position, QVariant());

    foreach (  DDCTreeModelItem *child,m_childItems)
        child->insertColumns(position, columns);

    return true;
}
//! [8]


//! [10]
bool   DDCTreeModelItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count >m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);

    return true;
}
//! [10]

bool   DDCTreeModelItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > m_itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData.remove(position);

    foreach (  DDCTreeModelItem *child,m_childItems)
        child->removeColumns(position, columns);

    return true;
}




ICProgram::ICProgram(const QVector<QVariant> &data, DDCTreeModelItem *parentItem)
    : BaseClass(data,parentItem)
{

}

QString ICProgram::getICName()
{
    st_dan_DataCore *dc = (st_dan_DataCore*)m_dataCore;
    if(dc) return QString(dc->datacoreInfo->progName);
    else return QString("");
}


Streamer::Streamer(const QVector<QVariant> &data, DDCTreeModelItem *parentItem)
    : BaseClass(data,parentItem)
{
}

QString Streamer::getICName()
{
    return parentItem()->getICName();
}





////////////////////////////////////////////////////////////////////////////////
//  DDC MODEL  /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


DDCTreeModel::DDCTreeModel(QObject *parent)
{
    (void)parent;
    QVector<QVariant> rootData;
    rootData << "name" << "subscribed" << "status";
    rootItem = new DDCTreeModelItem(rootData);

    update();
}


DDCTreeModel::~DDCTreeModel()
{
    delete rootItem;
}

int DDCTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<DDCTreeModelItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}


bool DDCTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool DDCTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    DDCTreeModelItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

bool  DDCTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool  DDCTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    DDCTreeModelItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}



QList<Streamer *> DDCTreeModel::getStreamersList() const
{
    QList<Streamer *> res;
    foreach(DDCTreeModelItem *ic, rootItem->m_childItems)
    {
        foreach (DDCTreeModelItem *el, ic->m_childItems) {
            if(el->type() == DDCTreeModelItem::StreamerType)
                res.append((Streamer *)el);
        }
    }
    return res;
}

void DDCTreeModel::load_from_file(const char *filename)
{
    dan_admin_api_load_fromFile(filename);
    this->update();
}

QVariant DDCTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    DDCTreeModelItem *item = static_cast<DDCTreeModelItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags DDCTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant DDCTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex DDCTreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DDCTreeModelItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DDCTreeModelItem*>(parent.internalPointer());

    DDCTreeModelItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DDCTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    DDCTreeModelItem *childItem = static_cast<DDCTreeModelItem*>(index.internalPointer());
    DDCTreeModelItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DDCTreeModel::rowCount(const QModelIndex &parent) const
{
    DDCTreeModelItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DDCTreeModelItem*>(parent.internalPointer());

    return parentItem->childCount();
}

// operator for find algorithm //
bool operator == (const dan_DataCore &_data, const QString &str) {
    st_dan_DataCore * data = (st_dan_DataCore *)_data;
    if(!data || !data->datacoreInfo || !data->datacoreInfo->progName) return false;
    return QString(data->datacoreInfo->progName) == str;
}

bool operator == (const DDCTreeModelItem *&item, const QString &str) {
    if(!item) return false;
    return item->data(0).toString() == str;
}

DDCTreeModelItem *DDCTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        DDCTreeModelItem *item = static_cast<DDCTreeModelItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

void DDCTreeModel::update()
{
    // UPDATE DATACORE LIST //
    int32_t num = 0;
    char ** icp = dan_admin_api_getAllLoadedICPrograms(&num);

    // check if removed IC //
    foreach (dan_DataCore _el, m_dclist) {
        st_dan_DataCore * el = (st_dan_DataCore *)_el;
        int found = 0;
        for(int j=0; j<num; ++j) {
            if(QString(el->datacoreInfo->progName) == QString(icp[j]))
                found = 1;
        }
        if(!found) { // remove //
            dan_closeLibrary(_el);
            m_dclist.removeOne(_el);
            QList<DDCTreeModelItem*>::iterator it = std::find(rootItem->m_childItems.begin(),
                                                              rootItem->m_childItems.end(),
                                                              QString(el->datacoreInfo->progName));

            // rootItem->
            this->removeRow((*it)->row());
            //this->reset();
        }
    }

    // check if not already in list //
    for(int i=0; i<num; ++i) {
        dan_DataCore _dc = 0;
        foreach(dan_DataCore _el, m_dclist) {
            st_dan_DataCore * el = (st_dan_DataCore *)_el;
            if(QString(el->datacoreInfo->progName) == QString(icp[i]))
                _dc = _el; //found
        }
        if(_dc==0) {
            std::cout << "ADDING NEW \n";
            _dc = dan_initLibrary_icprog(icp[i]);
            m_dclist << _dc;
            st_dan_DataCore *dc=(st_dan_DataCore *)_dc;
            QVector<QVariant> rowData;
            rowData << dc->datacoreInfo->progName << "" << "";
            ICProgram *icp = new ICProgram(rowData,rootItem);
            icp->m_dataCore = _dc;
            rootItem->appendChild(icp);
            for (int j=0; j < dc->datacoreInfo->nSubscribers; ++j) {
                QVector<QVariant> rowData;
                int subscribed = dc->datacoreInfo->subscribers_info[j].subscribed;
                int running    = isRunning(dc->datacoreInfo->progName,
                                           dc->datacoreInfo->subscribers_info[j].name);
                rowData << QString(dc->datacoreInfo->subscribers_info[j].name);
                subscribed ? rowData << QString("yes") : rowData << QString("no");
                running    ? rowData << QString("RUNNING") : rowData << QString("STOPPED");
                Streamer *st = new Streamer(rowData, icp);
                st->m_dataStreamInfo = &dc->datacoreInfo->subscribers_info[j];
                icp->appendChild(st);
            }
            //   this->reset();
        }
        free(icp[i]);
    }
    free(icp);
}



