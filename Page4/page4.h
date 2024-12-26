#ifndef PAGE4_H
#define PAGE4_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidgetItem>
#include "songwidget.h"
#include "databasemanager.h"
#include "page4_1.h"

namespace Ui {
class Page4;
}

class Page4 : public QWidget
{
    Q_OBJECT

public:
    explicit Page4(QWidget *parent = nullptr);

    ~Page4();

signals:
    void page4_1_double_clicked_item_to_music_player(const QString &img_music,
                                                     const QString &music_name,
                                                     const QString &music_composer,
                                                     const int &time_long,
                                                     const bool &is_like,
                                                     const int &music_form_from);
public slots:
    void clear_and_addsong();

private:
    Ui::Page4 *ui;
    void set_ui();
    void set_headimg();
    DatabaseManager *dbManager; // 添加数据库管理器指针
};

#endif // PAGE4_H

