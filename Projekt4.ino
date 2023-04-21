#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include<Wire.h>
#include<Ticker.h>
#include <Arduino.h> // include this header file



// zamenjajte uporabnika in geslo:
#define USERNAME "admin"
#define PASSWORD "admin"
#define PIN_LED 2
#define MODE 0 // 0 == AP, 1 == STA, 2 == AP & STA (combo)

//AP MODE SETTINGS
#define AP_SSID "dostopna_tocka_ESP"
#define AP_PASS "geslo123ESP"

//STA MODE SETTINGS
#define STA_SSID "Anze iPhone"
#define STA_PASS "eduroamjeshit"

// inicializirajte server na portu 80:
// Koda: ....
ESP8266WebServer server(80);

// globalne spremenljivke
int led_stanje = 0; // stanje LED lučke 



/* ----------------------------------------------------------------
 * is_authentified:
 */
//Check if header is present and correct
bool is_authentified(){
  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie")){
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}

/* ----------------------------------------------------------------
 * handleLogin:
 */
//login page, also called for disconnect
void handleLogin(){
  String msg;
  if (server.hasHeader("Cookie")){
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT")){
    Serial.println("Disconnection");
    server.sendHeader("Location","/login");
    server.sendHeader("Cache-Control","no-cache");
    server.sendHeader("Set-Cookie","ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")){
    if (server.arg("USERNAME") == USERNAME &&  server.arg("PASSWORD") == PASSWORD ){
      server.sendHeader("Location","/");
      server.sendHeader("Cache-Control","no-cache");
      server.sendHeader("Set-Cookie","ESPSESSIONID=1");
      server.send(301);
      Serial.println("Log in Successful");
      return;
    }
  msg = "Wrong username/password! try again.";
  Serial.println("Log in Failed");
  }
  String content = "<html><body><form action='/login' method='POST'>To log in enter user id and password:<br>";
  content += "User:<input type='text' name='USERNAME' placeholder='user name'><br>";
  content += "Password:<input type='password' name='PASSWORD' placeholder='password'><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'></form>" + msg + "<br>";
  content += "You also can go <a href='/inline'>here</a></body></html>";
  server.send(200, "text/html", content);
}

/* ----------------------------------------------------------------
 * handleRoot:
 */
//root page can be accessed only if authentification is ok
void handleRoot(){
  Serial.println("Enter handleRoot");
  String header;
  if (!is_authentified()){
    server.sendHeader("Location","/login");
    server.sendHeader("Cache-Control","no-cache");
    server.send(301);
    return;
  }
  String content = "<html><body><H2>hello, you successfully connected to esp8266!</H2><br>";
  content += "You can access this page until you <a href=\"/login?DISCONNECT=YES\">disconnect</a><br><hr />";

  /*
   * Dodajte HTML kodo, omogoča prižiganje in ugašanje vgrajene LED lučke. Vedno naj se prikaže le ena tipka, s katero lahko vklopimo ali izklopimo napajanje LED lučke.
   * Namig: v funkciji handleLogin() se nahaja HTML koda primera metode POST, kjer s klikom na tipko pošljemo uporabniško ime in geslo.
   */
  // Koda: ....

  server.send(200, "text/html", content);
}


/* ----------------------------------------------------------------
 * handleLED:
 */
void handleLED(){
  /*
   * Dodajte kodo za krmiljenje vgrajene LED lučke.
   * Po nastavitvi novega stanja vgrajene LED luči poskrbite, da se vrnete na glavno stran, kjer se mora nato prikazati ustrezna tipka za krmiljenje LED lučke
   * Namig: v funkciji handleRoot() se nahaja del kode, ki uporabnika v primeru, da le ta ni prijavljen, preusmeri na stran za avtentikacijo.
   */
  // Koda: ....
  
}

/* ----------------------------------------------------------------
 * handleNotFound:
 */
//no need authentification
void handleNotFound(){
  /*
   * Dodajte ustrezno HTML kodo, ki bo uporabnika obvestila, da strani ni bilo mogoče najti.
   * Omogočite tudi, da se uporabnik lahko vrne na glavno stran.
   */
  String vsebina = ""; // dopišite ustrezno HTML kodo
  server.send(404, "text/html", vsebina);
}

void setupWiFiAP(){
  /*
   * Dodajte ustrezno kodo za inicializacijo WiFi modula v načinu AP.
   * Omogočite tudi, da se uporabnik lahko vrne na glavno stran.
   */
  // Koda: ....

  //set wifi mode AP
  WiFi.mode(WIFI_AP);

  //set AP SSID and password
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(500);
  // print info
  Serial.print("Access Point: SSID = ");
  Serial.print(AP_SSID);
  Serial.print(", Password = ");
  Serial.println(AP_PASS);

    
}

void setupWiFiSTA(){
  /*
   * Dodajte ustrezno kodo za inicializacijo WiFi modula v načinu STA.
   * Omogočite tudi, da se uporabnik lahko vrne na glavno stran.
   */
  // Koda: ....

  //set wifi mode STA
  WiFi.mode(WIFI_STA);

  // Connect to WiFi network with SSID and password
  WiFi.begin(STA_SSID, STA_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with ssid: ");
  Serial.println(STA_SSID);
}



/* ----------------------------------------------------------------
 * setup:
 */
void setup() {
  // put your setup code here, to run once:
  /*
  * Dodajte ustrezno kodo za inicializacijo.
  */
  
  Serial.begin(115200);
  Serial.println("");
  
  pinMode(PIN_LED, OUTPUT);

  #if MODE == 0
    setupWiFiAP();
  #else if MODE == 1
    setupWiFiSTA();
  #endif

  //**** nastavitve strežnika:
  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/led", handleLED);
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works without need of authentification");
  });
  server.onNotFound(handleNotFound);
  
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent","Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize );
  
  // zaženemo strežnik:
  server.begin();

  Serial.println("HTTP server started");
}

/* ----------------------------------------------------------------
 * loop:
 */
void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
