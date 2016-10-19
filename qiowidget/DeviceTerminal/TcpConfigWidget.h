#ifndef TCPDESTINATIONWIDGET_H
#define TCPDESTINATIONWIDGET_H

#include "QIOWidget.h"
#include <QWidget>
#include <QLineEdit>

class TcpConfigWidget : public QIOWidget
{
    Q_OBJECT

    Q_PROPERTY(int PortNumber WRITE SetPortNumber READ GetPortNumber)
    Q_PROPERTY(QString Hostname WRITE SetHostname READ GetHostname)


public:
    explicit TcpConfigWidget(QWidget *parent = 0);

    void SetPortNumber(int pn);

    void SetHostname(QString hostname);

    int GetPortNumber() const;
    QString GetHostname() const;

    virtual QIODevice* GetQIoDevice();
signals:

public slots:

    virtual void ConnectToDevice();

protected:
    QLineEdit theHostname;

    QLineEdit thePortNumber;

};

#endif // TCPDESTINATIONWIDGET_H
