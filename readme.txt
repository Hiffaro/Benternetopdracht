Dice Roller Service - User Manual
Overview

The Dice Roller is a network service that allows you to perform virtual dice rolls. This manual explains how to use the service as a client.
Getting Started
Prerequisites

    Qt framework installed
    Benternet library configured in your project
    Basic knowledge of C++ programming

Using the Dice Roller Service
Step 1: Set Up Your Connection

First, initialize your Benternet connection:

QCoreApplication app(argc, argv);
QSharedPointer benternet = QSharedPointer<Benternet>(new Benternet(&app));
benternet->start();


Step 2: Subscribe to Results

Subscribe to the results channel to receive responses:

benternet->subscribe("dice>result>");


Step 3: Create a Response Handler

Set up a handler to process the dice roll results:

QObject::connect(benternet.data(), &Benternet::onReceive, [](const QByteArray &message) {
    QString result = QString(message.split('>').last());
    qDebug() << "Roll result:" << result;
    // Process your result here
});


Step 4: Send Roll Requests

Send your dice roll requests using the following format:

// Format: dice>roll>NdM
// Where N = number of dice, M = number of sides

// Example: Roll 1d20 (one 20-sided die)
benternet->send("dice>roll>1d20");

// Example: Roll 3d6 (three 6-sided dice)
benternet->send("dice>roll>3d6");


Request Format

The service uses standard dice notation:

    Format: NdM where:
        N is the number of dice to roll (must be a positive integer)
        d is the separator
        M is the number of sides on each die (must be a positive integer)

Examples:

    1d20 - Roll one 20-sided die
    3d6 - Roll three 6-sided dice
    2d10 - Roll two 10-sided dice

Response Format

The service will respond with either:

    A numeric result representing the total sum of all dice rolled
    An error message if your request was invalid

Error Messages

You may receive the following error messages:

    Format Error: Error: Expected format is [DiceAmount]d[DiceSize]. E.g.: 1d20.
    Invalid Parameters: Error: DiceAmount and DiceSize must be positive integers.
    Internal Error: Internal service error.

