#include "songwidget.h"
#include "ui_songwidget.h"

SongWidget::SongWidget(
                    const int &music_id,
                    const QString &img_music,
                    const QString &music_name,
                    const QString &music_composer,
                    const int &time_long,
                    const bool &is_like,
                    QWidget *parent)
    : QWidget(parent),ui(new Ui::SongWidget)
{
    ui->setupUi(this);
    this->music_id = music_id;
    this->img_music = img_music;
    this->music_name = music_name;
    this->music_composer = music_composer;
    this->collection = time_long;
    this->is_like = is_like;

    set_ui();

}



void SongWidget::on_pushButton_Like_clicked()
{
    is_like=!is_like;
    on_pushButton_Like_select();
    emit is_like_change(music_name,is_like);

}

void SongWidget::on_pushButton_Like_select()
{
    if(is_like){
        ui->pushButton_Like->setIcon(QIcon(":/icon/img/Like_1.png")); // 点赞后变为红色心
    }else{
        ui->pushButton_Like->setIcon(QIcon(":/icon/img/Like_0.png"));
    }
}

void SongWidget::set_ui()
{
    on_pushButton_Like_select();

    ui->label_id->setText(QString::number(music_id));
    ui->label_name->setText(music_name);
    ui->label_composer->setText(music_composer);

    // 加载图像
    toolclass img_file_path;
    QString img_path = img_file_path.getExecutableImagePath() + "/" + img_music;
    QPixmap pixmap(img_path); // 这里使用 QString 路径

    // 检查图片是否加载成功
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image from path:" << img_path;
    } else {
        // 设置图片到 QLabel
        ui->label_img->setPixmap(pixmap.scaled(ui->label_img->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    QTime time(0, collection / 60, collection % 60);
    QString time_long = time.toString("mm:ss");
    ui->label_collection->setText(time_long);
}

