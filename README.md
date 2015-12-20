# ESP8266-Wifi-Scanner

This is a simple WiFi scanner based on the Sming Framework that shows
on the serial port the Wifi Networks found, the protection mode (WEP/WPA)
and the power level. Lower values (more negative) are weaker networks.
Higher values, more positive values, are stronger networks.

If an I2C 1602 display is connected, the networks found are also displayed
on the LCD.

With a portable board powered up by an USB power bank, we can survey WiFi
networks available at a certain location along side their signal power. 
