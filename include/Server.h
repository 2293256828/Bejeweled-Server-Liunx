//
// Created by 86191 on 2021/6/2.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include <cstring>
#include "DBConnection.h"

class Server {
public:
   void ServerSocket();
    Server(){
        ID = new string();
        password = new string();
        dbConnection = new DBConnection();
    }
    

private:
    string *ID;
    string *password;
    DBConnection *dbConnection;
    MYSQL *mySql;

    void setMysql(){mySql = dbConnection->getMysql();}

    void registerClient(int clntSock,string func);
    void ScoreClient(int clntSock);
    void RankClient(int clntSock);

    bool loginDB();
    bool registerDB();
    bool scoreDB(int score,int mode);
    bool rankDB();

};

#endif //SERVER_SERVER_H
