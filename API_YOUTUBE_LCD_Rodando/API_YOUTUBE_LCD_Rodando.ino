
#include <Wire.h> //INCLUSÃO DE BIBLIOTECA
#include <LiquidCrystal_I2C.h> //INCLUSÃO DE BIBLIOTECA
LiquidCrystal_I2C lcd(0x27, 16, 2); //FUNÇÃO DO TIPO “LiquidCrystal_I2C”
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <YoutubeApi.h>
#include <ArduinoJson.h>

char ssid[] = "INSIRA AQUI O NOME DA SUA REDE WIFI";     
char password[] = "INSIRA AQUI A SUA SENHA DO WIFI";  
#define API_KEY "INSIRA AQUI A SUA CHAVE API"  
#define CHANNEL_ID "INSIRA AQUI O ID DO SEU CANAL" 


WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long timeBetweenRequests = 30000; // TEMPO EM MILISEGUNDOS PARA A ATUALIZAÇÃO DOS DADOS.
unsigned long nextRunTime;

long subs = 0;
int Inscritos = 0;

void setup() {

  lcd.init(); // INICIALIZA O DISPLAY LCD
  lcd.backlight(); // HABILITA O BACKLIGHT (LUZ DE FUNDO)

  Serial.begin(9600);
  Serial.print("inicio ");

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  Serial.print(".");
  delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  // Required if you are using ESP8266 V2.5 or above
  client.setInsecure();

  // If you want to enable some extra debugging
  api._debug = true;
}

void loop() {

  if (millis() > nextRunTime)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Comment Count: ");
      Serial.println(api.channelStats.commentCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      Serial.println("------------------------");
      Inscritos = api.channelStats.subscriberCount;
    }
    nextRunTime = millis() + timeBetweenRequests;
    
  }   // LOGICA QUE SERÁ INSERIDA NO LCD, MODIFIQUE O QUANTO QUISER.
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("INSCR. YOUTUBE");
      lcd.setCursor(6, 1);
      lcd.print(Inscritos);
      delay(2000);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Visualizacoes :");
      lcd.setCursor(5, 1);
      lcd.print(api.channelStats.viewCount);
      delay(2000);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Quant. Videos :");
      lcd.setCursor(7, 1);
      lcd.print(api.channelStats.videoCount);
      delay(2000);
  
}
