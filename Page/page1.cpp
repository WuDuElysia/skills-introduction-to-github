#include "page1.h"
#include "ui_Page1.h"

Page1::Page1(QWidget *parent) : QWidget(parent),ui(new Ui::Page1){

    ui->setupUi(this);




}

Page1::~Page1() {
    delete ui; // 确保删除 ui 指针以释放内存
}



