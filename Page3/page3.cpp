#include "page3.h"
#include "ui_page3.h"

Page3::Page3(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Page3)
{
    ui->setupUi(this);
}

Page3::~Page3()
{
    delete ui;
}
