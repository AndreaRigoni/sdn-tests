#ifndef ICBSTREAMER_H
#define ICBSTREAMER_H

#include <QWidget>

typedef void* dan_DataCore;

namespace Ui {
class ICBStreamer;
}

class ICBStreamer : public QWidget
{
    Q_OBJECT

public:
    explicit ICBStreamer(const char *name, QWidget *parent = 0);
    ~ICBStreamer();
    dan_DataCore getData() const { return m_data; }

private:
    dan_DataCore  m_data;
    Ui::ICBStreamer *ui;
};

#endif // ICBSTREAMER_H
