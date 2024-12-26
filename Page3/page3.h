#ifndef PAGE3_H
#define PAGE3_H

#include <QWidget>

namespace Ui {
class Page3;
}

class Page3 : public QWidget
{
    Q_OBJECT

public:
    explicit Page3(QWidget *parent = nullptr);
    ~Page3();

private:
    Ui::Page3 *ui;
};

#endif // PAGE3_H
