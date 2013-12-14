Atmega8
=======

You can found the Atmega8 datasheet at the following link:

http://www.atmel.com/images/atmel-2486-8-bit-avr-microcontroller-atmega8_l_datasheet.pdf

We are writing the firmware in C language


Programmation
=============


Pour programmer la carte arduino, il faut utiliser la commande suivante:

avrdude -p m8 -c arduino -P usb -b 19200 -U flash:w:main.hex

ATTENTION A NE PAS REECRIRE SUR  LE BOOTLOADER sinon il va faloir que j'aille chercher le programmateur ( que j'ai magnifiquement oublié) en S333

Si la commande marche pas , tente avec usb en majuscule

Si tu as le moindre problème envoi moi un SMS
