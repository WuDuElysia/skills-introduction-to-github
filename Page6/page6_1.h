#ifndef PAGE6_1_H
#define PAGE6_1_H

#include <QWidget>
#include <QLabel>
#include <QListWidgetItem>
#include "toolclass.h"
#include "databaseManager.h"  // 添加这一行以使用 DatabaseManager
#include "songwidget.h"

namespace Ui {
class Page6_1;
}

class Page6_1 : public QWidget
{
    Q_OBJECT

public:
    explicit Page6_1(QWidget *parent = nullptr);
    void setDbManager(DatabaseManager *manager);

    ~Page6_1();

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
    Ui::Page6_1 *ui;
    DatabaseManager *dbManager; // 添加数据库管理器指针
    void double_clicked_item_to_music_player_transfer(QListWidgetItem *item);
    void delete_songWidget_in_QListWidget(QListWidget *listWidget);
    void initialize_music_player();
};

#endif // PAGE6_1_H