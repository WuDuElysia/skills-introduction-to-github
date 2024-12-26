#include "page2.h"
#include "ui_page2.h"

Page2::Page2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Page2)
{
    ui->setupUi(this);
}

Page2::~Page2()
{
    delete ui;
}
