//
// Created by 86191 on 2021/6/2.
//

#include "Server.h"
/**
 * 接收端口访问，调用相关方法
 */
void Server::ServerSocket()
{

    //创建套接字
    int servSock = socket(AF_INET, SOCK_STREAM, 0);
    //绑定套接字
    struct sockaddr_in sockAddr;
    bzero(&sockAddr,sizeof (struct sockaddr_in));
    sockAddr.sin_family = AF_INET;  //使用IPv4地址
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);  //具体的IP地址
    sockAddr.sin_port = htons(9876);  //端口
    bind(servSock, (struct sockaddr*)&sockAddr, sizeof(struct sockaddr));

    //进入监听状态
    listen(servSock, 20);
    //接收客户端请求
    struct sockaddr clntAddr;
    int clntSock;

    while (true){
        unsigned int nSize = sizeof(struct sockaddr);
        clntSock = accept(servSock, (struct sockaddr*)&clntAddr, &nSize);
        string func;
        char szBuffer[0xff] = {0};
        recv(clntSock, szBuffer, 0xff, 0);
        func = szBuffer;
        cout << "Server:" << szBuffer << endl;
        if (func == "register" || func == "login"){
            registerClient(clntSock,func);
        }else{
            if (func == "score"){
                ScoreClient(clntSock);
            }else{
                if (func == "ranking"){
                    RankClient(clntSock);
                }
            }
        }
    }

}
/**
 *
 * @param clntSock 客户端的套接字
 * @param func string类型，是登录或者是注册
 */
void Server::registerClient(int clntSock,string func)
{
    //向客户端发送数据
    std::string s = "server accept";
    const char *str = s.c_str();
    send(clntSock, str, strlen(str)+sizeof(char), 0);

    char szBuffer1[0xff] = {0};
    recv(clntSock, szBuffer1, 0xff, 0);
    *ID = szBuffer1;
    cout << "register:SzBUffer:" << szBuffer1 <<endl;
    cout << "register:ID:" << *ID <<endl;

    s = "ID accept";
    str = s.c_str();
    send(clntSock, str, strlen(str)+sizeof(char), 0);

    char szBuffer2[0xff] = {0};
    recv(clntSock, szBuffer2, 0xff, 0);
    *password = szBuffer2;
    cout << "register:SzBUffer:" << szBuffer2 <<endl;
    cout << "register:password:" << *password <<endl;

    bool result = false;
    if (func == "register"){
        result = registerDB();
    }else{
        if (func == "login"){
            result = loginDB();
        }
    }

    if (result){
        s = "yes";
        str = s.c_str();
        send(clntSock, str, strlen(str)+sizeof(char), 0);
    }else{
        s = "no";
        str = s.c_str();
        send(clntSock, str, strlen(str)+sizeof(char), 0);
    }
}
/**
 *
 * @param clntSock 客户端的套接字
 */
void Server::ScoreClient(int clntSock)
{
    //向客户端发送数据
    std::string s = "server accept";
    const char *str = s.c_str();
    send(clntSock, str, strlen(str)+sizeof(char), 0);
    //接收ID
    char szBuffer[0xff] = {0};
    recv(clntSock, szBuffer, 0xff, 0);
    *ID = szBuffer;
    cout << "score:ID:" << *ID <<endl;

    s = "ID accept";
    str = s.c_str();
    send(clntSock, str, strlen(str)+sizeof(char), 0);
    //接收游戏模式
    string s_mode;
    char szBuffer1[0xff] = {0};
    recv(clntSock, szBuffer1, 0xff, 0);
    s_mode = szBuffer1;
    cout << "Socre:mode:" << szBuffer1 <<endl;

    s = "mode accept";
    str = s.c_str();
    send(clntSock, str, strlen(str)+sizeof(char), 0);
    //接收游戏分数
    string s_score;
    char szBuffer2[0xff] = {0};
    recv(clntSock, szBuffer2, 0xff, 0);
    s_score = szBuffer2;
    cout << "Score:Score:" << szBuffer2 << endl;
    bool result = false;
    result = scoreDB(stoi(s_score),stoi(s_mode));

    if (result){
        s = "yes";
        str = s.c_str();
        send(clntSock, str, strlen(str)+sizeof(char), 0);
    }else{
        s = "no";
        str = s.c_str();
        send(clntSock, str, strlen(str)+sizeof(char), 0);
    }
}

/**
 * 查看数据库记录中是否有该用户
 * @param s1 用户名
 * @param s2 密码
 * @return
 */
bool Server::loginDB()
{
    cout << "Register:" << *ID << endl;
    cout << "Register:" << *password << endl;
    string sql = "select * from register where ID = " + *ID + " and password = " + *password;
    bool result = dbConnection->QueryDatabase(sql);
    return result;
}

/**
 *将注册记录存储在数据库，包括用户名，密码并初始化当前用户分数为0
 * @param s1 用户名
 * @param s2 密码
 * @return 是否注册成功
 */
bool Server::registerDB()
{
    bool result = false;
    string sql = "select ID from register where ID = " + *ID;
    if(! dbConnection->QueryDatabase(sql)){
        sql = "insert into register values ( " + *ID +" , " + *password +" )";
        result = dbConnection->InsertDataBase(sql);
        sql = "insert into score values ( " + *ID + " , 0 , 0 )";
        result = dbConnection->InsertDataBase(sql);
    }
    return result;
}
/**
 * 如果当前分数比当前ID中对应游戏模式的最高分数高，就修改数据库中的分数记录
 * @param score 当前玩家的游戏分数
 * @return 是否修改
 */
bool Server::scoreDB(int score,int mode)
{
    bool result = false;
    string sql;
    if (mode == 0){
        sql = "update score set Time = " + to_string(score) + " where ID = " + *ID + " and Time <= " + to_string(score);
    }else{
        if (mode == 1){
            sql = "update score set Fast = " + to_string(score) + " where ID = " + *ID + " and Fast <= " + to_string(score);
        }
    }

    if(dbConnection->updateData(sql)){
        result = true;
//        sql = "select * from score order by Time desc";
//        dbConnection->QueryDatabase(sql);
        sql = "select * from score where ID = " + *ID;
        dbConnection->QueryDatabase(sql);
        setMysql();
        MYSQL_ROW column;
        column = mysql_fetch_row(dbConnection->getMysqlRes());
        cout << "Mode::Score:" << column[mode +1] << endl;
    }

    return result;
}
/**
 *
 * @param clntSock 客户端套接字
 */
void Server::RankClient(int clntSock)
{
    bool result = rankDB();
    MYSQL_ROW column;
    int n = mysql_affected_rows(dbConnection->getMysql()) > 8 ? 8:mysql_affected_rows(dbConnection->getMysql());

    //向客户端发送排行榜（记录数）
    string s = to_string(n);
    send(clntSock, s.c_str(), strlen(s.c_str())+sizeof(char), 0);

    char szBuffer[0xff] = {0};
    recv(clntSock, szBuffer, 0xff, 0);

    for (int i = 0;i < n;i++){
        if (column = mysql_fetch_row(dbConnection->getMysqlRes())){
            for (int j = 1;j < 3;j++){
                //向客户端发送分数
                string s;
                s = column[j];
                send(clntSock, s.c_str(), strlen(s.c_str())+sizeof(char), 0);
                char szBuffer[0xff] = {0};
                recv(clntSock, szBuffer, 0xff, 0);
            }
        }
    }
}
/**
 * 查询数据库中的所有分数记录
 * @return
 */
bool Server::rankDB()
{
    string sql = "select * from score order by Time desc";
    bool result = dbConnection->QueryDatabase(sql);
    return result;
}