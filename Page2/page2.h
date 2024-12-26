#ifndef PAGE2_H
#define PAGE2_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Page2;
}
QT_END_NAMESPACE

class Page2 : public QWidget
{
    Q_OBJECT

public:
    explicit Page2(QWidget *parent = nullptr);
    ~Page2();

private:
    Ui::Page2 *ui;
};

#endif // PAGE2_H
