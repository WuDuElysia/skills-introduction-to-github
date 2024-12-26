#include "page4.h"
#include "ui_page4.h"
#include "songwidget.h"

Page4::Page4(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Page4)
{
    ui->setupUi(this);
    dbManager = new DatabaseManager("example.db","Page4",this);
    set_ui();




    //向上传递信号
    connect(ui->widget,&Page4_1::double_clicked_item_to_music_player,this,&Page4::page4_1_double_clicked_item_to_music_player);

    //更换首图片以及刷新列表
    connect(ui->widget,&Page4_1::clear_and_add,this,&Page4::clear_and_addsong);



}





Page4::~Page4()
{
    delete ui;
    delete dbManager;
}

void Page4::clear_and_addsong()
{

    ui->widget->addSong();
    set_headimg();
}

void Page4::set_ui()
{
    // 设置滚动条的样式
    setStyleSheet("QScrollBar:vertical {"
                  "   border: 2px;"
                  "   border-radius: 2px;"
                  "   background: #ddd;"
                  "   width: 6px;"
                  "   margin: 22px 0 22px 0;"
                  "}"
                  "QScrollBar::handle:vertical {"
                  "   background: #888;"
                  "   border-radius: 2px;"
                  "   min-height: 30px;"
                  "}"
                  "QScrollBar::add-line:vertical {"
                  "   background: none;"
                  "   height: 0px;"
                  "   border-radius: 2px;"
                  "}"
                  "QScrollBar::sub-line:vertical {"
                  "   background: none;"
                  "   height: 0px;"
                  "   border-radius: 2px;"
                  "}"
                  );

    set_headimg();

}

void Page4::set_headimg()
{
    ui->label->clear();
    // 加载图像
    toolclass img_file_path;
    if (!dbManager->openDatabase()) {
        qDebug() << "Failed to open database.";
        // 处理失败情况，例如关闭程序或提示用户
        return; // 或者抛出异常、退出应用程序等
    }
    QVector<QString> first_message = dbManager->getFirstLikeRowData();
    if(first_message.isEmpty()){
        QString img_path = img_file_path.getExecutableImagePath() + "/" + "000.jpg";
        QPixmap pixmap(img_path); // 这里使用 QString 路径
        ui->label->setPixmap(pixmap);
        ui->label->setScaledContents(true); // 图像会缩放以适应 QLabel 的大小
    }else{
        QString img_first_path = first_message[1];
        //qDebug()<<img_first_path;
        QString img_path = img_file_path.getExecutableImagePath() + "/" + img_first_path;
        QPixmap pixmap(img_path); // 这里使用 QString 路径
        ui->label->setPixmap(pixmap);
        ui->label->setScaledContents(true); // 图像会缩放以适应 QLabel 的大小
    }
}
