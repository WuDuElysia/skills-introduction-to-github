#ifndef PAGE1_H
#define PAGE1_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>
#include <QTimer>
#include <QPixmap>
#include <QStringList>
#include <QResizeEvent>


QT_BEGIN_NAMESPACE
namespace Ui {
class Page1;
}
QT_END_NAMESPACE


class Page1 : public QWidget
{
    Q_OBJECT
public:
    explicit Page1(QWidget *parent = nullptr);
    ~Page1();

signals:

private:
    QVBoxLayout *layout;
    Ui::Page1 *ui;

};



#endif // PAGE1_H
