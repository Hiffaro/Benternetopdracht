```markdown
# Benternet Dobbelstenen Service
==============================

Een netwerkdienst voor dobbelstenen gebaseerd op het Benternet berichtensysteem met ZeroMQ en Qt. Deze service stelt clients in staat om dobbelsteenworpen aan te vragen met verschillende opties.

## Functies
--------

1. Basis Dobbelsteenworpen: Gooi elk aantal en type dobbelstenen (bijv. 1d20, 3d6)
2. Modificaties: Tel waarden op of trek ze af van worpen (bijv. 1d20+5)
3. Voordeel/Nadeel: Gooi tweemaal en behoud het hoogste (adv) of laagste (dis) resultaat
4. DC Controles: Vergelijk werpresultaten met een moeilijkheidsgraad (DC)
5. Aangepaste Identificaties: Voeg aangepaste identificaties toe aan je verzoeken

## Berichtformaat
-------------

Basis Worp:
```
dice>roll>[identificatie>]XdY[±Z][adv/dis]
```

Worp tegen DC:
```
dice>rollvs>[identificatie>]XdY[±Z][adv/dis]>DC
```

Waarbij:
- X = Aantal dobbelstenen (optioneel, standaard 1)
- Y = Aantal zijden op de dobbelsteen
- Z = Modificatie (optioneel)
- adv/dis = Gooi met voordeel of nadeel (optioneel)
- identificatie = Aangepaste identificatie (optioneel)
- DC = Moeilijkheidsgraad om tegen te controleren

## Antwoordformaat
--------------

Basis Worp Antwoord:
```
dice>result>[identificatie>]N
```

Worp tegen DC Antwoord:
```
dice>result>[identificatie>]pass|fail
```

## Terminal Voorbeelden
-------------------

Basis Dobbelsteenworpen:
```
> dice>roll>1d20                    | dice>result>17
> dice>roll>4d6                     | dice>result>14
> dice>roll>1d12                    | dice>result>8
> dice>roll>d10                     | dice>result>6
```
Dobbelsteenworpen met Modificaties:
```
> dice>roll>1d20+5                  | dice>result>23
> dice>roll>1d20-3                  | dice>result>12
> dice>roll>2d8+4                   | dice>result>15
```

Dobbelsteenworpen met Voordeel/Nadeel:
```
> dice>roll>1d20adv                 | dice>result>18
> dice>roll>1d20dis                 | dice>result>7
> dice>roll>1d20+2adv               | dice>result>19
```

Dobbelsteenworpen met Identificaties:
```
> dice>roll>aanval>1d20             | dice>result>aanval>15
> dice>roll>acrobatiek>1d20+3       | dice>result>acrobatiek>18
> dice>roll>schade>2d6+2            | dice>result>schade>11
```
DC Controles:
```
> dice>rollvs>1d20>15               | dice>result>pass
                                    | (of dice>result>fail)
> dice>rollvs>1d20+4>18             | dice>result>pass
                                    | (of dice>result>fail)
> dice>rollvs>1d20adv>12            | dice>result>pass
                                    | (of dice>result>fail)
> dice>rollvs>sluipen>1d20+5>17     | dice>result>sluipen>pass
                                    | (of dice>result>sluipen>fail)
> dice>rollvs>redding>1d20-1dis>10  | dice>result>redding>pass
                                   | (of dice>result>redding>fail)
