#include <DHT.h> //  DHT.h library


#define DHTPIN 50
#define DHTTYPE  DHT22
DHT dht(DHTPIN, DHTTYPE);

#define SSID "EDRAKBH1"
#define PASS "edrakbh123"
#define host "184.106.153.149"          //"https://api.thingspeak.com/channels/218432"
#define writeAPIKey "TXQSR7VIGB6JMFR1"
//const char* ssid     = "EDRAKBH1";
//const char* password = "edrakbh123";
//const char* host = "api.thingspeak.com/channels/218432";
//const char* writeAPIKey = "TXQSR7VIGB6JMFR1";

String GET ="GET /update?key=TXQSR7VIGB6JMFR1";
const int ledR1 = 46;


//DHT dht(DHTPIN, DHTTYPE, 15);

void setup() {
  // Initialize sensor
pinMode(ledR1, OUTPUT);
 dht.begin();
  delay(1000);
Serial2.begin(115200);//9600 (mine), 57600, 115200
     Serial2.setTimeout(1000);
    
     //serial 0 is to usb
     Serial.begin(9600);
     
          
     while(!Serial); 
     while(!Serial2);
     
     //dbgSerial.begin(9600); //can't be faster than 19200 for softserial
     //dbgSerial.println("ESP8266 Demo");
     Serial.println("ESP8266 Demo on Mega2560");
    
    while(Serial2.available()>0)
    Serial2.read();
      
     delay(1000);
       //test if the module is ready
    Serial2.println("AT");
    delay(1000);
    Serial2.println("AT");
    delay(1000);
    Serial2.println("AT");
    
    /* Serial2.println("AT+RST");
     //delay(1000);
     //delay(1000);
     Serial.println("Resetting module");
     Serial2.flush();
     
     //if(Serial2.find("ready"))
     if(Serial2.find("Ready")||Serial2.find("ready"))
     {
       //dbgSerial.println("Module is ready");
       Serial.println("Module is ready");
     }
     else
     {
       //dbgSerial.println("Module have no response.");
       Serial.println("Module have no response.");
       while(1);
     }
     */
     delay(1000);
     //connect to the wifi
     boolean connected=false;
     for(int i=0;i<5;i++)
     {
       if(connectWiFi())
       {
         connected = true;
         break;
       }
     }
     if (!connected){while(1);}
     delay(5000);
     //print the ip addr
     /*
   Serial2.println("AT+CIFSR");
     Serial.println("ip address:");
     while (Serial2.available())
     Serial.write(Serial2.read());
   
   */
     //set the single connection mode
     Serial2.println("AT+CIPMUX=0");
   }
//  Connect to WiFi network
  //wifi.connectToAP("ssid","password");
 


void loop() {
 float humidity = dht.readHumidity();
 float temperature = dht.readTemperature();
 //String temp = float temperature;
 //String hum = float humidity;
  if (isnan(humidity) || isnan(temperature)) {
    return;
  }

// make TCP connections

  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd+= host;
  cmd += "\",80";
  Serial2.println(cmd);
  delay(1000);
  Serial.println(cmd);
  if(Serial2.find("Error")) return;
  /* String url = "/update?key=";
  url+=writeAPIKey;
  url+="&field1=";
  url+=temperature; //String(temperature);
  url+="&field2=";
  url+=humidity; //String(humidity);
  url+="\r\n";
    */ cmd = GET+ "&field1=" + dht.readTemperature() + "&field2=" + dht.readHumidity() +"\r\n";
      // cmd = (String("GET  ")+ url + "HTTP/1.1\r\n " + host + "\r\n"); 
     Serial2.print("AT+CIPSEND=");
     Serial2.println(cmd.length());

     
  
  

  if(Serial2.find(">"))
     {
       //dbgSerial.print(">");
       Serial.print(">");
       }
       else
       { //delay(1000);
         Serial2.println("AT+CIPCLOSE");
         //dbgSerial.println("connect timeout");
         Serial.println("connect timeout");
         delay(1000);
         return;
       }
       Serial2.print(cmd);
       delay(2000);
       //Serial.find("+IPD");
       while (Serial2.available())
       {
         char c = Serial2.read();
         //dbgSerial.write(c);
         Serial.write(c);
         //if(c=='\r') dbgSerial.print('\n');
         if(c=='\r') Serial.print('\n');
       }
       //dbgSerial.println("====");
       Serial.println("====");
       delay(1000);
     
/*  
  // Request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    delay(1000);
*/
}
void tempsense()
 {
 int ledtemp = dht.readTemperature();
 if ((ledtemp >=24) || (ledtemp >=20))
{  Serial.println("temperature exceeded");
   digitalWrite(ledR1,HIGH);
}

else
{
  Serial.println("temperature is perfercto!");
  digitalWrite(ledR1,LOW);
}
}

