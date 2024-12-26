#include "page4_1.h"

#include "ui_page4_1.h"



Page4_1::Page4_1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Page4_1)
{
    ui->setupUi(this);

    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    QString styleSheet = R"(
        QListWidget{
            border:none;
        }
        QListWidget::item {
            border-radius:15px;
        }
        QListWidget::item:hover {
            background-color: rgb(255, 235, 251);
        }

        QListWidget::item:selected {
            background-color: rgb(255, 235, 251);
            color: black;
            outline: none;
        }
    )";
    ui->listWidget->setStyleSheet(styleSheet);
    dbManager = new DatabaseManager("example.db","Page4_1",this);
    if (dbManager->openDatabase()) {  // 打开数据库
        //dbManager->createTable();      // 创建表
        //dbManager->insertData("Burn It All Down.jpg","Burn It All Down","英雄联盟",201,false);  // 插入数据
        //dbManager->deleteData("HOYO-MiX - Da Capo.jpg");    //删除数据

        dbManager->queryData();         // 查询数据
        addSong();
    }


    connect(ui->listWidget,&QListWidget::itemDoubleClicked,this,&Page4_1::double_clicked_item_to_music_player_transfer);




}


void Page4_1::addSong()
{
    //释放songWidget所占用的内存
    // 清空 QListWidget
    delete_songWidget_in_QListWidget(ui->listWidget);
    // 获取所有数据
    QVector<QVector<QString>> LikeSongs = dbManager->getLikeData();
    // 遍历所有歌曲数据并动态添加到 QListWidget
    int music_id = 1;
    for (const auto &song : LikeSongs) {
        // 假设 song 是一个 QVector<QString>，其中包含字段 [img_name, music_name, music_compose, time_long, Like]
        QString img_name = song[1];
        QString music_name = song[2];
        QString music_compose = song[3];
        int time_long = song[4].toInt();
        bool is_like = song[5] == "1";  // 解析 Like 字段
        SongWidget *songWidget = new SongWidget(music_id,img_name,music_name,music_compose,time_long,is_like);
        QListWidgetItem *ListWidget_Music = new QListWidgetItem(ui->listWidget);
        ListWidget_Music->setSizeHint(QSize(931,56));
        ui->listWidget->addItem(ListWidget_Music); // 添加至 QListWidget
        ui->listWidget->setItemWidget(ListWidget_Music,songWidget);
        music_id++;
        //将songWidget指针加入ListWidget_Music的用户数据，以便在删除ListWidget_Music时方便删除与之绑定的songWidget
        ListWidget_Music->setData(Qt::UserRole, QVariant::fromValue(songWidget));
        //接受songWidget传来的is_like更改信号
        connect(songWidget,&SongWidget::is_like_change,this,&Page4_1::update_is_like);

    }




}

void Page4_1::update_is_like(QString &music_name, bool &is_like)
{
    dbManager->updateIsLiked(music_name,is_like);
    emit clear_and_add();
}

Page4_1::~Page4_1()
{
    delete ui;
    delete dbManager; // 清理数据库管理器
}

void Page4_1::double_clicked_item_to_music_player_transfer(QListWidgetItem *item)
{
    SongWidget *songWidget = qobject_cast<SongWidget*>(ui->listWidget->itemWidget(item));
    if (songWidget) {
        // 这里的 songWidget 确实是用户双击的那个 item 所关联的 SongWidget
        QString img_music = songWidget->img_music;
        QString music_name = songWidget->music_name;
        QString music_composer = songWidget->music_composer;
        int time_long = songWidget->collection;
        bool is_like = songWidget->is_like;
        int music_form_from = 1;
        // 发射自定义信号并传递额外参数
        emit double_clicked_item_to_music_player(img_music, music_name,music_composer,time_long,is_like,music_form_from);
    }
}

void Page4_1::delete_songWidget_in_QListWidget(QListWidget *listWidget)
{
    while (listWidget->count() > 0) {
        QListWidgetItem *item = listWidget->takeItem(0); // 移除并获取第一个 item
        if (item) {
            // 从 item 中获取保存的 SongWidget 指针
            SongWidget *widget = item->data(Qt::UserRole).value<SongWidget*>();
            if (widget) {
                delete widget; // 删除 SongWidget
            }
            delete item; // 删除 QListWidgetItem
        }
    }
}

void Page4_1::updateListHeight()
{
    // 计算列表高度并更新
    int totalHeight = 0;
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        totalHeight += ui->listWidget->sizeHintForRow(i); // 每行所需的高度
    }
    ui->listWidget->setFixedHeight(totalHeight + 10); // 加些边距
}




