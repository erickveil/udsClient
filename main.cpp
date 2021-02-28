#include <QCoreApplication>

#include "localsocketclient.h"
#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream output(stdout);

    auto parseCb = [&] (QByteArray msg) {
        output << msg << "\n";
        output.flush();
    };

    LocalSocketClient client;
    client.init("b_socket", parseCb);
    client.send("Hello World.");

    return a.exec();
}
