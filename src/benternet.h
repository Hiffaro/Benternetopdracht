#ifndef BENTERNET_H_
#define BENTERNET_H_

#include <QObject>
#include <nzmqt/nzmqt.hpp>

class Benternet : public QObject {
    Q_OBJECT

public:
    Benternet(QObject *parent = nullptr);
    Benternet(nzmqt::ZMQContext *ctx);
    nzmqt::ZMQContext* getContext();
    QByteArray receive();

signals:
    void onReceive(const QByteArray &message);

public slots:
    void start();
    bool send(const char *message);
    bool send(const QString &message);
    bool send(const QByteArray &message);
    bool send(nzmqt::ZMQMessage &message);
    void subscribe(const char *topic);
    void subscribe(const QString &topic);
    void subscribe(const QByteArray &topic);
    void unsubscribe(const char *topic);
    void unsubscribe(const QString &topic);
    void unsubscribe(const QByteArray &topic);

private slots:
    void messageReceived(const QList<QByteArray> &messages);

private:
    void doConnection();

    nzmqt::ZMQContext *mCtx;
    nzmqt::ZMQSocket *mPusher;
    nzmqt::ZMQSocket *mSubscriber;
};

#endif
