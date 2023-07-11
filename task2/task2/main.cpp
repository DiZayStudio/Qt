#include <QCoreApplication>
#include <QTcpServer>
#include <QSqlQuery>

QTcpServer *tcpServer;
QSqlQuery *sql;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    tcpServer = new QTcpServer(nullptr);

    sql = new QSqlQuery();

    return a.exec();
}
