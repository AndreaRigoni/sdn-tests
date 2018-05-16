#ifndef DDCMODEL_H
#define DDCMODEL_H


#include <QWidget>
#include <QAbstractItemView>

typedef void *dan_DataCore;
typedef void *dan_StreamInfo;

class DDCTreeModelItem
{
public:
    explicit DDCTreeModelItem(const QVector<QVariant> &data, DDCTreeModelItem *parentItem = 0);
    virtual ~DDCTreeModelItem();

    void appendChild(DDCTreeModelItem *child);
    DDCTreeModelItem *parentItem();
    DDCTreeModelItem *child(int row);
    int childCount() const;

    virtual int columnCount() const;
    virtual QVariant data(int column) const;
    virtual int row() const;

    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);


    virtual QString getICName() { return QString("[unknown]"); }
    virtual void update() {}

    enum DDCTreeModelItemTypeEnum {
        rootType,
        ICprogramType,
        StreamerType
    };
    virtual enum DDCTreeModelItemTypeEnum type() const { return rootType; }
private:
    QList<DDCTreeModelItem*>  m_childItems;
    QVector<QVariant>  m_itemData;
    DDCTreeModelItem * m_parentItem;

    friend class DDCTreeModel;
    friend class ICBrowser;
};


class ICProgram : public DDCTreeModelItem {
    typedef DDCTreeModelItem BaseClass;
public:
    ICProgram(const QVector<QVariant> &data, DDCTreeModelItem *parentItem = 0);
    QString getICName();
    enum DDCTreeModelItemTypeEnum type() const { return ICprogramType; }
    dan_DataCore  m_dataCore;
private:
    friend class DDCTreeModel;
    friend class ICBrowser;
};


class Streamer : public DDCTreeModelItem {
    typedef DDCTreeModelItem BaseClass;
public:
    Streamer(const QVector<QVariant> &data, DDCTreeModelItem *parentItem = 0);
    QString getICName();
    enum DDCTreeModelItemTypeEnum type() const { return StreamerType; }
    dan_StreamInfo m_dataStreamInfo;
private:
    friend class DDCTreeModel;
    friend class ICBrowser;
};


////////////////////////////////////////////////////////////////////////////////
//  DDC MODEL  /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


class DDCTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    DDCTreeModel(QObject *parent = 0);
    ~DDCTreeModel();

    QVariant data(const QModelIndex &index, int role) const ;
    Qt::ItemFlags flags(const QModelIndex &index) const ;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const ;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const ;
    QModelIndex parent(const QModelIndex &index) const ;
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const ;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) ;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) ;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) ;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) ;

    QList<Streamer *> getStreamersList() const;
    void load_from_file(const char *filename);

public slots:
    void update();

private:
    DDCTreeModelItem *getItem(const QModelIndex &index) const;

    QList<dan_DataCore> m_dclist;
    DDCTreeModelItem *rootItem;
};


#endif // DDCMODEL_H
