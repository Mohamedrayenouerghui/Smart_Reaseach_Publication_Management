// connection.h
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QObject>

class Connection : public QObject
{
    Q_OBJECT

public:
    static Connection& createInstance();   // Singleton access
    bool createConnection();
    QSqlDatabase& getDb();                 // Returns the named connection

private:
    explicit Connection(QObject *parent = nullptr);
    ~Connection() = default;

    QSqlDatabase db;
    static Connection* instance;

    // Disable copy
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
};

#endif // CONNECTION_H
