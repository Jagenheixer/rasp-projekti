#  Tietoliikenteen sovellusprojekti — Syksy 2025

##  Projektikuvaus

Opiskelijoiden tehtävänä on suunnitella **nRF5340 Development Kit** -alustalla toimiva **client**, joka mittaa anturidataa (esim. kiihtyvyysanturi) ja välittää tiedot langattomasti **IoT-reitittimelle (Raspberry Pi v2)**.

Raspberry Pi toimii välittäjänä ja lähettää datan joko:
- **omalle MySQL-palvelimelle**, tai  
- **Oamkin MySQL-palvelimelle**.

Tietokantaan tallentuvaan dataan on käytettävissä:
- **TCP-sokettirajapinta**, jos käytössä on Oamkin MySQL-tietokanta  
- **HTTP API**, jota voidaan hyödyntää tiedon hakemiseen

Kerättyä dataa haetaan HTTP-rajapinnan kautta omalle kannettavalle kehitetyn **Python-ohjelman** avulla, ja sitä käsitellään **koneoppimista** varten.

---


##  Apache2-asennus ja testaus

Asennettu **Apache2**-palvelin Ubuntu Serverille ja testattu selaimilla:

 [http://172.20.241.37/](http://172.20.241.37/) 
 ---
 [http://172.20.241.36/](http://172.20.241.36/)

Apache näytti onnistuneesti oletussivunsa:  
> *"It works!"*


---

##  Sivun muokkaus

Muokattu oletussivua (`index.html`) ja luotu yksinkertainen oma HTML-sivun.

 **Tiedostopolku:**
/var/www/html/index.html

---
##  Palvelimen tiedot

| Ominaisuus | Arvo |
|-------------|------|
| Käyttöjärjestelmä | Ubuntu Server |
| Web-palvelin | Apache2 |
| Web root -hakemisto | `/var/www/html` |
| Pääsivu | `index.html` |

---

## Arkkitehtuurikuva

![Arkkitehtuurikuva](https://github.com/Jagenheixer/rasp-projekti/blob/main/arkkitehtuuri.png)

---


##  Projektin eteneminen
**Viikko 1.**
- [x] Github repo luotu
- [x] Kanban- taulu tehty (backlog, TO DO, In Progress, Review, Done)
- [x] README
- [x] GIT BRANCHIT käytössä (main, linux-kokeilut, testailu ....)
- [x] Markdown perusteet
- [x] Videot ja Git/Github kurssit
- [x] Projektin kuvaus ja repositorion rakenteen luonti

---
**Viikko 2**
- [x] Karin tehtävät / Katariina
- [x] Karin tehtävät / Tiina
---
*Katariinan palvelin*
**Viikko 3**
- [x]  Raspberry Pi asennettu ja liitetty labran verkkoon
- [x]  SSH- yhteys testattu onnistuneesti
*Katariinan palvelin*
*Linux palomuuri (netfilter/iptables)*
- [x] firewall.sh luotu
- [x] portit 22 (SSH) ja 80 (HTTP) sallittu
- [x] ICMP sallittu
- [x] "Takaovi" (193.167.100.97) sallittu
- [x] MYSQL- portin (3306) sääntö lisätty firewall.sh
- [x] firewall.sh pushattu Githubii
- [x] testattu että Tiina pääsee MYSQL:ään

*Apache + PHP + MySQL*
- [x] Apache 2 asennettu
- [x] PHP asennettu / testattu
- [x] MYSQL- testidata
- [x] db_config.php luotu ja toimii
- [x] luedataa.php toimii selaimessa
- [x] rawdata- taulun data näkyy oikein
- [x] virhee debugattu (PDO, bind-address, config polut yms)

*Python -> MySQL
- [x] Python virtuaaliympäristö luotu
- [x] mysql-connector-python asennettu
- [x] testisensori-skripti (rasp_insert_testi_data.py) toimii
- [x] data näkyy rawdata- taulussa ja luedataa.php sivulla

*BLE -> Raspberry -> MySQL (oikea anturi)
- [x] Nordic nRF5340dk asennettu
- [x] BLE- kehitysympäristö aloitettu
- [ ] BLE- serveri Nordicilla
- [ ] BLE- client Raspberryllä (bleak)
- [ ] mittauksien tallennus MySQL:ään BLE:n kautta


*Liikenteen kaappaus ja analysointi*
- [x] tcpdump- kaappaus portista 89
- [x] pcap- tiedosto siirretty omalle koneelle
- [x] Wireshark analyysi
     - [x] TCP handshake kesto
     - [x] HTTP- vastauksen content- type
     - [x] HTTP- vastauksen server
- [x]  selaimen devtoolseja kokeiltu


*Python HTTP- client*
- [x]  HTTP GET -> CSV (requests-lib)
- [x]  Tallennus groupid-data.csv
- [x]  testattu



*Python TCP- client*
- [x] Socket yhteys porttiin 20000
- [x] lähettää groupid rivinvaihdolla
- [x] vastaanottaa max 300 riviä
- [x] tallentaa tiedostoksi
- [x] testattu
- [x] wireshark- analyysi tcp- siirrosta

*SSH- avaimet*
- [x] ed25519- avainpari
- [x] julkinen avain palvelimelle
- [x] PasswordAuthentication pois
- [x] testattu -> SSH toimii vain avaimella

---
**Viikko 4**
- [x] Katso lisenssivideo ja lue lisenssikalvot
- [x] Julkiset APIt ja Cataas
- [x] Thunder Client - REST API testaus
- [x] Oulun liikenteen GraphQL (POST)
- [x] Python TCP- socket client
- [x] Wireshark (TCP- client analyysi)

---
**Viikko 5**
*Karin tehtäviä*
- [ ] Katariina
- [ ] Tiina
##  Yhteenveto

nnnnnnn

