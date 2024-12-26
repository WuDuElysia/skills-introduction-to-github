#include "toolclass.h"

toolclass::toolclass(QObject *parent)
    : QObject{parent}
{}

QString toolclass::getExecutableMediaPath()
{
    // 获取程序可执行文件所在目录
    QString execDir = QCoreApplication::applicationDirPath();

    // 定义 media 文件夹路径
    QString mediaPath = QDir(execDir).absoluteFilePath("media");

    // 确保 media 文件夹存在
    QDir().mkpath(mediaPath);

    return mediaPath;
}

QString toolclass::getExecutableImagePath()
{
    // 获取程序可执行文件所在目录
    QString execDir = QCoreApplication::applicationDirPath();

    // 定义 img_music 文件夹路径
    QString img_music_Path = QDir(execDir).absoluteFilePath("img_music");

    // 确保 media 文件夹存在
    QDir().mkpath(img_music_Path);

    return img_music_Path;
}
