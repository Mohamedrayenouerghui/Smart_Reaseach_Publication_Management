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
    void setCredentials(const QString &host, const QString &uid, const QString &pwd);
    QSqlDatabase& getDb();                 // Fixed: consistent naming
    static QSqlDatabase getDatabase();

private:
    explicit Connection(QObject *parent = nullptr);
    ~Connection() = default;

    QSqlDatabase db;
    static Connection* instance;
    QString m_host = "localhost:1521/XE";
    QString m_uid  = "MARWA";
    QString m_pwd  = "1234";

    // Disable copy
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
};

#endif // CONNECTION_H