```
Foutafhandeling:
```
> dice>roll>20                      | dice>result>Error: Expected format is [DiceAmount]d[DiceSize](+/-[Modifier])([dis/adv]). E.g.: 1d20.
> dice>roll>d                       | dice>result>Error: Expected format is [DiceAmount]d[DiceSize](+/-[Modifier])([dis/adv]). E.g.: 1d20.
> dice>roll>1d20++3                 | dice>result>Error: Expected format is [DiceAmount]d[DiceSize](+/-[Modifier])([dis/adv]). E.g.: 1d20.
```
Voorbeeld Sessie:
```
> dice>roll>d20                     | dice>result>14
> dice>roll>aanval>d20+2            | dice>result>aanval>19
> dice>rollvs>aanval>d20+2>15       | dice>result>aanval>pass
> dice>roll>schade>2d6+3            | dice>result>schade>12
> dice>roll>d20adv                  | dice>result>18
> dice>rollvs>redding>d20-1>12      | dice>result>redding>fail
```

## Uitgebreide Voorbeelden
----------------------

Basis Dobbelsteenworpen:

1. Enkele d20
   Verzoek: dice>roll>1d20
   Antwoord: dice>result>17

2. Meerdere dobbelstenen
   Verzoek: dice>roll>4d6
   Antwoord: dice>result>14

3. Dobbelsteen met andere zijden
   Verzoek: dice>roll>1d12
   Antwoord: dice>result>8

4. Standaard 1 dobbelsteen (zonder aantal)
   Verzoek: dice>roll>d10
   Antwoord: dice>result>6

Dobbelsteenworpen met Modificaties:

1. Positieve modificatie
   Verzoek: dice>roll>1d20+5
   Antwoord: dice>result>23

2. Negatieve modificatie
   Verzoek: dice>roll>1d20-3
   Antwoord: dice>result>12

3. Meerdere dobbelstenen met modificatie
   Verzoek: dice>roll>2d8+4
   Antwoord: dice>result>15

Dobbelsteenworpen met Voordeel/Nadeel:

1. Worp met voordeel
   Verzoek: dice>roll>1d20adv
   Antwoord: dice>result>18

2. Worp met nadeel
   Verzoek: dice>roll>1d20dis
   Antwoord: dice>result>7

3. Worp met voordeel en modificatie
   Verzoek: dice>roll>1d20+2adv
   Antwoord: dice>result>19

Dobbelsteenworpen met Identificaties:

1. Aanvalsworp
   Verzoek: dice>roll>aanval>1d20
   Antwoord: dice>result>aanval>15

2. Vaardigheidscheck met modificatie
   Verzoek: dice>roll>acrobatiek>1d20+3
   Antwoord: dice>result>acrobatiek>18

3. Schadeberekening
   Verzoek: dice>roll>schade>2d6+2
   Antwoord: dice>result>schade>11

DC Controles:

1. Basis DC check
   Verzoek: dice>rollvs>1d20>15
   Antwoord: dice>result>pass (als worp ≥ 15)
   Antwoord: dice>result>fail (als worp < 15)

2. DC check met modificatie
   Verzoek: dice>rollvs>1d20+4>18
   Antwoord: dice>result>pass (als worp+4 ≥ 18)
   Antwoord: dice>result>fail (als worp+4 < 18)

3. DC check met voordeel
   Verzoek: dice>rollvs>1d20adv>12
   Antwoord: dice>result>pass (als hoogste worp ≥ 12)
   Antwoord: dice>result>fail (als hoogste worp < 12)

4. DC check met identificatie
   Verzoek: dice>rollvs>sluipen>1d20+5>17
   Antwoord: dice>result>sluipen>pass (als worp+5 ≥ 17)
   Antwoord: dice>result>sluipen>fail (als worp+5 < 17)

5. Complexe DC check
   Verzoek: dice>rollvs>redding>1d20-1dis>10
   Antwoord: dice>result>redding>pass (als laagste worp-1 ≥ 10)
   Antwoord: dice>result>redding>fail (als laagste worp-1 < 10)

## Foutafhandeling
--------------

De service geeft foutmeldingen terug voor ongeldige verzoeken:

```
dice>result>[identificatie>]Error: Expected format is [DiceAmount]d[DiceSize](+/-[Modifier])([dis/adv]). E.g.: 1d20.
```

## Bouwen en Uitvoeren
-----------------

### Vereisten

Voordat je de Benternet Dobbelstenen Service kunt bouwen, zorg ervoor dat je de volgende afhankelijkheden hebt geïnstalleerd:

- **Qt 5.12+** (Core en Network modules)
- **ZeroMQ 4.3.x**
- **nzmqt** (Qt wrapper voor ZeroMQ)
- **Benternet bibliotheek**

### Installatie van Afhankelijkheden

#### Ubuntu/Debian
```bash
# Installeer Qt5
sudo apt update
sudo apt install qt5-default qtbase5-dev

