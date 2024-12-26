#include "databaseManager.h"
#include <QUuid>
#include <QDebug>

// 构造函数接收一个可选的连接名称，默认为基于 UUID 的唯一名称
DatabaseManager::DatabaseManager(const QString &dbName, const QString &connectionName, QObject *parent)
    : QObject(parent), connectionName(connectionName.isEmpty() ? QUuid::createUuid().toString() : connectionName) {
    db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(dbName);
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
    qDebug() << "Removing database connection:" << connectionName;
    QSqlDatabase::removeDatabase(connectionName); // 移除数据库连接
}

bool DatabaseManager::openDatabase()
{
    if (!db.open()) {
        qDebug() << "Could not open database:" << db.lastError().text();
        return false;
    } else {
        qDebug() << "打开数据库成功"<<connectionName;
    }
    return true;
}

void DatabaseManager::closeDatabase()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::createTable()
{
    QSqlQuery query(db); // 明确指定使用 db 连接
    if (!query.exec("CREATE TABLE IF NOT EXISTS my_table ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "img_name TEXT, "
                    "music_name TEXT, "
                    "music_compose TEXT, "
                    "time_long INTEGER, "
                    "is_liked INTEGER)")) {
        qDebug() << "Create table failed:" << query.lastError();
        return false;
    } else {
        qDebug() << "创建成功";
        return true;
    }
}

bool DatabaseManager::insertData(const QString &img_name, const QString &music_name,
                                 const QString &music_compose, const int &time_long,
                                 const bool &is_liked) {
    QSqlQuery query(db); // 明确指定使用 db 连接
    query.prepare("INSERT INTO my_table (img_name, music_name, music_compose, time_long, is_liked) "
                  "VALUES (:img_name, :music_name, :music_compose, :time_long, :is_liked)");

    query.bindValue(":img_name", img_name);
    query.bindValue(":music_name", music_name);
    query.bindValue(":music_compose", music_compose);
    query.bindValue(":time_long", time_long);
    query.bindValue(":is_liked", is_liked ? 1 : 0); // 使用整数作为参数

    if (!query.exec()) {
        qDebug() << "Insert failed:" << query.lastError() << "Bound values:" << query.boundValues(); // 输出详细错误信息
        return false;
    }

    // 调试：插入后立即查询新插入的数据，确认插入正确
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT id, img_name, music_name, music_compose, time_long, is_liked "
                       "FROM my_table WHERE id = LAST_INSERT_ROWID()");
    if (!checkQuery.exec()) {
        qDebug() << "Check insert failed:" << checkQuery.lastError();
        return false;
    }

    if (checkQuery.next()) {
        qDebug() << "Inserted row ID:" << checkQuery.value(0).toInt();
        qDebug() << "Inserted img_name:" << checkQuery.value(1).toString();
        qDebug() << "Inserted music_name:" << checkQuery.value(2).toString();
        qDebug() << "Inserted music_compose:" << checkQuery.value(3).toString();
        qDebug() << "Inserted time_long:" << checkQuery.value(4).toInt();
        qDebug() << "Inserted is_liked:" << checkQuery.value(5).toBool();
    } else {
        qDebug() << "Failed to retrieve inserted data.";
        return false;
    }

    return true;
}

void DatabaseManager::queryData()
{
    if (!db.isOpen()) {
        qDebug() << "Here is queryData" << "Error: Database is not open.";
        return;
    }

    QSqlQuery query(db); // 明确指定使用 db 连接
    if (!query.exec("SELECT * FROM my_table")) {
        qDebug() << "Error executing query:" << query.lastError();
        return;
    }

    while (query.next()) {
        QString img_name = query.value(1).toString();
        QString music_name = query.value(2).toString();
        QString music_compose = query.value(3).toString();
        int time_long = query.value(4).toInt();
        bool is_liked = query.value(5).toBool();
        qDebug() << "img_name:" << img_name << "music_name:" << music_name << "music_compose:" << music_compose << "time_long:" << time_long << "is_liked:" << is_liked;
    }
}

