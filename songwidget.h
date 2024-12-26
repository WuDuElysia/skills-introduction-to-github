#ifndef SONGWIDGET_H
#define SONGWIDGET_H

#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QPixmap>
#include <QTime>
#include "toolclass.h"


namespace Ui {
class SongWidget; // 这是由 Qt Designer 自动生成的 namespace
}

class SongWidget : public QWidget
{
    Q_OBJECT
public:
    SongWidget(
            const int &music_id,
            const QString &img_music,
            const QString &music_name,
            const QString &music_composer,
            const int &collection,
            const bool &is_like,
            QWidget *parent = nullptr);

    int music_id;
    QString img_music;
    QString music_name;
    QString music_composer;
        int collection;
        bool is_like;

signals:
        void is_like_change(QString &music_name,bool &is_like);

private slots:


    void on_pushButton_Like_clicked();

private:
    Ui::SongWidget *ui; // 指向 UI 组件的指针
    void on_pushButton_Like_select();
    void set_ui();
};

#endif // SONGWIDGET_H
