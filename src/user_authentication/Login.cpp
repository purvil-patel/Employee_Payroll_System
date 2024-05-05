#include "Login.h"
#include <iostream>

Login::Login(sqlite3 *db) : db(db){
    // Create the users table if it does not exist
    int rc1 = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT);", nullptr, 0, &zErrMsg);
    
    if (rc1 != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Table 'user' checked/created successfully.\n";
    }
    // Insert default admin user if not exists
    int rc2 = sqlite3_exec(db, "INSERT OR IGNORE INTO users (username, password) VALUES ('admin', 'password123');", nullptr, 0, &zErrMsg);
    
    if (rc2 != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Table 'user' inserted successfully.\n";
    }
}


bool Login::authenticate(const std::string& username, const std::string& password) {
    std::string sql = "SELECT * FROM users WHERE username='" + username + "' AND password='" + password + "';";
    bool loginSuccess = false;
    
    rc = sqlite3_exec(db, sql.c_str(), [](void* data, int argc, char** argv, char** azColName) -> int {
        *(bool*)data = true;
        return 0;
    }, &loginSuccess, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    return loginSuccess;
}