bool DatabaseManager::deleteData(const QString &music_name)
{
    if (!db.transaction()) { // 开始事务
        qDebug() << "Transaction start failed:" << db.lastError();
        return false;
    }

    QSqlQuery query(db); // 明确指定使用 db 连接
    query.prepare("DELETE FROM my_table WHERE music_name = :music_name");
    query.bindValue(":music_name", music_name);

    bool success = query.exec();
    if (!success) {
        qDebug() << "Delete failed:" << query.lastError() << "Bound values:" << query.boundValues();
        db.rollback(); // 发生错误时回滚事务
        return false;
    }

    if (!db.commit()) { // 提交事务
        qDebug() << "Commit failed:" << db.lastError();
        return false;
    }

    return true;
}

bool DatabaseManager::updateIsLiked(const QString &music_name, bool is_liked)
{
    QSqlQuery query(db); // 明确指定使用 db 连接

    // 首先检查是否存在该音乐名称的记录
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT id FROM my_table WHERE music_name = ?");
    checkQuery.bindValue(0, music_name);

    if (!checkQuery.exec()) {
        qDebug() << "Check failed:" << checkQuery.lastError();
        return false;
    }

    if (!checkQuery.next()) {
        qDebug() << "No record found for music_name:" << music_name;
        return false;
    }

    int id = checkQuery.value(0).toInt();

    // 更新 is_liked 字段
    query.prepare("UPDATE my_table SET is_liked = :is_liked WHERE id = :id");
    query.bindValue(":is_liked", is_liked ? 1 : 0); // 使用整数作为参数
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Update failed:" << query.lastError() << "Bound values:" << query.boundValues(); // 输出详细错误信息
        return false;
    }

    // 更新成功后通知界面刷新
    emit isLikedUpdated();  // 自定义信号，通知界面刷新

    return true;
}


QVector<QVector<QString>> DatabaseManager::getAllData()
{
    QVector<QVector<QString>> allData;  // 用于存储所有记录

    QSqlQuery query(db); // 明确指定使用 db 连接
    if (!query.exec("SELECT id, img_name, music_name, music_compose, time_long, is_liked FROM my_table")) {
        qDebug() << "Error executing query:" << query.lastError();
        return allData;
    }

    while (query.next()) {
        QVector<QString> rowData;  // 用于存储当前行的数据
        rowData.append(query.value(0).toString());  // id
        rowData.append(query.value(1).toString());  // img_name
        rowData.append(query.value(2).toString());  // music_name
        rowData.append(query.value(3).toString());  // music_compose
        rowData.append(QString::number(query.value(4).toInt()));  // time_long
        rowData.append(query.value(5).toBool() ? "1" : "0");  // is_liked (存储为字符串)

        allData.append(rowData);  // 将当前行的数据添加到所有记录中
    }
    return allData;  // 返回所有数据
}

QVector<QVector<QString>> DatabaseManager::getLikeData()
{
    QVector<QVector<QString>> likeData;  // 用于存储所有记录

    QSqlQuery query(db); // 明确指定使用 db 连接
    if (!query.exec("SELECT * FROM my_table WHERE is_liked = 1")) { // 使用 1 表示 true
        qDebug() << "Error executing query:" << query.lastError();
        return likeData;
    }

    while (query.next()) {
        QVector<QString> rowData;  // 用于存储当前行的数据
        rowData.append(query.value(0).toString());  // id
        rowData.append(query.value(1).toString());  // img_name
        rowData.append(query.value(2).toString());  // music_name
        rowData.append(query.value(3).toString());  // music_compose
        rowData.append(QString::number(query.value(4).toInt()));  // time_long
        rowData.append(query.value(5).toBool() ? "1" : "0");  // is_liked (存储为字符串)

        likeData.append(rowData);  // 将当前行的数据添加到所有记录中
    }
    return likeData;  // 返回所有数据
}

