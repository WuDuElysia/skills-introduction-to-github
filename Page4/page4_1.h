#ifndef PAGE4_1_H
#define PAGE4_1_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include "toolclass.h"
#include "databaseManager.h"  // 添加这一行以使用 DatabaseManager
#include "songwidget.h"


namespace Ui {
class Page4_1;
}

class Page4_1 : public QWidget
{
    Q_OBJECT

public:
    explicit Page4_1(QWidget *parent = nullptr);
    void updateListHeight();
    ~Page4_1();
signals:
    void double_clicked_item_to_music_player(const QString &img_music,
                                             const QString &music_name,
                                             const QString &music_composer,
                                             const int &time_long,
                                             const bool &is_like,
                                             const int &music_form_from); // 自定义信号
    void clear_and_add();
public slots:
     void addSong();

private slots:

    void update_is_like(QString &music_name,
                        bool &is_like);

private:
    Ui::Page4_1 *ui;
    DatabaseManager *dbManager; // 添加数据库管理器指针
    QLabel *img_head;
    void double_clicked_item_to_music_player_transfer(QListWidgetItem *item);
    void delete_songWidget_in_QListWidget(QListWidget *listWidget);


};

#endif // PAGE4_1_H
