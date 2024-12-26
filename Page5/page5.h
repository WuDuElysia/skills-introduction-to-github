#ifndef PAGE5_H
#define PAGE5_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidgetItem>
#include "songwidget.h"
#include "databasemanager.h"
#include "page5_1.h"

namespace Ui {
class Page5;
}

class Page5 : public QWidget
{
    Q_OBJECT

public:
    explicit Page5(QWidget *parent = nullptr);
    ~Page5();

signals:
    void page5_1_double_clicked_item_to_music_player(const QString &img_music,
                                                     const QString &music_name,
                                                     const QString &music_composer,
                                                     const int &time_long,
                                                     const bool &is_like,
                                                     const int &music_form_from);

public slots:
    void clear_and_addsong();
private:

    Ui::Page5 *ui;
    DatabaseManager *dbManager; // 添加数据库管理器指针
    void set_ui();
};

#endif // PAGE5_H
