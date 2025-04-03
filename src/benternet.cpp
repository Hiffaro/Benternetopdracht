#include "benternet.h"
#include <iostream>
#include <cstdlib>
#include <nzmqt/nzmqt.hpp>

Benternet::Benternet(QObject *parent) : QObject(parent) {
    mCtx = nzmqt::createDefaultContext(this);
    doConnection();
}

Benternet::Benternet(nzmqt::ZMQContext *ctx) : QObject(ctx), mCtx(ctx) {
    doConnection();
}

nzmqt::ZMQContext* Benternet::getContext() {
    return mCtx;
}


QByteArray Benternet::receive() {
    return mSubscriber->receiveMessage((nzmqt::ZMQSocket::ReceiveFlags)0).first();
}

void Benternet::start() {
    mCtx->start();
    if(!mPusher->isConnected() || !mSubscriber->isConnected()) {
        std::cerr << "Connection failure." << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool Benternet::send(const char *message) {
    return mPusher->sendMessage(QString( message ).toUtf8());
}

bool Benternet::send(const QString &message) {
    return mPusher->sendMessage(message.toUtf8());
}

bool Benternet::send(const QByteArray &message) {
    return mPusher->sendMessage(message);
}

bool Benternet::send(nzmqt::ZMQMessage &message) {
    return mPusher->sendMessage(message);
}

void Benternet::subscribe(const char *topic) {
    mSubscriber->subscribeTo(topic);
}

void Benternet::subscribe(const QString &topic) {
    mSubscriber->subscribeTo(topic);
}

void Benternet::subscribe(const QByteArray &topic) {
    mSubscriber->subscribeTo(topic);
}

void Benternet::unsubscribe(const char *topic) {
    mSubscriber->unsubscribeFrom(topic);
}

void Benternet::unsubscribe(const QString &topic) {
    mSubscriber->unsubscribeFrom(topic);
}

void Benternet::unsubscribe(const QByteArray &topic) {
    mSubscriber->unsubscribeFrom(topic);
}



void Benternet::messageReceived(const QList<QByteArray> &messages) {
    for(const QByteArray &message : messages) {
        emit(onReceive(message));
    }
}


void Benternet::doConnection() {
    mPusher = mCtx->createSocket(nzmqt::ZMQSocket::TYP_PUSH, mCtx);
    mSubscriber = mCtx->createSocket(nzmqt::ZMQSocket::TYP_SUB, mCtx);
    mPusher->connectTo("tcp://benternet.pxl-ea-ict.be:24041");
    mSubscriber->connectTo("tcp://benternet.pxl-ea-ict.be:24042");
    QObject::connect(mSubscriber, &nzmqt::ZMQSocket::messageReceived, this, &Benternet::messageReceived);
}
