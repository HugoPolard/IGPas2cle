 /*
 * RFID-RC522 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * RFID-RC522 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RFID-RC522.  If not, see <http://www.gnu.org/licenses/>.
 */

version 1.0
    initial version

version 1.1
    fixed issue with sscanf warning.

version 1.11
    fixed different issues for unbuntu mate compiler
    (much more strict)

version 1.12
    fixed issue with endless loop

version 1.50
    fixed number of issues and code clean-up

version 1.60 / october 2017
    code clean-up
    extra command line options
    handling of value block	

version 1.65.3
   It is now possible to write and read (multiple) data blocks on a classic card from the command line. 
   All options are in capitals. Enable to write and read a message to the card from the command line. 
   If the message does not fit within a single 16 bytes block, the next data address block 
   (which might be in a next sector) will be used. 
   In principle one could read or write 16 sector * 3 (blocks) * 16 bytes.
   code clean-up for quality

 
For details about the program, installation and options see the included RC522.odt.
   
Installation instructions
**************************
Make sure you are on latest versions
1. sudo apt-get update & sudo apt-get upgrade & raspi-update
2. Make sure you have SPI in raspi-config (advanced section)
3. Make sure that Device tree is enabled in raspi config (advanced)

For Raspbian 9 (stretch) no need to enable "Device tree" as [it always enabled](https://www.raspberrypi.org/forums/viewtopic.php?p=1257655&sid=a088f010b2b78ab5f14e22e747b8ff2b#p1257655).
2. Make sure you have SPI in raspi-config (5. Interfacing Options -> P4)

cd  /home/pi (assuming this is your home directory)

BCM2835 library (library 1.56 was the current version at the time of writting)
Install latest from BCM2835 from : http://www.airspayce.com/mikem/bcm2835/

1. wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.56.tar.gz
2. tar -zxf bcm2835-1.56.tar.gz
3. cd bcm2835-1.56
4. ./configure
5. sudo make check
6. sudo make install
7. cd  ..

RC522 utility
0. git clone https://github.com/paulvha/rfid-rc522.git
1. cd rfid-rc522
2. tar -zxvf rc522-1-6-5.tar.gz
3. cd rc522
4. ./mc                             # compile executable
5. sudo cp RC522.conf   /etc
6. sudo chmod 666 /etc/RC522.conf
7. edit   /etc/RC522.conf           # if necessary

Reboot
cd rc522
Run command as root :  ./rc522  -h
