#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QVector>

class DatabaseManager: public QObject
{

    Q_OBJECT

public:
    DatabaseManager(const QString &dbName , const QString &connectionName,QObject *parent);
    ~DatabaseManager();

    bool openDatabase();          // 打开数据库
    void closeDatabase();         // 关闭数据库
    bool createTable();           // 创建表
    bool insertData(const QString &img_name , const QString &music_name, const QString &music_compose,const int &time_long, const bool &Like);   // 插入数据
    void queryData();             // 查询数据
    bool deleteData(const QString &music_name);     //删除数据
    QVector<QVector<QString>> getAllData();
    QVector<QVector<QString>> getLikeData();
    QVector<QString> getThisData(const QString &music_name);
    QVector<QString> getFirstRowData();
    QVector<QString> getFirstLikeRowData();
    QVector<QString> getNextMusicData(const QString &music_name);
    QVector<QString> getNextLikeMusicData(const QString &music_name);
    QVector<QString> getLastMusicData(const QString &music_name);
    QVector<QString> getLastLikeMusicData(const QString &music_name);
    bool updateIsLiked(const QString &music_name, bool is_liked);


    //最近播放
    bool createRecentlyTable();
    void recentlyData(); //查询最近播放数据
    bool saveRecentlyMusic(const QString &music_name);
    QVector<QString> getRecentlyThisData(const QString &music_name);
    QVector<QString> getRecentlyMusic();
signals:
    void isLikedUpdated();
private:
    QSqlDatabase db;              // 数据库对象
    QString connectionName;     //数据库连接名
};

#endif // DATABASEMANAGER_H
