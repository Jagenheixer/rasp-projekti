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

Asennettu **Apache2**-palvelin Ubuntu Serverille ja testattu selaimella:

 [http://172.20.241.37/](http://172.20.241.37/)

Apache näytti onnistuneesti oletussivunsa:  
> *"It works!"*

---

##  Sivun muokkaus

Muokattu oletussivua (`index.html`) ja luotu yksinkertainen oma HTML-sivun.

 **Tiedostopolku:**
/var/www/html/index.html

##  Palvelimen tiedot

| Ominaisuus | Arvo |
|-------------|------|
| Käyttöjärjestelmä | Ubuntu Server |
| Web-palvelin | Apache2 |
| Web root -hakemisto | `/var/www/html` |
| Pääsivu | `index.html` |

---



---


##  Projektin eteneminen

- [x] Apache2 asennettu ja testattu
- [x] Oma etusivu (index.php) luotu ja testattu
- [x] Tietokantayhteys MySQL:ään (raspberrydata)
- [x] HTTP API (GET/POST) toteutettu ja testattu (api.php)
- [x] Admin-kansio ja PHP-info salasanasuojattuna
- [ ] Raspberry Pi -reititin asennettu ja konfiguroitu
- [ ] Python-ohjelma datan hakemiseen/käsittelyyn
- [ ] Koneoppimisen analyysi

##  Yhteenveto

 Apache2 asennettu ja toimii  
 Oletussivu näkyi selaimessa  
 Omaa sisältöä muokattu onnistuneesti  


