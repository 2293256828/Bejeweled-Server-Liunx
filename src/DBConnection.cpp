//
// Created by 86191 on 2021/6/2.
//

#include "DBConnection.h"
DBConnection::DBConnection()
{
    ConnectDataBase();
}
DBConnection::~DBConnection()
{
    FreeConnect();
}
bool DBConnection::ConnectDataBase()
{
    mysql_init(&mysql);//初始化mysql
    if (!(mysql_real_connect(&mysql,
                             "localhost",//主机
                             "root",//用户名
                             "Ab135799@",//密码
                             "base",//数据库名 必须是已经存在的 我的mysql大小写不敏感
                             3306,//端口号 默认的0或者3306
                             NULL, 0//最后两个参数的常用写法 几乎都是这两个
    ))) {
        cout << "Error connecting to database:" + (string) mysql_error(&mysql) << endl;
        return false;
    } else {
        cout << "Connected..." << endl;
        return true;
    }
}
/**
 * //查询并输出结果
 * @param sql  操作数据库的语句
 * @return 是否查询成功
 */
bool DBConnection::QueryDatabase(string sql)
{
    mysql_query(&mysql, "set names gbk");//设置编码格式 否则cmd下中文乱码
    // 执行SQL语句
    // 0 执行成功
    // 1 执行失败
    if (mysql_query(&mysql, sql.c_str())) {//mysql_query第二个参数只接受const cahr* 需要将string类型转化
        cout << "Query failed ( " + (string) mysql_error(&mysql) + " )" << endl;
        return false;
    } else {
        cout << "Query success" << endl;
    }

    //获得结果集 MYSQL_RES *res;
    if (!(res = mysql_store_result(&mysql))) {
        cout << "Couldn't get result from " + (string) mysql_error(&mysql) << endl;
        return false;
    }
   // outMysql();//打印结果
    if (mysql_affected_rows(&mysql) == 0){
        return false;
    }
    return true;
}
/**
 *
 * @return
 */
bool DBConnection::InsertDataBase(string sql)
{
    mysql_query(&mysql, "set names gbk");//设置编码格式 否则cmd下中文乱码
    // 执行SQL语句
    // 0 执行成功
    // 1 执行失败
    if (mysql_query(&mysql, sql.c_str())) {//mysql_query第二个参数只接受const cahr* 需要将string类型转化
        cout << "Insert failed ( " + (string) mysql_error(&mysql) + " )" << endl;
        return false;
    } else {
        cout << "Insert success" << endl;
    }
    return true;
}

///**
// * 输出查询结果
// */
//void DBConnection::outMysql()
//{
//    //打印数据行数
//    cout << "number of dataline returned: " << mysql_affected_rows(&mysql) << endl;
//
//    char *field[32];//字段名
//
//    int num = mysql_num_fields(res);//获取列数
//    for (int i = 0; i < num; ++i) {//获取字段名
//        field[i] = mysql_fetch_field(res)->name;
//    }
//
//    for (int i = 0; i < num; ++i) {
//        cout << (string) field[i] << "    ";
//    }
//    cout << endl;
//
//    while (column = mysql_fetch_row(res)) {//获取一行数据
//        for (int i = 0; i < num; ++i) {
//            cout << column[i] << "    ";
//        }
//        cout << endl;
//    }
//}

/**
 * 插入删除修改操作
 * @param sql
 * @return
 */
bool DBConnection::updateData(string sql)
{
    mysql_query(&mysql, "set names utf8");//设置编码格式 否则mysql里中文乱码
    // 执行SQL语句
    // 0 执行成功
    // 1 执行失败
    if (mysql_query(&mysql, sql.c_str())) {//mysql_query第二个参数只接受const cahr* 需要将string类型转化
        cout << "Update failed ( " + (string) mysql_error(&mysql) + " )" << endl;
        return false;
    } else {
        cout << "Update success..." << endl;
        return true;
    }
}
/**
 * 释放资源
 */
void DBConnection::FreeConnect()
{
    mysql_free_result(res);
    mysql_close(&mysql);
}
