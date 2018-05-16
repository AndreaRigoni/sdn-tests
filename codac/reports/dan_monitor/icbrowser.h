#ifndef ICBROWSER_H
#define ICBROWSER_H


#include <QWidget>

typedef void *dan_DataCore;

namespace Ui {
class ICBrowser;
}

class ICBrowser : public QWidget
{
    Q_OBJECT


public:
    explicit ICBrowser(QWidget *parent = 0);
    ~ICBrowser();

    void update(u_int32_t delay);

    void icLoadFromXML(const char *xml_file);

    class DDCTreeModel * getModel() const { return m_model; }

protected slots:
    void timerEvent(QTimerEvent *e );
    void print_current_index();

private:
    Ui::ICBrowser *ui;
    QList<dan_DataCore> m_dclist;
    class DDCTreeModel * m_model;
};

#endif // ICBROWSER_H
