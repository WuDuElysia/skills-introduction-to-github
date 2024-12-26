#include "widget.h"
#include "ui_widget.h"
#include "ui_page1.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    is_like_checked = false;
    playerActionChecked = false; // 用于跟踪状态

    Page1 *page1 = new Page1(this);
    Page2 *page2 = new Page2(this);
    Page3 *page3 = new Page3(this);
    Page4 *page4 = new Page4(this);
    Page5 *page5 = new Page5(this);
    Page6 *page6 = new Page6(this);

    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput;
    music_now = NULL;
    music_form_from = 0;  //默认音乐是所有表的

    player->setAudioOutput(audioOutput);

    audioOutput->setVolume(100);





    connect(player, &QMediaPlayer::errorOccurred, [](QMediaPlayer::Error error) {
        switch (error) {
        case QMediaPlayer::NoError:
            qDebug() << "No error.";
            break;
        case QMediaPlayer::ResourceError:
            qDebug() << "Resource Error: Could not access the resource.";
            break;
        case QMediaPlayer::FormatError:
            qDebug() << "Format Error: The format is not supported.";
            break;
        default:
            qDebug() << "Other Error occurred:" << error;
            break;
        }
    });
    connect(ui->horizontalSlider_player, &QSlider::sliderMoved, this, &Widget::setPosition);
    connect(player, &QMediaPlayer::durationChanged, this, &Widget::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &Widget::positionChanged);





    // 创建 StackedWidget 的实例并设置 UI
    // 将页面添加到 QStackedWidget 中
    ui->stackedWidget->addWidget(page1);
    ui->stackedWidget->addWidget(page2);
    ui->stackedWidget->addWidget(page3);
    ui->stackedWidget->addWidget(page4);
    ui->stackedWidget->addWidget(page5);
    ui->stackedWidget->addWidget(page6);

    // 设定想要默认显示的页面
    ui->stackedWidget->setCurrentIndex(0);



    // 连接按钮的点击信号到槽函数
    connect(ui->pushButton_1, &QPushButton::clicked, [=]() {
        ui->stackedWidget->setCurrentIndex(0); // 切换到第一个页面
    });
    connect(ui->pushButton_2, &QPushButton::clicked, [=]() {
        ui->stackedWidget->setCurrentIndex(1); // 切换到第二个页面
    });
    connect(ui->pushButton_3, &QPushButton::clicked, [=]() {
        ui->stackedWidget->setCurrentIndex(2);
    });
    connect(ui->pushButton_4, &QPushButton::clicked, [=]() {
        ui->stackedWidget->setCurrentIndex(3);
    });
    connect(ui->pushButton_5, &QPushButton::clicked, [=]() {
        ui->stackedWidget->setCurrentIndex(4);
    });
    connect(ui->pushButton_6, &QPushButton::clicked, [=]() {
        ui->stackedWidget->setCurrentIndex(5);
    });


    connect(ui->pushButton_1, &QPushButton::clicked, [=]() { onButtonClicked(ui->pushButton_1); });
    connect(ui->pushButton_2, &QPushButton::clicked, [=]() { onButtonClicked(ui->pushButton_2); });
    connect(ui->pushButton_3, &QPushButton::clicked, [=]() { onButtonClicked(ui->pushButton_3); });
    connect(ui->pushButton_4, &QPushButton::clicked, [=]() { onButtonClicked(ui->pushButton_4); });
    connect(ui->pushButton_5, &QPushButton::clicked, [=]() { onButtonClicked(ui->pushButton_5); });
    connect(ui->pushButton_6, &QPushButton::clicked, [=]() { onButtonClicked(ui->pushButton_6); });

    //点击刷新QListWidget
    connect(ui->pushButton_4, &QPushButton::clicked, page4,&Page4::clear_and_addsong);
    connect(ui->pushButton_5, &QPushButton::clicked, page5,&Page5::clear_and_addsong);
    connect(ui->pushButton_6, &QPushButton::clicked,page6,&Page6::clear_and_addsong);






    set_ui();

    //禁止窗口改变大小
    //this->setFixedSize(this->geometry().size());

    //去掉标题栏
    this->setWindowFlag(Qt::FramelessWindowHint);

    //窗口最小化
    connect(ui->pushButton_Little,&QPushButton::clicked,this,&Widget::on_pushButton_Little_clicked);

    //调用音乐播放器
    connect(page4,&Page4::page4_1_double_clicked_item_to_music_player,this,&Widget::action_music_player);
    connect(page5,&Page5::page5_1_double_clicked_item_to_music_player,this,&Widget::action_music_player);
    connect(page6,&Page6::page6_1_double_clicked_item_to_music_player,this,&Widget::action_music_player);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::set_ui()
{
    on_pushButton_player_like_select();
    ui->horizontalSlider_player->setStyleSheet(R"(
            QSlider {
                height: 2px;
                background: #D3D3D3;
                border-radius: 1px;
            }
            QSlider::groove:horizontal {    /*定义滑动条的轨道（即未填充的部分）的外观*/
                background: #D3D3D3;
                border-radius: 1px;
            }
            QSlider::handle:horizontal {        /*定义水平方向滑块的外观，包括颜色、大小和位置*/
                background: rgb(255, 170, 255);
                border: none;
                width: 6px;
                height: 6px;
                margin: -2px;       /* 设置滑块的外边距为 -4 像素，使其垂直居中于滑道 */
                border-radius: 3px;     /* 设置滑块的圆角半径为 6 像素，使其外观更为圆润 */
                opacity: 0;     /* 默认状态下滑块不可见（透明度为零） */
            }
            QSlider::handle:horizontal:hover {      /*定义当鼠标悬停在滑块上时的样式*/
                opacity: 1;     /* 当鼠标悬停在滑块上时，设置其透明度为 1，使滑块可见 */
            }
            QSlider::handle:horizontal:pressed {        /*定义当滑块被按下时的样式*/
                opacity: 1;     /* 当滑块被按下时，设置其透明度为 1，使滑块可见 */
            }
            QSlider::sub-page:horizontal {      /*定义滑块在已播放部分的外观*/
                background: rgb(255, 170, 255);        /* 设置已播放部分的颜色为粉红色 */
                border-radius: 1px;     /* 设置已播放部分的圆角半径 */
            }
            QSlider::add-page:horizontal {      /*定义滑块在未播放部分的外观*/
                background: #D3D3D3;        /* 设置未播放部分的颜色为浅灰色 */
                border-radius: 1px;     /* 设置未播放部分的圆角半径 */
            }
        )");
    // 设置按钮的固定高度// 设置按键高度
    ui->pushButton_1->setFixedHeight(30);
    ui->pushButton_2->setFixedHeight(30);
    ui->pushButton_3->setFixedHeight(30);
    ui->pushButton_4->setFixedHeight(30);
    ui->pushButton_5->setFixedHeight(30);
    ui->pushButton_6->setFixedHeight(30);
    initialize_music_player();
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //painter.drawPixmap(0,0,width(),height(),QPixmap(":/background/img/background.png"));
}

