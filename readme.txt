Benternet Dobbelstenen Service
==============================

Een netwerkdienst voor dobbelstenen gebaseerd op het Benternet berichtensysteem met ZeroMQ en Qt. Deze service stelt clients in staat om dobbelsteenworpen aan te vragen met verschillende opties.

Functies
--------

1. Basis Dobbelsteenworpen: Gooi elk aantal en type dobbelstenen (bijv. 1d20, 3d6)
2. Modificaties: Tel waarden op of trek ze af van worpen (bijv. 1d20+5)
3. Voordeel/Nadeel: Gooi tweemaal en behoud het hoogste (adv) of laagste (dis) resultaat
4. DC Controles: Vergelijk werpresultaten met een moeilijkheidsgraad (DC)
5. Aangepaste Identificaties: Voeg aangepaste identificaties toe aan je verzoeken

Berichtformaat
-------------

Basis Worp:
dice>roll>[identificatie>]XdY[±Z][adv/dis]

Worp tegen DC:
dice>rollvs>[identificatie>]XdY[±Z][adv/dis]>DC

Waarbij:
- X = Aantal dobbelstenen (optioneel, standaard 1)
- Y = Aantal zijden op de dobbelsteen
- Z = Modificatie (optioneel)
- adv/dis = Gooi met voordeel of nadeel (optioneel)
- identificatie = Aangepaste identificatie (optioneel)
- DC = Moeilijkheidsgraad om tegen te controleren

Antwoordformaat
--------------

Basis Worp Antwoord:
dice>result>[identificatie>]N

Worp tegen DC Antwoord:
dice>result>[identificatie>]pass|fail

Uitgebreide Voorbeelden
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

Foutafhandeling
--------------

De service geeft foutmeldingen terug voor ongeldige verzoeken:

dice>result>[identificatie>]Error: Expected format is [DiceAmount]d[DiceSize](+/-[Modifier])([dis/adv]). E.g.: 1d20.

Technische Details
----------------

De service gebruikt:
- Een beveiligde willekeurige getallengenerator (QRandomGenerator::securelySeeded())
- Reguliere expressies voor het verwerken van invoer
- ZeroMQ voor netwerkberichten via Benternet
- Qt voor event-driven architectuur

Code Implementatie
----------------

De belangrijkste componenten zijn:

1. Regex patroon voor dobbelsteenverzoeken:
   R"-(^(?<amount>\d+){,1}d(?<faces>\d+)(?<modifier>[+-]\d+){,1}(?<advdis>(adv)|(dis)){,1}$)-"

2. Dobbelsteenworp functie:
   QByteArray doRoll(QString rollRequest, QString ident) {
       static QRandomGenerator rng = QRandomGenerator::securelySeeded();
       // ...
   }

3. DC check functie:
   QByteArray doRollVs(QString result, QString dc, QString ident) {
       // ...
   }

4. Berichtverwerking:
   QObject::connect(benternet.data(), &Benternet::onReceive, [&](const QByteArray &message) {
       // ...
   });

Gebruik in Spelomgevingen
-----------------------

Deze service is ideaal voor:
1. Online rollenspellen (RPGs)
2. Virtuele bordspellen
3. Geautomatiseerde spelomgevingen
4. Toepassingen die eerlijke willekeurige getallen nodig hebben

Bouwen en Uitvoeren
-----------------

1. Zorg ervoor dat je de vereiste afhankelijkheden hebt:
   - Qt5
   - ZeroMQ
   - nzmqt
   - Benternet bibliotheek

2. Bouw het project:
   qmake
   make

3. Voer de service uit:
   ./dice-service
