#ifndef TOOLCLASS_H
#define TOOLCLASS_H

#include <QObject>
#include <QDir>
#include <QApplication>

class toolclass : public QObject
{
    Q_OBJECT
public:
    explicit toolclass(QObject *parent = nullptr);
    QString getExecutableMediaPath();
    QString getExecutableImagePath();

signals:
};

#endif // TOOLCLASS_H