QVector<QString> DatabaseManager::getThisData(const QString &music_name)
{
    QVector<QString> thisData;
    QSqlQuery query(db); // 明确指定使用 db 连接
    // 准备 SQL 语句并绑定参数
    if (!query.prepare("SELECT * FROM my_table WHERE music_name = ?")) {
        qDebug() << "Failed to prepare query getThisData:" << query.lastError();
        return thisData;
    }

    query.bindValue(0, music_name);

    // 执行查询
    if (!query.exec()) {
        qDebug() << "Error executing query getThisData:" << query.lastError();
        return thisData;
    }
    if (query.next()) {
        thisData.append(query.value(0).toString());  // id
        thisData.append(query.value(1).toString());  // img_name
        thisData.append(query.value(2).toString());  // music_name
        thisData.append(query.value(3).toString());  // music_compose
        thisData.append(QString::number(query.value(4).toInt()));  // time_long
        thisData.append(query.value(5).toBool() ? "1" : "0");  // is_liked (存储为字符串)
    } else {
        qDebug() << "No data found.";
    }
    return thisData;
}

QVector<QString> DatabaseManager::getFirstRowData()
{

    QVector<QString> firstRowData;  // 用于存储第一行的数据

    QSqlQuery query(db); // 明确指定使用 db 连接
    if (!query.exec("SELECT * FROM my_table LIMIT 1")) {
        qDebug() << "Error executing query:" << query.lastError();
        return firstRowData;
    }

    if (query.next()) {
        firstRowData.append(query.value(0).toString());  // id
        firstRowData.append(query.value(1).toString());  // img_name
        firstRowData.append(query.value(2).toString());  // music_name
        firstRowData.append(query.value(3).toString());  // music_compose
        firstRowData.append(QString::number(query.value(4).toInt()));  // time_long
        firstRowData.append(query.value(5).toBool() ? "1" : "0");  // is_liked (存储为字符串)
    } else {
        qDebug() << "No data found.";
    }

    return firstRowData;  // 返回第一行数据
}

QVector<QString> DatabaseManager::getFirstLikeRowData()
{
    QVector<QString> firstLikeRowData;  // 用于存储第一行的数据

    QSqlQuery query(db); // 明确指定使用 db 连接
    if (!query.exec("SELECT * FROM my_table WHERE is_liked = 1 LIMIT 1")) {
        qDebug() << "Error executing query:" << query.lastError();
        return firstLikeRowData;
    }

    if (query.next()) {
        firstLikeRowData.append(query.value(0).toString());  // id
        firstLikeRowData.append(query.value(1).toString());  // img_name
        firstLikeRowData.append(query.value(2).toString());  // music_name
        firstLikeRowData.append(query.value(3).toString());  // music_compose
        firstLikeRowData.append(QString::number(query.value(4).toInt()));  // time_long
        firstLikeRowData.append(query.value(5).toBool() ? "1" : "0");  // is_liked (存储为字符串)
    } else {
        qDebug() << "No data found.";
    }

    return firstLikeRowData;  // 返回第一行数据
}

QVector<QString> DatabaseManager::getNextMusicData(const QString &music_name)
{
    QVector<QString> nextRowData;  // 用于存储下一行的数据

    QSqlQuery query(db); // 明确指定使用 db 连接

    // 使用参数化查询防止 SQL 注入
    QString sql = "WITH CurrentItem AS (SELECT id FROM my_table WHERE music_name = ?), "
                  "NextItem AS (SELECT * FROM my_table WHERE id > (SELECT id FROM CurrentItem) ORDER BY id ASC LIMIT 1), "
                  "FirstItem AS (SELECT * FROM my_table ORDER BY id ASC LIMIT 1) "
                  "SELECT * FROM NextItem UNION ALL SELECT * FROM FirstItem WHERE NOT EXISTS (SELECT 1 FROM NextItem)";

    if (!query.prepare(sql)) {
        qDebug() << "Error preparing query:" << query.lastError();
        return nextRowData;
    }

    // 绑定参数以防止 SQL 注入
    query.bindValue(0, music_name);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError();
        return nextRowData;
    }

    if (query.next()) {
        nextRowData.append(query.value(0).toString());  // id
        nextRowData.append(query.value(1).toString());  // img_name
        nextRowData.append(query.value(2).toString());  // music_name
        nextRowData.append(query.value(3).toString());  // music_compose
        nextRowData.append(QString::number(query.value(4).toInt()));  // time_long
        nextRowData.append(query.value(5).toBool() ? "1" : "0");  // is_liked (存储为字符串)
    } else {
        qDebug() << "No next data found.";
    }

    return nextRowData;  // 返回下一行数据
}


