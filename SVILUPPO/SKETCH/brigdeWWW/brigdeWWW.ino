/*
  Arduino Yún Bridge example
  This example for the Arduino Yún shows how to use the
  Bridge library to access the digital and analog pins
  on the board through REST calls. It demonstrates how
  you can create your own API when using REST style
  calls through the browser.
  Possible commands created in this shetch:
  "/arduino/digital/13"     -> digitalRead(13)
  "/arduino/digital/13/1"   -> digitalWrite(13, HIGH)
  "/arduino/analog/2/123"   -> analogWrite(2, 123)
  "/arduino/analog/2"       -> analogRead(2)
  "/arduino/mode/13/input"  -> pinMode(13, INPUT)
  "/arduino/mode/13/output" -> pinMode(13, OUTPUT)
  This example code is part of the public domain
  http://www.arduino.cc/en/Tutorial/Bridge
*/

#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <LiquidCrystal.h>

#define allPin  "all"

// Listen to the default port 5555, the Yún webserver
// will forward there all the HTTP requests you send
BridgeServer server;

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(7, 12, 2, 3, 4, 5);

void setup() {
  // Bridge startup
  pinMode(13, OUTPUT);
  //pinMode(11, OUTPUT);
  //pinMode(8, OUTPUT);
  
  digitalWrite(13, LOW);
  //digitalWrite(11, LOW);
  //digitalWrite(8, LOW);
  
  delay(60000);
  Bridge.begin();
  //lcd.begin(16, 2);
  //digitalWrite(13, HIGH);

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  //server.listenOnLocalhost();
  server.begin();

  Serial.begin(9600);
  int i;
  // azzero tutti i pin digitali
  for (i = 1; i <= 13; i++) {
	  //digitalWrite(i, 0);
  }
  Serial.println("READY");

}

void loop() {
  // Get clients coming from server
  BridgeClient client = server.accept();

  // There is a new client?
  if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();
  }

  delay(50); // Poll every 50ms
}

void process(BridgeClient client) {
  // read the command
  String command = client.readStringUntil('/');

  Serial.print("command " + command + "\n");
  // is "digital" command?
  if (command.indexOf("getalldigital") == 0) {
	  allDigitalCommand(client);
  }
  if (command == "digital") {
    digitalCommand(client);
  }

  // is "analog" command?
  if (command == "analog") {
    //analogCommand(client);
  }

  // is "mode" command?
  if (command == "mode") {
    modeCommand(client);
  }

  // is "lcd" command?
  if (command == "lcd") {
    //lcdCommand(client);
  }
}

void setHeaders(BridgeClient client){
  //adding headers
  client.println(F("Status: 200"));
  client.println(F("Access-Control-Allow-Origin: *"));
  client.println(F("Content-Type: text/html"));

  client.println(); //mandatory blank line
}

/*void lcdCommand(BridgeClient client) {
  String text = client.readStringUntil('/');
  String text2 = "";
  lcd.clear();
  if (text.length()-2 > 16)
  {
    text2 = text.substring(16, text.length()-2);
    lcd.print(text.substring(0,16));
    lcd.setCursor(0, 1);
    lcd.print(text2);
  }
  else
  {
    lcd.print(text.substring(0,text.length()-2));    
  }

  setHeaders(client);
  
  client.print(F("LCD: "));
  client.print(text);
}*/
void allDigitalCommand(BridgeClient client) {
	Serial.println("funzione allDigitalCommand");

	int pin, value, i;
	String mode = "json"; // json oppure vuoto
	String jsonString = "";

	if (mode == "json") {
		jsonString = "[";
	} else {
		jsonString = "{\"digital\":\"";
	}
	
	for (i = 1; i <= 13; i++) {
		Serial.println("ANALIZZO IL PIN D" + String(i));
		value = digitalRead(i);

		if (mode == "json") {
			jsonString += "{\"pin\":\"";
			jsonString += String(i);
			jsonString += "\",\"value\":\"";
			jsonString += value;
			jsonString += "\"}";

			if (i < 13) {
				jsonString += ",";
			} else {
				jsonString += "]";

			}
		} else {
			jsonString += String(value);
			if (i < 13) {
				jsonString += ",";
			} else {
				jsonString += "\"}";

			}
		}
	}
	setHeaders(client);
	client.println(jsonString);
	Serial.println(jsonString);
}

void digitalCommand(BridgeClient client) {
	int pin, value, mSec;
	//Read pin number
	pin = client.parseInt();
	Serial.println("E' IL PIN " + String(pin));

	// If the next character is a '/' it means we have an URL
	// with a value like: "/digital/13/1"
	if (client.read() == '/') {
		// rintraccio il valore da impostare al pin
		value = client.parseInt();
		Serial.println("LETTURA " + String(value));
		if (client.read() == '/') {
			// è presente il valore in millisecondi
			mSec = client.parseInt();
			Serial.println("MILLISEC " + String(mSec));
			digitalWrite(pin, value);
			delay(mSec);
			digitalWrite(pin, (value==1 ? 0 : 1));
			value = digitalRead(pin);
		} else {
			// è il valore secco
			Serial.println("Imposto il valore " + value);
			digitalWrite(pin, value);
		}
	} else {
		// faccio la lettura del valore del pin
		value = digitalRead(pin);
	}
	setHeaders(client);

  	String jsonString = "{\"pin\":\"";
	jsonString += pin;
	jsonString +="\",\"value\":\"";
	jsonString += value;
	jsonString +="\"}";
	
	// 
	client.println(jsonString);
	Serial.println(jsonString);

	// Update datastore key with the current pin value
	String key = "D";
	key += pin;
	Bridge.put(key, String(value));
}

/*void analogCommand(BridgeClient client) {
  int pin, value;

  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/analog/5/120"
  if (client.read() == '/') {
    // Read value and execute command
    value = client.parseInt();
    analogWrite(pin, value);

    // Send feedback to client
    setHeaders(client);
    
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" set to analog "));
    client.println(value);

    // Update datastore key with the current pin value
    String key = "D";
    key += pin;
    Bridge.put(key, String(value));
  } else {
    // Read analog pin
    value = analogRead(pin);

    // Send feedback to client
    client.print(F("Pin A"));
    client.print(pin);
    client.print(F(" reads analog "));
    client.println(value);

    // Update datastore key with the current pin value
    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
  }
}*/

void modeCommand(BridgeClient client) {
  int pin;

  // Read pin number
  pin = client.parseInt();

  // If the next character is not a '/' we have a malformed URL
  if (client.read() != '/') {
    client.println(F("error"));
    return;
  }

  String mode = client.readStringUntil('\r');

  if (mode == "input") {
    pinMode(pin, INPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as INPUT!"));
    return;
  }

  if (mode == "output") {
    pinMode(pin, OUTPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as OUTPUT!"));
    return;
  }

  client.print(F("error: invalid mode "));
  client.print(mode);
}
