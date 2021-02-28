/* localsocketclient.h
 * Erick Veil
 * 2021-02-28
 * Copyright 2021 Erick Veil
 */
#ifndef LOCALSOCKETCLIENT_H
#define LOCALSOCKETCLIENT_H

#include <functional>

#include <QByteArray>
#include <QLocalSocket>
#include <QObject>
#include <QString>
#include <QTextStream>

/**
 * @brief The LocalSocketClient class
 * This is a basic local socket class.
 * Allows you to set callbacks instead of signals and slots.
 * Just define the Attributes, call init, and send your message.
 */
class LocalSocketClient : public QObject
{
    Q_OBJECT

    QLocalSocket _client;

    bool _isSignalsConnected = false;

    QByteArray _responseBuffer;

public:

    /**
     * @brief Name
     * This is the name/id of the local domain socket. The listener it's
     * talking to should have the same name.
     * Default is "b_socket" if not defined.
     */
    QString Name;

    /**
     * @brief ParseResponse
     * If defined, this will be run when the server sends back its response.
     */
    std::function<void (QByteArray)> ParseResponse;

    explicit LocalSocketClient(QObject *parent = nullptr);

    /**
     * @brief init
     * Initializes the class with whatever attributes have been set (or the
     * defaults if they haven't been set).
     * Connects all the signals and slots the first time it's run.
     * Can be called again after changing attributes.
     * Must be called before using this class.
     */
    void init();

    /**
     * @brief init
     * @param name
     * @param parseResponse
     * Overloaded method sets the attributes to the provided values, then calls
     * it's partner, `init()`.
     */
    void init(QString name, std::function<void (QByteArray)> parseResponse);

    /**
     * @brief send
     * Sends msg to the listener.
     * Expects an ack.
     * You should call init at least once before calling this method.
     * @param msg
     */
    void send(QByteArray msg);

signals:

public slots:
    // QLocalSocket
    void eventConnected();
    void eventDisconnected();
    void eventErrorOccurred(QLocalSocket::LocalSocketError errnum);
    void eventStateChanged(QLocalSocket::LocalSocketState state);

    // QIODevice
    void eventIODeviceAboutToClose();
    void eventIODeviceBytesWritten(qint64 bytes);
    void eventIODeviceChannelBytesWritten(int channel, qint64 bytes);
    void eventIODeviceChannelReadyRead(int channel);
    void eventIODeviceReadChannelFinished();
    void eventIODeviceReadyRead();

private:
};


#endif // LOCALSOCKETCLIENT_H