QVector<QString> DatabaseManager::getNextLikeMusicData(const QString &music_name)
{
    QVector<QString> nextRowData;  // 用于存储下一行的数据

    QSqlQuery query(db); // 明确指定使用 db 连接

    // 使用参数化查询防止 SQL 注入
    QString sql = "WITH CurrentItem AS (SELECT id FROM my_table WHERE music_name = ? AND is_liked = 1), "
                  "NextItem AS (SELECT * FROM my_table WHERE id > (SELECT id FROM CurrentItem) AND is_liked = 1 ORDER BY id ASC LIMIT 1), "
                  "FirstItem AS (SELECT * FROM my_table WHERE is_liked = 1 ORDER BY id ASC LIMIT 1) "
                  "SELECT * FROM NextItem UNION ALL SELECT * FROM FirstItem WHERE NOT EXISTS (SELECT 1 FROM NextItem)";

    if (!query.prepare(sql)) {
        qDebug() << "Error preparing query:" << query.lastError();
        return nextRowData;
    }

    // 绑定参数以防止 SQL 注入
    query.bindValue(0, music_name);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError();
        return nextRowData;
    }

    if (query.next()) {
        // 假设列的顺序为 id, img_name, music_name, music_compose, time_long, is_liked
        nextRowData.append(query.value(0).toString());  // id
        nextRowData.append(query.value(1).toString());  // img_name
        nextRowData.append(query.value(2).toString());  // music_name
        nextRowData.append(query.value(3).toString());  // music_compose
        nextRowData.append(QString::number(query.value(4).toInt()));  // time_long
        nextRowData.append(query.value(5).toBool() ? "1" : "0");  // is_liked (存储为字符串)
    } else {
        qDebug() << "No next liked data found.";
    }

    return nextRowData;  // 返回满足条件的下一行数据
}

QVector<QString> DatabaseManager::getLastMusicData(const QString &music_name)
{
    QVector<QString> lastRowData;  // 用于存储上一行的数据

    QSqlQuery query(db); // 明确指定使用 db 连接

    // 使用参数化查询防止 SQL 注入
    QString sql = "WITH CurrentItem AS (SELECT id FROM my_table WHERE music_name = ?), "
                  "LastItem AS (SELECT * FROM my_table WHERE id < (SELECT id FROM CurrentItem) ORDER BY id DESC LIMIT 1), "
                  "EndItem AS (SELECT * FROM my_table ORDER BY id DESC LIMIT 1) "
                  "SELECT * FROM LastItem UNION ALL SELECT * FROM EndItem WHERE NOT EXISTS (SELECT 1 FROM LastItem)";

    if (!query.prepare(sql)) {
        qDebug() << "Error preparing query:" << query.lastError();
        return lastRowData;
    }

    // 绑定参数以防止 SQL 注入
    query.bindValue(0, music_name);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError();
        return lastRowData;
    }

    if (query.next()) {
        // 假设列的顺序为 id, img_name, music_name, music_compose, time_long, is_liked
        lastRowData.append(query.value(0).toString());  // id
        lastRowData.append(query.value(1).toString());  // img_name
        lastRowData.append(query.value(2).toString());  // music_name
        lastRowData.append(query.value(3).toString());  // music_compose
        lastRowData.append(QString::number(query.value(4).toInt()));  // time_long
        lastRowData.append(query.value(5).toBool() ? "1" : "0");  // is_liked (存储为字符串)
    } else {
        qDebug() << "No next liked data found.";
    }

    return lastRowData;  // 返回满足条件的下一行数据
}

