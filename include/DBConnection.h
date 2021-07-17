//
// Created by 86191 on 2021/6/2.
//

#ifndef SERVER_DBCONNECTION_H
#define SERVER_DBCONNECTION_H
#include <iostream>
#include <mysql.h>

using namespace std;
class DBConnection {
public:
    DBConnection();
    ~DBConnection();
    bool ConnectDataBase();
    bool QueryDatabase(string sql);//查询并输出结果
    void outMysql();//输出查询结果
    bool updateData(string sql);//插入删除修改操作
    void FreeConnect();//释放资源
    bool InsertDataBase(string sql);

    MYSQL_RES *getMysqlRes(){return res;}
    MYSQL* getMysql(){return &mysql;}

private:
    MYSQL mysql;
    MYSQL_RES *res;//这个结构代表返回行的一个查询结果集
    MYSQL_ROW column;//一个行数据的类型安全(type-safe)的表示
};


#endif //SERVER_DBCONNECTION_H
