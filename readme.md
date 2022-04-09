# PRÀCTICA 5: TERMÒSTAT

Un termòstat és un regulador de temperatura que permet de mantenir constant la temperatura de l’interior d’un recinte.

En aquesta pràctica en crearem un indicat per utilitzar-lo en un sistema de calefacció.

## Requeriments

* ESP32
* Sensor temperatura I2C HTU21D
* Display I2C SSD1306
* 2 polsadors
* 1 diode LED

## Funcionament

Aquest termòstat funciona de la següent manera:

El sensor de temperatura HTU21D està constantment llegint la temperatura ambient i la compara amb una establerta (per defecte 21º C). Aquesta temperatura es pot canviar mitjançant els dos polsadors, un per augmentar-la i l'altre per disminuir-la. Si la temperatura ambient és inferior a la temperatura establerta, s'activarà la sortida GPIO 19, on podem tenir connectat, per exemple, un LED o un circuit que encengui la calefacció de la llar.

A més a més, si es té connectada l'ESP32 a una xarxa wifi, es podrà consultar una web, que té la IP que es mostra per pantalla, la temperatura actual, humitat i la temperatura establerta al termòstat.

[Vídeo del funcionament](https://drive.google.com/file/d/1vi2bVQkk9TjXFpX0RurgPKSNPhAjmCIw/view?usp=sharing) (s'ha de tenir iniciada la sessió amb un usuari acabat en "upc.edu").
