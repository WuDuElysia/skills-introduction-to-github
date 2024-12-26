#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter> //实现窗口重绘
#include <QApplication>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QLabel>
#include <QSlider>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "toolclass.h"
#include "databaseManager.h"
#include "Page/page1.h"
#include "Page2/page2.h"
#include "Page3/page3.h"
#include "Page4/page4.h"
#include "Page5/page5.h"
#include "Page6/page6.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    //处理窗口ui图片
    void paintEvent(QPaintEvent* event);

private slots:
    void on_pushButton_Close_clicked();

    void on_pushButton_Large_clicked();

    void on_pushButton_Little_clicked();

    void onButtonClicked(QPushButton *clickedButton);

    void on_pushButton_PlayerAction_clicked();




    void on_pushButton_player_like_clicked();

    void setPosition(int position);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);

    void on_pushButton_player_right_clicked();

    void on_pushButton_player_left_clicked();

    void action_music_player(const QString &img_music,
                             const QString &music_name,
                             const QString &music_composer,
                             const int &time_long,
                             const bool &is_like,
                             const int &music_form_from);

private:
    Ui::Widget *ui;
    bool playerActionChecked; // 用于跟踪状态
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QPushButton *currentButton = nullptr;
    bool is_like_checked;
    QString music_now;
    int music_form_from;
    DatabaseManager *dbManager;

    void set_ui();
    void on_pushButton_player_like_select();
    void select_img_playerActionChecked();
    void initialize_music_player();
    void stopMusic();



};



#endif // WIDGET_H
