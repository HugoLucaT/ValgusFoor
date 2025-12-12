# ValgusFoor
Tarkvara loodud Tallinna Ülikooli Asjade interneti tunni raames.

## TARKVARA STRUKTUURI KIRJELDUS

virtuaalnefoor.html - virtuaalse foori browseri fail
fullAdminPage.html - administraatori leht
fyysilinefoor.ino - füüsilise foori kood

## INSTALLIMISJUHIS

Kopeeri Git repositoorium kohalikku masinasse
Loo Google Apps Scripti kaks uut skripti ja kopeeri sisu Adminpage.txt ja TrafficLight.txt failidest.
Skiptidele lisa tabelid, saab kasutada näidis tabeleid.
Deploy skriptid, kopeeri URL aadressid failidesse:
virtuaalnefoor.html - rida 19
fullAdminPage.html - rida 67
fyysilinefoor.ino - rida 16
Lae kood kontrollerile: Kasuta koodi laadimiseks Arduino IDE või muu tarkvarage.


## LIIKLUSKORRALDUS
Foor on mõeldud kasutuseks ristmikul.

## ANDMEVAHETUSE KIRJELDUS
Kontrollerid/URL foorid pärivad serverist aja ja konfiguratsiooni sätted.
Server saab muutujate väärtused google sheets lehelt ja saadab kontrolleritele/URL fooridele.
Seadete muutmisel admin lehel saadetakse uued väärtused serverisse.
Server kirjutab uued väärtused google sheets tabelisse.

<img width="1007" height="682" alt="image" src="https://github.com/user-attachments/assets/3353e8f7-7098-40a5-9a58-9d4308ff679b" />

