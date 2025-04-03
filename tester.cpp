#include "benternet.h"
#include <nzmqt/nzmqt.hpp>
#include <zmq.h>
#include <QtCore>
#include <QtNetwork>

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);
    QRandomGenerator rng = QRandomGenerator::securelySeeded();

    // Setup benternet.
    qDebug() << "Starting benternet connection...";
    QSharedPointer benternet = QSharedPointer<Benternet>(new Benternet(&a));
    benternet->start();

    // Setup task forwarding.
    qDebug() << "Setting up response parsing...";
    QObject::connect(benternet.data(), &Benternet::onReceive, [&](const QByteArray &message) {
        QString rollRequest = QString(message.split('>').last());
        qDebug() << "Result:" << rollRequest;
        QTimer::singleShot((rng.bounded(50) + 1) * 100, [&]() {
            QByteArray toSend = QByteArray("dice>roll>") + QByteArray::number(rng.bounded(5) + 1) + QByteArray("d") + QByteArray::number(rng.bounded(100) + 1);
            qDebug() << "Sending request:" << QString(toSend);
            benternet->send(toSend);
        });
    });

    // Subscribe to our endpoints.
    qDebug() << "Subscribing to endpoint...";
    benternet->subscribe("dice>result>");

    qDebug() << "Starting event loop...";
    QByteArray toSend = QByteArray("dice>roll>") + QByteArray::number(rng.bounded(5) + 1) + QByteArray("d") + QByteArray::number(rng.bounded(100) + 1);
    qDebug() << "Sending request:" << QString(toSend);
    benternet->send(toSend);
    return a.exec();
}
