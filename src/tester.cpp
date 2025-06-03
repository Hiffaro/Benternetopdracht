#include "benternet.h"
#include <cstdlib>
#include <nzmqt/nzmqt.hpp>
#include <zmq.h>
#include <QtCore>
#include <QtNetwork>

int ctr = 0;

QByteArray generateRequest(QRandomGenerator *rng) {
    bool addModifier = rng->bounded(2) == 1;
    int addedModifier = rng->bounded(31) - 15;
    int addedAdvDis = rng->bounded(3);
    bool superAdvDis = rng->bounded(2) == 1;
    bool addDc = rng->bounded(2) == 1;
    bool addIdent = rng->bounded(2) == 1;
    int addedDc = rng->bounded(31);
    int diceCount = rng->bounded(5) + 1;
    QList<int> faces = {2, 3, 4, 6, 8, 10, 12, 20, 100};
    int diceFaces = faces[rng->bounded(faces.size())];

    QByteArray toSend = addDc ? QByteArray("dice>rollvs>") : QByteArray("dice>roll>");
    if(addIdent) toSend += (QUuid::createUuid().toString(QUuid::WithoutBraces) + ">").toUtf8();
    if(diceCount != 1) toSend += QByteArray::number(diceCount);
    toSend += QByteArray("d");
    toSend += QByteArray::number(diceFaces);
    if(addModifier) {
        toSend += addedModifier < 0 ? QByteArray("-") : QByteArray("+");
        toSend += QByteArray::number(std::abs(addedModifier));
    }
    if(addedAdvDis == 1) toSend += QByteArray("adv");
    if(addedAdvDis == 2) toSend += QByteArray("dis");
    if(addedAdvDis != 0 && superAdvDis) toSend += QByteArray("+");
    if(addDc) {
        toSend += QByteArray(">");
        toSend += QByteArray::number(addedDc);
    }
    return toSend;
}

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);
    QRandomGenerator *rng = QRandomGenerator::global();

    // Setup benternet.
    qDebug() << "Starting benternet connection...";
    QSharedPointer benternet = QSharedPointer<Benternet>(new Benternet(&a));
    benternet->start();

    // Setup task forwarding.
    qDebug() << "Setting up response parsing...";
    QObject::connect(benternet.data(), &Benternet::onReceive, [&](const QByteArray &message) {
        qDebug().noquote() << "Result:" << QString(message);
        if(ctr % 2 == 0) {
            QTimer::singleShot((rng->bounded(50) + 1) * 100, [&]() {
                auto toSend = generateRequest(rng);
                qDebug() << "Sending request:" << QString(toSend);
                benternet->send(toSend);
                benternet->send("dice>history");
            });
        }
        ctr++;
        ctr %= 2;
    });

    // Subscribe to our endpoints.
    qDebug() << "Subscribing to endpoint...";
    benternet->subscribe("dice>result>");

    qDebug() << "Starting event loop...";
    auto toSend = generateRequest(rng);
    qDebug() << "Sending request:" << QString(toSend);
    benternet->send(toSend);
    return a.exec();
}