//ConnectWifi
 boolean connectWiFi()
     {
       Serial2.println("AT+CWMODE=1");
       String cmd="AT+CWJAP=\"";
       cmd+=SSID;
       cmd+="\",\"";
       cmd+=PASS;
       cmd+="\"";
       //dbgSerial.println(cmd);
       Serial2.println(cmd);
       Serial.println(cmd);
       delay(2000);
       if(Serial2.find("OK"))
       {
         //dbgSerial.println("OK, Connected to WiFi.");
         Serial.println("OK, Connected to WiFi.");
         return true;
         }else
         {
           //dbgSerial.println("Can not connect to the WiFi.");
           Serial.println("Can not connect to the WiFi.");
           return false;
         }
}


/*void loop() {
  value_temp = analogRead(sensor_temp);
  value_light = analogRead(sensor_light);
  value_humid = analogRead(sensor_humid);
  String temp =String(value_temp);// turn integer to string
  String light= String(value_light);// turn integer to string
  String humid=String(value_humid);// turn integer to string
  updateTS(temp,light, humid);
  delay(3000); //
}
//----- update the  Thingspeak string with 3 values
void updateTS( String T, String L , String H)
{
  // ESP8266 Client
  String cmd = "AT+CIPSTART=\"TCP\",\"";// Setup TCP connection
  cmd += IP;
  cmd += "\",80";
  sendDebug(cmd);
  delay(2000);
  if( Serial.find( "Error" ) )
  {
    debug.print( "RECEIVED: Error\nExit1" );
    return;
  }

  cmd = GET + "&field1=" + T +"&field2="+ L + "&field3=" + H +"\r\n";
  Serial.print( "AT+CIPSEND=" );
  Serial.println( cmd.length() );
  if(Serial.find( ">" ) )
  {
    debug.print(">");
    debug.print(cmd);
    Serial.print(cmd);
  }
  else
  {
    sendDebug( "AT+CIPCLOSE" );//close TCP connection
  }
  if( Serial.find("OK") )
  {
    debug.println( "RECEIVED: OK" );
  }
  else
  {
    debug.println( "RECEIVED: Error\nExit2" );
  }
}

void sendDebug(String cmd)
{
  debug.print("SEND: ");
  debug.println(cmd);
  Serial.println(cmd);
}

boolean connectWiFi()
{
  Serial.println("AT+CWMODE=1");//WiFi STA mode - if '3' it is both client and AP
  delay(2000);
  //Connect to Router with AT+CWJAP="SSID","Password";
  // Check if connected with AT+CWJAP?
  String cmd="AT+CWJAP=\""; // Join accespoint
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  sendDebug(cmd);
  delay(5000);
  if(Serial.find("OK"))
  {
    debug.println("RECEIVED: OK");
    return true;
  }
  else
  {
    debug.println("RECEIVED: Error");
    return false;
  }

  cmd = "AT+CIPMUX=0";// Set Single connection
  sendDebug( cmd );
  if( Serial.find( "Error") )
  {
    debug.print( "RECEIVED: Error" );
    return false;
  }
}
*/
