#ifndef PAGE6_H
#define PAGE6_H

#include <QWidget>
#include <QLabel>
#include <QListWidgetItem>
#include "songwidget.h"
#include "databasemanager.h"
#include "page6_1.h"
namespace Ui {
class Page6;
}

class Page6 : public QWidget
{
    Q_OBJECT

public:
    explicit Page6(QWidget *parent = nullptr);
    void setDbManager(DatabaseManager *manager);
    ~Page6();

signals:
    void page6_1_double_clicked_item_to_music_player(const QString &img_music,
                                                     const QString &music_name,
                                                     const QString &music_composer,
                                                     const int &time_long,
                                                     const bool &is_like,
                                                     const int &music_form_from);

public slots:
    void clear_and_addsong();
private:
    Ui::Page6 *ui;
    void set_ui();
    void set_headimg();
    DatabaseManager *dbManager; // 添加数据库管理器指针
};

#endif // PAGE6_H
