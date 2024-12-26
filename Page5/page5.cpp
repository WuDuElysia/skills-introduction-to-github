#include "page5.h"
#include "ui_page5.h"

Page5::Page5(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Page5)
{
    ui->setupUi(this);

    dbManager = new DatabaseManager("example.db","Page5",this);
    set_ui();

    //向上传递信号
    connect(ui->widget_2,&Page5_1::double_clicked_item_to_music_player,this,&Page5::page5_1_double_clicked_item_to_music_player);

    //更换首图片以及刷新列表
    connect(ui->widget_2,&Page5_1::clear_and_add,this,&Page5::clear_and_addsong);
}

Page5::~Page5()
{
    delete ui;
    delete dbManager;
}

void Page5::clear_and_addsong()
{
    ui->widget_2->addSong();
}

void Page5::set_ui()
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
}


