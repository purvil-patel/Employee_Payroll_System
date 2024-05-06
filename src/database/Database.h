#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

class Database {
public:
    static Database& getInstance() {
        static Database instance; // Guaranteed to be destroyed and instantiated on first use.
        return instance;
    }

    sqlite3* getConn() { return db; }

private:
    sqlite3 *db;
    Database() { sqlite3_open("payroll_system.db", &db); } // Constructor is private
    ~Database() { sqlite3_close(db); }
    Database(Database const&) = delete; // Prevent copies
    void operator=(Database const&) = delete; // Prevent assignments
};

#endif