void Widget::on_pushButton_Close_clicked()
{
    //关闭窗口
    close();
}


void Widget::on_pushButton_Large_clicked()
{

}


void Widget::on_pushButton_Little_clicked()
{
    this->showMinimized();
}

void Widget::onButtonClicked(QPushButton *clickedButton)
{
    // 如果上一个按钮指针不为空，重置其颜色


    if (currentButton) {
        QString icon_0 = currentButton->objectName();
        QString iconPath_0 = QString(":/icon/img/%1_0.png").arg(icon_0);
        currentButton->setIcon(QIcon(iconPath_0));
        currentButton->setStyleSheet(""); // 恢复默认颜色
    }

    // 设置被点击按钮的新颜色
    QString icon = clickedButton->objectName();
    QString iconPath = QString(":/icon/img/%1_1.png").arg(icon);
    clickedButton->setIcon(QIcon(iconPath));
    clickedButton->setStyleSheet("background-color:rgb(255, 102, 105);"
                                "color:white;"
                                "padding-left: 10px;"
                                );

    // 更新当前按钮为被点击的按钮
    currentButton = clickedButton;
}






void Widget::on_pushButton_PlayerAction_clicked()
{
    playerActionChecked = !playerActionChecked; // 切换状态
    select_img_playerActionChecked();
}



void Widget::select_img_playerActionChecked()
{
    if(playerActionChecked){
        ui->pushButton_PlayerAction->setIcon(QIcon(":/icon/img/pushButton_PlayerAction_1.png"));
        player->play();
    }else{
        ui->pushButton_PlayerAction->setIcon(QIcon(":/icon/img/pushButton_PlayerAction_0.png"));
        player->pause();
    }
}







void Widget::on_pushButton_player_like_select()
{
    if(is_like_checked) {
        ui->pushButton_player_like->setIcon(QIcon(":/icon/img/pushButton_player_like_1.png"));
    } else {
        ui->pushButton_player_like->setIcon(QIcon(":/icon/img/pushButton_player_like_0.png"));
    }
}

void Widget::on_pushButton_player_like_clicked()
{
    is_like_checked = !is_like_checked; // 切换状态
    on_pushButton_player_like_select();
}

void Widget::setPosition(int position)
{
    player->setPosition(position);
}


void Widget::durationChanged(qint64 duration) {
    ui->horizontalSlider_player->setRange(0, duration);
    //totalDurationLabel->setText(formatTime(duration));
}

void Widget::positionChanged(qint64 position) {
    ui->horizontalSlider_player->setValue(position);
    //currentPositionLabel->setText(formatTime(position));
}

