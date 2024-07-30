#include <TinyGPS.h> // Librería para obtener datos del GPS

TinyGPS gps;
HardwareSerial ss(1); // Utilizamos el segundo puerto serial hardware del ESP32

void setup()
{
  Serial.begin(9600); // Velocidad del puerto serial principal del ESP32
  ss.begin(9600, SERIAL_8N1, 17, 16); // Velocidad del módulo GPS y pines TX y RX
  Serial.println("Iniciando envío de coordenadas con TinyGPS v. "); 
  Serial.println(TinyGPS::library_version());
}

void loop()
{
  bool nuevosDatos = false;
  unsigned long caracteres;
  unsigned short sentencias, fallas;

  // Analizamos si hay datos del módulo GPS cada 5 segundos
  for (unsigned long start = millis(); millis() - start < 5000;)
  {
    while (ss.available()) // Se cumple mientras hay datos disponibles desde el GPS
    {
      char c = ss.read();
      if (gps.encode(c)) // Si hay datos válidos
        nuevosDatos = true; // Asignamos un valor "true" a la variable
    }
  }

  if (nuevosDatos)
  {
    float flat, flon; // Variables para almacenar la latitud y longitud
    gps.f_get_position(&flat, &flon);
    Serial.print("https://maps.google.com/maps?q="); // Formato url de google maps
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6); // Obtenemos la latitud
    Serial.print(",");
    Serial.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6); // Obtenemos la longitud
  }

  // En el caso de que exista una mala conexión con el GPS, en el cableado, nos enviará una alerta  
  gps.stats(&caracteres, &sentencias, &fallas);
  if (caracteres == 0)
    Serial.println("*No se han recibido caracteres del GPS: compruebe el cableado*");
}
