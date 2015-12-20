// Please configure your SSID, passwords and OW key on the user_config.h file
#include <user_config.h>

#include <SmingCore/SmingCore.h>
#include <Libraries/LiquidCrystal/LiquidCrystal_I2C.h>

// For more information visit useful wiki page: http://arduino-info.wikispaces.com/LCD-Blue-I2C
#define I2C_LCD_ADDR 0x27
LiquidCrystal_I2C lcd(I2C_LCD_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

Vector<String> wifiList;
int wifiListSize = 0;
int wifiListPos  = 0;

Timer displayTimer;
Timer checkTimer;

String deviceid;

//forward declarations
void displayNets();
void networkScanCompleted(bool succeeded, BssList list);

void startMain() {
    checkTimer.stop();
}

void displayNets() {
  if ( wifiListPos >= wifiListSize ){
	displayTimer.stop();	
    	wifiListPos = 0;
	lcd.clear();
        lcd.setCursor( 0 , 0 );
        lcd.print("Scanning...");
        WifiStation.startScan(networkScanCompleted);

  } else {
	lcd.clear();
	lcd.setCursor( 0 , 0 );
	if ( wifiListSize > 0 ) {
		lcd.setCursor( 0 , 0 ) ;
		lcd.print( wifiList[wifiListPos] ); 
		wifiListPos++;
		lcd.setCursor( 0 , 1 ) ;
		lcd.print( "P: " + wifiList[wifiListPos] ); 
		wifiListPos++;
	} else {
		lcd.setCursor( 0 , 0 );
		lcd.print("No WiFi Nets");
		lcd.setCursor( 0 , 1 );
		lcd.print("found.");
	}
  	//Serial.println("pos: " + String(wifiListPos));
  }
}
void networkScanCompleted(bool succeeded, BssList list) {

    if (!succeeded) {
        Serial.println("Failed to scan networks");
        return;
    }

    wifiList.clear();
    for (int i = 0; i < list.count(); i++) {
        Serial.print("\tWiFi: ");
        Serial.print(list[i].ssid);
        wifiList.add( list[i].ssid);
        Serial.print(", ");
        Serial.print(list[i].getAuthorizationMethodName());
        if (list[i].hidden) Serial.print(" (hidden)");
        Serial.println("");
        Serial.println("\tPower: " + String(list[i].rssi));
        wifiList.add( String(list[i].rssi) + " " + String(list[i].getAuthorizationMethodName()));
        Serial.println();
    }
    wifiListSize = wifiList.size();
    wifiListPos = 0;
    displayTimer.initializeMs(5 * 1000, displayNets).start();
}

// Will be called when WiFi station was connected to AP

void connectOk() {
    char data[200];
    char dt[20] = "";

    Serial.println("I'm CONNECTED");
    // Let's get the Mac address, that should be unique...
    deviceid = WifiStation.getMAC();
    deviceid.toUpperCase();
    Serial.println("Device ID: " + deviceid);

    checkTimer.initializeMs(5 * 1000, startMain).start();
}

// Will be called when WiFi station timeout was reached

void connectFail() {
    Serial.println("I'm NOT CONNECTED. Need help :(");

    WifiStation.waitConnection(connectOk, 20, connectFail); // We recommend 20+ seconds for connection timeout at start
}





void sysReady() {

    Serial.println("System ready callback called....");
}

void init() {

    system_set_os_print(0);

    Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
    Serial.systemDebugOutput(false); // Disable debug output to serial

    WifiStation.config(WIFI_SSID, WIFI_PWD);
    WifiStation.enable(true);
    WifiAccessPoint.enable(false);

    lcd.begin(16, 2);   // initialize the lcd for 16 chars 2 lines, turn on backlight
    // ------- Quick 3 blinks of backlight  -------------
    for(int i = 0; i< 3; i++)
    {
            lcd.backlight();
            delay(150);
            lcd.noBacklight();
            delay(250);
    }
    lcd.backlight(); // finish with backlight on

    lcd.setCursor( 0 , 0 );
    lcd.print("Scanning...");
    WifiStation.startScan(networkScanCompleted);

    // Run our method when station was connected to AP (or not connected)
    WifiStation.waitConnection(connectOk, 20, connectFail); // We recommend 20+ seconds for connection timeout at start

    System.onReady(sysReady);
       
}
