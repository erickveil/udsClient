#include "localsocketclient.h"

LocalSocketClient::LocalSocketClient(QObject *parent) : QObject(parent)
{

}

void LocalSocketClient::init()
{
    if (Name == "") { Name = "b_socket"; }

    if (_isSignalsConnected) { return; }

    connect (&_client,
             SIGNAL(connected()),
             this,
             SLOT(eventConnected()));
    connect (&_client,
             SIGNAL(disconnected()),
             this,
             SLOT(eventDisconnected()));
    connect (&_client,
             QOverload<QLocalSocket::LocalSocketError>::
             of(&QLocalSocket::error),
             [=](QLocalSocket::LocalSocketError socketError) {
        eventErrorOccurred(socketError);
    });
    connect (&_client,
             SIGNAL(stateChanged(QLocalSocket::LocalSocketState)),
             this,
             SLOT(eventStateChanged(QLocalSocket::LocalSocketState)));
    connect (&_client,
             SIGNAL(bytesWritten(qint64)),
             this,
             SLOT(eventIODeviceBytesWritten(qint64)));
    connect (&_client,
             SIGNAL(channelBytesWritten(int,qint64)),
             this,
             SLOT(eventIODeviceChannelBytesWritten(int,qint64)));
    connect (&_client,
             SIGNAL(channelReadyRead(int)),
             this,
             SLOT(eventIODeviceChannelReadyRead(int)));
    connect (&_client,
             SIGNAL(readChannelFinished()),
             this,
             SLOT(eventIODeviceReadChannelFinished()));
    connect (&_client,
             SIGNAL(readyRead()),
             this,
             SLOT( eventIODeviceReadyRead() ));

    _isSignalsConnected = true;
}

void LocalSocketClient::init(QString name, std::function<void (QByteArray)> parseResponse)
{
    Name = name;
    ParseResponse = parseResponse;
    init();
}

void LocalSocketClient::send(QByteArray msg)
{
    if (!_isSignalsConnected) { init(); }
    _client.connectToServer(Name);
    _client.write(msg);
    _client.flush();
}

void LocalSocketClient::eventConnected()
{
}

void LocalSocketClient::eventDisconnected()
{
}

void LocalSocketClient::eventErrorOccurred(QLocalSocket::LocalSocketError errnum)
{
    // server is in charge of hang up after sending ack
    if (errnum == QLocalSocket::PeerClosedError) { return; }

    QTextStream errorOut( stderr );
    errorOut << "error: " << _client.errorString();
    errorOut.flush();
}

void LocalSocketClient::eventStateChanged(QLocalSocket::LocalSocketState state)
{

    switch (state) {
    case QLocalSocket::UnconnectedState:
        //
        break;
    case QLocalSocket::ConnectingState:
        //
        break;
    case QLocalSocket::ConnectedState:
        //
        break;
    case QLocalSocket::ClosingState:
        //
        break;
    default:
        // unknown state
        break;
    }
}

void LocalSocketClient::eventIODeviceAboutToClose()
{

}

void LocalSocketClient::eventIODeviceBytesWritten(qint64 bytes)
{
    Q_UNUSED(bytes);
}

void LocalSocketClient::eventIODeviceChannelBytesWritten(int channel, qint64 bytes)
{
    Q_UNUSED(channel);
    Q_UNUSED(bytes);
}

void LocalSocketClient::eventIODeviceChannelReadyRead(int channel)
{
    Q_UNUSED(channel);
}

void LocalSocketClient::eventIODeviceReadChannelFinished()
{
    _client.close();
    if (ParseResponse) { ParseResponse(_responseBuffer); }
}

void LocalSocketClient::eventIODeviceReadyRead()
{
    _responseBuffer = _client.readAll();
    _client.flush();
}

