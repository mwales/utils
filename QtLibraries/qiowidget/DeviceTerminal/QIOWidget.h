#ifndef QIOWIDGET_H
#define QIOWIDGET_H

#include <QWidget>

class QIODevice;

class QIOWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QIOWidget(QWidget *parent = 0);



    virtual QIODevice* getQIoDevice() = 0;

signals:

public slots:

    virtual void connectToDevice() = 0;
};

#endif // QIOWIDGET_H
