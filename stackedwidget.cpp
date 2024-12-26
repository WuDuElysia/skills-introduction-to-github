#include "stackedwidget.h"


StackedWidget::StackedWidget(QWidget *parent)
    : QStackedWidget(parent)
    , ui(new Ui::StackedWidget)
{
    ui->setupUi(this);
}

StackedWidget::~StackedWidget()
{
    delete ui;
}