QVector<QString> DatabaseManager::getLastLikeMusicData(const QString &music_name)
{
    QVector<QString> lastRowData;  // 用于存储上一行的数据

    QSqlQuery query(db); // 明确指定使用 db 连接

    // 使用参数化查询防止 SQL 注入
    QString sql = "WITH CurrentItem AS (SELECT id FROM my_table WHERE music_name = ? AND is_liked = 1), "
                  "LastItem AS (SELECT * FROM my_table WHERE id < (SELECT id FROM CurrentItem) AND is_liked = 1 ORDER BY id DESC LIMIT 1), "
                  "EndItem AS (SELECT * FROM my_table WHERE is_liked = 1 ORDER BY id DESC LIMIT 1)"
                  "SELECT * FROM LastItem UNION ALL SELECT * FROM EndItem WHERE NOT EXISTS (SELECT 1 FROM LastItem)";

    if (!query.prepare(sql)) {
        qDebug() << "Error preparing query:" << query.lastError();
        return lastRowData;
    }

    // 绑定参数以防止 SQL 注入
    query.bindValue(0, music_name);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError();
        return lastRowData;
    }

    if (query.next()) {
        // 假设列的顺序为 id, img_name, music_name, music_compose, time_long, is_liked
        lastRowData.append(query.value(0).toString());  // id
        lastRowData.append(query.value(1).toString());  // img_name
        lastRowData.append(query.value(2).toString());  // music_name
        lastRowData.append(query.value(3).toString());  // music_compose
        lastRowData.append(QString::number(query.value(4).toInt()));  // time_long
        lastRowData.append(query.value(5).toBool() ? "1" : "0");  // is_liked (存储为字符串)
    } else {
        qDebug() << "No next liked data found.";
    }

    return lastRowData;  // 返回满足条件的下一行数据
}



//最近播放
//
//
//

bool DatabaseManager::createRecentlyTable()
{
    QSqlQuery query(db); // 明确指定使用 db 连接
    if (!query.exec("CREATE TABLE IF NOT EXISTS recently_music_table ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "music_name TEXT, "
                    "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP)")) {
        qDebug() << "Create table failed:" << query.lastError();
        return false;
    } else {
        qDebug() << "创建成功";
        return true;
    }
}

void DatabaseManager::recentlyData()
{
    if (!db.isOpen()) {
        qDebug() << "Here is queryData" << "Error: Database is not open.";
        return;
    }

    QSqlQuery query(db); // 明确指定使用 db 连接
    if (!query.exec("SELECT * FROM recently_music_table")) {
        qDebug() << "Error executing query:" << query.lastError();
        return;
    }

    while (query.next()) {
        QString music_name = query.value(1).toString();
        qDebug() <<"music_name"<<music_name;
    }
}

bool DatabaseManager::saveRecentlyMusic(const QString &music_name)
{
    QSqlQuery query(db);

    // 使用参数绑定来防止 SQL 注入
    if (!query.prepare("DELETE FROM recently_music_table WHERE music_name = ?")) {
        qDebug() << "Failed to prepare DELETE statement:" << query.lastError();
        return false;
    }
    query.bindValue(0, music_name);

    if (!query.exec()) {
        qDebug() << "Delete failed:" << query.lastError();
        return false;
    }

    // 插入新记录
    if (!query.prepare("INSERT INTO recently_music_table (music_name) VALUES (?)")) {
        qDebug() << "Failed to prepare INSERT statement:" << query.lastError();
        return false;
    }
    query.bindValue(0, music_name);
    if (!query.exec()) {
        qDebug() << "Insert failed:" << query.lastError() << "Bound values:" << query.boundValues(); // 输出详细错误信息
        return false;
    }

    return true;
}

QVector<QString> DatabaseManager::getRecentlyThisData(const QString &music_name)
{
    QVector<QString> thisData;
    QSqlQuery query(db); // 明确指定使用 db 连接
    if(!query.exec("SELECT * FROM recently_music_table WHERE music_name = ?")){
        qDebug() << "Error executing query getThisData:" << query.lastError();
        return thisData;
    }
    query.bindValue(0,music_name);
    if (query.next()) {
        thisData.append(query.value(0).toString());  // id
        thisData.append(query.value(1).toString());  // music_name
        thisData.append(query.value(2).toString());  // timestamp
    } else {
        qDebug() << "No data found.";
    }
    return thisData;
}

QVector<QString> DatabaseManager::getRecentlyMusic()
{
    QVector<QString> recentlyPlayedMusic;
    QSqlQuery query(db);

    if (query.exec("SELECT music_name FROM recently_music_table ORDER BY timestamp DESC")) {
        while (query.next()) {
            recentlyPlayedMusic.append(query.value(0).toString()); //music_name
        }
    } else {
        qDebug() << "Query failed:" << query.lastError();
    }

    return recentlyPlayedMusic;
}


