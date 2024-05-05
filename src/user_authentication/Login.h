#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include <sqlite3.h>

class Login {
private:
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

public:
    Login(sqlite3 *db);
    bool authenticate(const std::string& username, const std::string& password);
};

#endif // LOGIN_H