void Widget::stopMusic()
{
    player->stop();
    ui->horizontalSlider_player->setValue(0); // 将进度条重置
}





void Widget::on_pushButton_player_right_clicked()
{
    stopMusic();
    if(playerActionChecked){
        playerActionChecked = !playerActionChecked;
    }

    select_img_playerActionChecked();
    /*
    */
    DatabaseManager dbManager("example.db","on_pushButton_player_right_clicked",this);
    if(!dbManager.openDatabase()){
        qDebug()<<"on_pushButton_player_right_clicked打开数据库失败";
        return;
    }
    if(music_form_from){
        QVector<QString> Like_data = dbManager.getNextLikeMusicData(music_now);
        QString img_music = Like_data[1];
        QString music_name = Like_data[2];
        QString music_composer = Like_data[3];
        int time_long = Like_data[4].toInt();
        bool is_like = Like_data[5] == "1";  // 解析 Like 字段
        qDebug()<<music_name;
        action_music_player(img_music,music_name,music_composer,time_long,is_like,music_form_from);
    }else{
        QVector<QString> All_data = dbManager.getNextMusicData(music_now);
        QString img_music = All_data[1];
        QString music_name = All_data[2];
        QString music_composer = All_data[3];
        int time_long = All_data[4].toInt();
        bool is_like = All_data[5] == "1";  // 解析 Like 字段
        action_music_player(img_music,music_name,music_composer,time_long,is_like,music_form_from);
    }


}


void Widget::on_pushButton_player_left_clicked()
{
    stopMusic();
    if(playerActionChecked){
        playerActionChecked = !playerActionChecked;
    }
    select_img_playerActionChecked();
    /*
    */
    DatabaseManager dbManager("example.db","on_pushButton_player_left_clicked",this);
    if(!dbManager.openDatabase()){
        qDebug()<<"on_pushButton_player_left_clicked打开数据库失败";
        return;
    }
    if(music_form_from){
        QVector<QString> Like_data = dbManager.getLastLikeMusicData(music_now);
        QString img_music = Like_data[1];
        QString music_name = Like_data[2];
        QString music_composer = Like_data[3];
        int time_long = Like_data[4].toInt();
        bool is_like = Like_data[5] == "1";  // 解析 Like 字段
        qDebug()<<music_name;
        action_music_player(img_music,music_name,music_composer,time_long,is_like,music_form_from);
    }else{
        QVector<QString> All_data = dbManager.getLastMusicData(music_now);
        QString img_music = All_data[1];
        QString music_name = All_data[2];
        QString music_composer = All_data[3];
        int time_long = All_data[4].toInt();
        bool is_like = All_data[5] == "1";  // 解析 Like 字段
        action_music_player(img_music,music_name,music_composer,time_long,is_like,music_form_from);
    }
}

void Widget::action_music_player(const QString &img_music,
                                 const QString &music_name,
                                 const QString &music_composer,
                                 const int &time_long,
                                 const bool &is_like,
                                 const int &music_form_from)
{
    qDebug()<<"player is ready";
    this->music_form_from = music_form_from;
    this->music_now = music_name;
    DatabaseManager dbManager("example.db","action_music_player",this);
    if(dbManager.openDatabase()){
        dbManager.saveRecentlyMusic(music_name);
    }else{
        qDebug()<<"error open rencently table";
    }

    if(playerActionChecked){
        playerActionChecked = !playerActionChecked; // 切换状态
        select_img_playerActionChecked();
    }
    toolclass file_path;

    QString music_path = file_path.getExecutableMediaPath() + "/" + music_name + ".mp3";

    player->setSource(QUrl(music_path));
    // 加载图像
    toolclass img_file_path;
    QString img_path = img_file_path.getExecutableImagePath() + "/" + img_music;
    QPixmap pixmap(img_path); // 这里使用 QString 路径

    // 检查图片是否加载成功
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image from path:" << img_path;
    } else {
        // 设置图片到 QLabel
        ui->label_music_player_img_music->setPixmap(pixmap.scaled(ui->label_music_player_img_music->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    ui->label_music_player_music_name->setText(music_name);
    ui->label_music_player_music_composer->setText(music_composer);
    is_like_checked = is_like;
    on_pushButton_player_like_select();

}

void Widget::initialize_music_player()
{
    DatabaseManager dbManager("example.db","initialize",this);
    if(!dbManager.openDatabase()){
        qDebug()<<"initialize_music_player()打开数据库失败";
        return;
    }
    QVector<QString> FirstMusic = dbManager.getFirstRowData();
    QString img_music = FirstMusic[1];
    QString music_name = FirstMusic[2];
    QString music_composer = FirstMusic[3];
    int time_long = FirstMusic[4].toInt();
    bool is_like = FirstMusic[5] == "1";
    int music_form_from = 0;
    action_music_player(img_music,music_name,music_composer,time_long,is_like,music_form_from);
}