# Installeer ZeroMQ
sudo apt install libzmq3-dev

# Installeer Git (indien nog niet geïnstalleerd)
sudo apt install git

# Kloon en bouw nzmqt
git clone https://github.com/jonnydee/nzmqt.git
cd nzmqt
qmake
make
sudo make install

# Kloon Benternet bibliotheek
git clone https://github.com/your-repo/benternet.git
cd benternet
qmake
make
sudo make install
```

#### macOS (met Homebrew)
```bash
# Installeer Qt5
brew install qt5

# Installeer ZeroMQ
brew install zeromq

# Kloon en bouw nzmqt
git clone https://github.com/jonnydee/nzmqt.git
cd nzmqt
/usr/local/opt/qt5/bin/qmake
make
make install

# Kloon Benternet bibliotheek
git clone https://github.com/your-repo/benternet.git
cd benternet
/usr/local/opt/qt5/bin/qmake
make
make install
```

#### Windows (met MSVC)
1. Download en installeer Qt vanaf de [Qt website](https://www.qt.io/download)
2. Download en installeer ZeroMQ vanaf [ZeroMQ releases](https://github.com/zeromq/libzmq/releases)
3. Kloon nzmqt en Benternet repositories
4. Open Qt Creator, laad de .pro bestanden, en bouw

### Bouwen van de Dobbelstenen Service

1. Kloon de repository:
```bash
git clone https://github.com/your-username/benternet-dice-service.git
cd benternet-dice-service
```

2. Configureer en bouw het project:
```bash
qmake
make
```

3. Voer de service uit:
```bash
./dice-service
```

## Probleemoplossing
----------------

### Veelvoorkomende Bouwproblemen

#### ZeroMQ Bibliotheek Niet Gevonden
**Fout**: ```cannot find -lzmq``` of ```zmq.h: No such file or directory```

**Oplossing**:
- Zorg ervoor dat ZeroMQ correct is geïnstalleerd
- Controleer of het bibliotheekpad correct is ingesteld:
```bash
# Op Linux
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
# Op macOS
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/usr/local/lib
```
- Voeg het bibliotheekpad toe aan je .pro bestand:
```
LIBS += -L/usr/local/lib -lzmq
INCLUDEPATH += /usr/local/include
```

#### nzmqt Bibliotheek Niet Gevonden
**Fout**: ```nzmqt/nzmqt.hpp: No such file or directory```

**Oplossing**:
- Zorg ervoor dat nzmqt correct is gebouwd en geïnstalleerd
- Voeg het include-pad toe aan je .pro bestand:
```
INCLUDEPATH += /path/to/nzmqt/include
```

#### Benternet Bibliotheek Niet Gevonden
**Fout**: ```benternet.h: No such file or directory```

**Oplossing**:
- Zorg ervoor dat de Benternet bibliotheek correct is gebouwd en geïnstalleerd
- Voeg de include- en bibliotheekpaden toe aan je .pro bestand:
```
INCLUDEPATH += /path/to/benternet/include
LIBS += -L/path/to/benternet/lib -lbenternet
```

#### Qt Versie Mismatch
**Fout**: ```This file was generated by an earlier version of MOC```

**Oplossing**:
- Maak de build-directory schoon en herbouw:
```bash
make clean
qmake
make
```
- Zorg ervoor dat je dezelfde Qt-versie gebruikt voor alle componenten

### Runtime Problemen

#### Verbinding met Benternet Mislukt
**Fout**: ```Starting benternet connection... [Failed]```

**Oplossing**:
- Controleer of de Benternet broker draait
- Verifieer netwerkconnectiviteit naar de broker
- Controleer firewall-instellingen die ZeroMQ-verkeer kunnen blokkeren
- Probeer met een ander broker-adres:
```cpp
// In je code
benternet->setBrokerAddress("tcp://new-broker-address:port");
```

#### Berichten Worden Niet Ontvangen
**Fout**: Geen foutmelding, maar geen dobbelsteenresultaten

**Oplossing**:
- Verifieer dat je abonnementsstrings exact overeenkomen met wat clients publiceren
- Controleer op hoofdlettergevoeligheidsproblemen in berichtonderwerpen
- Voeg debug-output toe om te verifiëren dat berichten worden verzonden en ontvangen:
```cpp
qDebug() << "Ontvangen bericht:" << message;
```

#### Problemen met Random Number Generator
**Fout**: Dobbelsteenworpen lijken te herhalen of niet willekeurig te zijn

**Oplossing**:
- Zorg ervoor dat de random number generator goed is geïnitialiseerd:
```cpp
// Vervang
static QRandomGenerator rng = QRandomGenerator::securelySeeded();
// Met
static QRandomGenerator rng(QDateTime::currentMSecsSinceEpoch());
```

#### ZMQ Exception Tijdens Verzenden
**Fout**: ```ZMQException: Resource temporarily unavailable```

**Oplossing**:
- Implementeer retry-logica voor verzendingen:
```cpp
int retries = 3;
while (retries > 0) {
    try {
        benternet->send(result);
        break;
    } catch(nzmqt::ZMQException &e) {
        qDebug() << "ZMQException: " << e.what();
        retries--;
        if (retries > 0) {
            QThread::msleep(100);  // Wacht voor retry
        }
    }
}
```

### Prestatieproblemen

Als de service traag of niet-responsief wordt:

1. Controleer systeembronnen (CPU, geheugen)
2. Overweeg een berichtenwachtrij te implementeren om grote volumes te verwerken
3. Voeg timeouts toe voor operaties:
```cpp
QTimer::singleShot(5000, [&]() {
    // Afhandelen timeout
    qDebug() << "Operatie timed out";
});
```

### Voor Verdere Hulp

Als je problemen tegenkomt die hier niet worden behandeld:
- Raadpleeg de ZeroMQ-documentatie: [zeromq.org](https://zeromq.org/documentation/)
- Raadpleeg de Qt-documentatie: [doc.qt.io](https://doc.qt.io/)
- Open een issue op de GitHub-repository met gedetailleerde informatie over je probleem

## Netwerkarchitectuur
------------------

De Benternet Dobbelstenen Service maakt gebruik van het Benternet berichtensysteem, dat is gebaseerd op ZeroMQ. Hieronder is een vereenvoudigd schema van de architectuur:

```
+----------------+       +----------------+       +----------------+
|                |       |                |       |                |
|  Client        |------>|  Benternet     |------>|  Dice Service  |
|  (Aanvrager)   |       |  (Broker)      |       |  (Verwerker)   |
|                |<------|                |<------|                |
+----------------+       +----------------+       +----------------+
```

1. **Client**: Stuurt dobbelsteenverzoeken in het gespecificeerde formaat
2. **Benternet Broker**: Routeert berichten tussen clients en services
3. **Dice Service**: Abonneert zich op dobbelsteenverzoeken, verwerkt ze, en stuurt resultaten terug

De service gebruikt het publish/subscribe patroon van ZeroMQ:
- De service abonneert zich op de topics ```dice>roll>``` en ```dice>rollvs>```
- Clients publiceren verzoeken naar deze topics
- De service publiceert resultaten naar het topic ```dice>result>```
- Clients kunnen zich abonneren op dit topic om resultaten te ontvangen
```

