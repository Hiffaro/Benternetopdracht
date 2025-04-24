#include "benternet.h"
#include <algorithm>
#include <nzmqt/nzmqt.hpp>
#include <zmq.h>
#include <QtCore>
#include <QtNetwork>

QByteArray doRollVs(QString result, QString dc, QString ident) {
    if(result.contains("Error")) {
        qDebug() << "Upstream error.";
        return result.toUtf8();
    }

    // Build return array.
    QByteArray ret = QByteArray("dice>result>");
    if(ident != "dice" && ident != "dicevs") ret += (ident + ">").toUtf8();

    // Parse DC and result.
    bool okVs = true;
    auto vs = dc.toInt(&okVs);
    bool okRes = true;
    auto res = QString(result.split('>').last()).toInt(&okRes);

    if(okVs && okRes) {
        qDebug() << "Emitting:" << res << "vs" << vs;
        return ret + QByteArray(res >= vs ? "pass" : "fail");
    } else {
        qDebug() << "Invalid DC.";
        return ret + QByteArray("Error: Invalid DC.");
    }
}

QByteArray doRoll(QString rollRequest, QString ident) {
    static QRandomGenerator rng = QRandomGenerator::securelySeeded();

    // Build return array.
    QByteArray ret = QByteArray("dice>result>");
    if(ident != "dice" && ident != "dicevs") ret += (ident + ">").toUtf8();

    // Parse request.
    qDebug() << "Parsing roll request:" << rollRequest;
    QRegularExpression re(R"-(^(?<amount>\d+){,1}d(?<faces>\d+)(?<modifier>[+-]\d+){,1}(?<advdis>(adv)|(dis)){,1}$)-", QRegularExpression::CaseInsensitiveOption);
    auto match = re.match(rollRequest);

    if(match.hasMatch()) {
        // Extract data from regex capture.
        uint diceAmount = match.captured("amount").toUInt();
        if(diceAmount == 0) diceAmount = 1;
        uint diceFaces = match.captured("faces").toUInt();
        bool keepHighest = match.captured("advdis") == "adv";
        bool keepLowest = match.captured("advdis") == "dis";
        int modifier = match.captured("modifier").toInt();

        // Calculate two rolls, starting at modifier.
        int total1 = modifier;
        int total2 = modifier;
        for(int i = 0; i < diceAmount; i++) total1 += rng.bounded(diceFaces) + 1;
        for(int i = 0; i < diceAmount; i++) total2 += rng.bounded(diceFaces) + 1;

        // Return the 1st, or the highest/lowest if that option was picked.
        int total = total1;
        if(keepHighest) total = std::max(total1, total2);
        if(keepLowest)  total = std::min(total1, total2);
        qDebug() << "Emitting:" << total;
        return ret + QByteArray::number(total);
    }  else {
        qDebug() << "Wrong format.";
        return ret + QByteArray("Error: Expected format is [DiceAmount]d[DiceSize](+/-[Modifier])([dis/adv]). E.g.: 1d20.");
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
        auto params = QString(message).split(">");
        if(params.size() >= 3) {
            QString mode = params[1];
            if(mode == "roll") {
                QString rollRequest = QString(params[params.length() - 1]);
                QString ident = QString(params[params.length() - 2]);
                QByteArray result = doRoll(rollRequest, ident);
                try {
                    benternet->send(result);
                } catch(nzmqt::ZMQException &e) {
                    qDebug() << "ZMQException: " << e.what();
                }
            } else if(mode == "rollvs") {
                QString rollRequest = QString(params[params.length() - 2]);
                QString dc = QString(params[params.length() - 1]);
                QString ident = QString(params[params.length() - 3]);
                QByteArray result = doRollVs(doRoll(rollRequest, ident), dc, ident);
                try {
                    benternet->send(result);
                } catch(nzmqt::ZMQException &e) {
                    qDebug() << "ZMQException: " << e.what();
                }
            }
        }
    });

    // Subscribe to our endpoints.
    qDebug() << "Subscribing to endpoint...";

    // Roll in the format dice>roll>(identifier>)XdY±Z[dis/adv]. Returns result.
    benternet->subscribe("dice>roll>");

    // Roll in the format dice>rollvs>(identifier>)XdY±Z[dis/adv]>DC. Returns true/false.
    benternet->subscribe("dice>rollvs>");

    qDebug() << "Starting event loop...";
    return a.exec();
}
