#include "benternet.h"
#include <nzmqt/nzmqt.hpp>
#include <zmq.h>
#include <QtCore>
#include <QtNetwork>

QByteArray doRoll(QString rollRequest) {
    static QRandomGenerator rng = QRandomGenerator::securelySeeded();
    try {
        qDebug() << "Parsing roll request:" << rollRequest;
        auto elements = rollRequest.split('d');
        if(elements.length() == 2) {
            bool okAmount = true;
            auto amount = elements[0].toUInt(&okAmount);
            bool okDSize = true;
            auto dSize = elements[1].toUInt(&okDSize);
            if(okAmount && okDSize) {
                int total = 0;
                for(int i = 0; i < amount; i++) total += rng.bounded(dSize) + 1;
                qDebug() << "Emitting:" << total;
                return QByteArray("dice>result>") + QByteArray::number(total);
            } else {
                qDebug() << "Invalid parameters.";
                return QByteArray("dice>result>Error: DiceAmount and DiceSize must be positive integers.");
            }
        } else {
            qDebug() << "Wrong format.";
            return QByteArray("dice>result>Error: Expected format is [DiceAmount]d[DiceSize]. E.g.: 1d20.");
        }
    } catch(nzmqt::ZMQException &e) {
        qDebug() << "ZMQException: " << e.what();
        return QByteArray("dice>result>Internal service error.");
    }
}

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);

    // Setup benternet.
    qDebug() << "Starting benternet connection...";
    QSharedPointer benternet = QSharedPointer<Benternet>(new Benternet(&a));
    benternet->start();

    // Setup task forwarding.
    qDebug() << "Setting up message forwarding...";
    QObject::connect(benternet.data(), &Benternet::onReceive, [&](const QByteArray &message) {
        QString rollRequest = QString(message.split('>').last());
        benternet->send(doRoll(rollRequest));
    });

    // Subscribe to our endpoints.
    qDebug() << "Subscribing to endpoint...";
    benternet->subscribe("dice>roll>");

    qDebug() << "Starting event loop...";
    return a.exec();
}
