#include <RH_ASK.h>
#include <SPI.h> // Necesaria para la comunicación interna de la librería
#include <DHT.h>

// Configuración DHT11
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Configuración LDR
const int pinLDR = A0;

// Configuración Radio RF (Transmisor en Pin 12 por defecto)
RH_ASK driver;

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  if (!driver.init()) {
    Serial.println("Error al iniciar el transmisor RF");
  } else {
    Serial.println("Transmisor listo...");
  }
}

void loop() {
  // Leer datos de los sensores
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  int luz = analogRead(pinLDR);

  // Validar lectura del DHT
  if (isnan(h) || isnan(t)) {
    Serial.println("Error al leer el sensor DHT11");
    return;
  }

  // Crear el mensaje: Formato "Temp,Hum,Luz"
  // Ejemplo: "25,60,450"
  String mensaje = String(t) + "," + String(h) + "," + String(luz);
  
  // Convertir a formato compatible con RadioHead
  const char *msg = mensaje.c_str();
  
  // Enviar datos
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  
  // Mostrar en monitor local para depuración
  Serial.print("Enviando paquete: ");
  Serial.println(mensaje);
  
  delay(2000); // Pausa de 2 segundos entre envíos
}