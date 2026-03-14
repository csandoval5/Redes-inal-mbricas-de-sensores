#include <RH_ASK.h>
#include <SPI.h> 

// Configuración Radio RF (Receptor en Pin 11 por defecto)
RH_ASK driver;

void setup() {
  Serial.begin(9600);
  
  if (!driver.init()) {
    Serial.println("Error al iniciar el receptor RF");
  } else {
    Serial.println("Receptor listo, esperando datos...");
  }
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  // Verificar si hay datos en el aire
  if (driver.recv(buf, &buflen)) {
    buf[buflen] = '\0'; // Cerrar la cadena de texto adecuadamente
    
    String recibido = String((char*)buf);
    
    // Procesar y separar los datos
    // Buscamos las comas para dividir el String
    int coma1 = recibido.indexOf(',');
    int coma2 = recibido.lastIndexOf(',');

    if (coma1 != -1 && coma2 != -1) {
      String temp = recibido.substring(0, coma1);
      String hum = recibido.substring(coma1 + 1, coma2);
      String luz = recibido.substring(coma2 + 1);

      // Imprimir resultados con formato limpio
      Serial.println("========================");
      Serial.print("Temperatura: "); Serial.print(temp); Serial.println(" C");
      Serial.print("Humedad:     "); Serial.print(hum);  Serial.println(" %");
      Serial.print("Luz (LDR):   "); Serial.println(luz);
      Serial.println("========================");
    } else {
      Serial.print("Mensaje recibido con formato extraño: ");
      Serial.println(recibido);
    }
  }
}