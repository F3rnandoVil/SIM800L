#include <SoftwareSerial.h> // Incluye la biblioteca SoftwareSerial para la comunicación serial

#define rxPin 12 // Define el pin RX como 12
#define txPin 11 // Define el pin TX como 11
SoftwareSerial Sim800L(rxPin,txPin); // Crea un objeto SoftwareSerial llamado Sim800L con los pines RX y TX especificados

String buff; // Variable para almacenar datos

void setup() {
  Serial.begin(9600); // Inicia la comunicación serial con la velocidad de 9600 bps
  
  Sim800L.begin(9600); // Inicia la comunicación serial con el módulo SIM800L a 9600 bps

  Serial.println("Iniciando..."); // Imprime un mensaje en el monitor serial
  
  Sim800L.println("AT"); // Envia el comando AT al módulo SIM800L para verificar la conexión
  esperarRespuesta(); // Llama a la función esperarRespuesta para esperar la respuesta del módulo

  Sim800L.println("ATE1"); // Configura el módulo para mostrar los comandos AT enviados
  esperarRespuesta(); // Espera la respuesta del módulo

  Sim800L.println("AT+CMGF=1"); // Configura el módulo para enviar mensajes SMS en modo texto
  esperarRespuesta(); // Espera la respuesta del módulo

  Sim800L.println("AT+CNMI=1,2,0,0,0"); // Configura el módulo para recibir notificaciones de mensajes SMS
  esperarRespuesta(); // Espera la respuesta del módulo
}

void loop() {
  while(Sim800L.available()) { // Verifica si hay datos disponibles en el módulo SIM800L
    buff = Sim800L.readString(); // Lee los datos del módulo y los guarda en la variable buff
    Serial.println(buff); // Imprime los datos en el monitor serial
  }
  while(Serial.available())  { // Verifica si hay datos disponibles en el puerto serial
    buff = Serial.readString(); // Lee los datos del puerto serial y los guarda en la variable buff
    buff.trim(); // Elimina los espacios en blanco al principio y al final de la cadena
    if(buff == "SMS") // Verifica si buff es igual a "SMS"
      send_sms(); // Llama a la función send_sms para enviar un mensaje SMS
    else if(buff == "CALL") // Verifica si buff es igual a "CALL"
      make_call(); // Llama a la función make_call para realizar una llamada
    else
      Sim800L.println(buff); // Envia los datos del puerto serial al módulo SIM800L
  }
}

void send_sms() {
  Sim800L.print("AT+CMGS=\"'Coloca aqui el nmero mas lada'\"\r"); // Envia el comando para enviar un mensaje SMS al número especificado
  esperarRespuesta(); // Espera la respuesta del módulo
  
  Sim800L.print("Hola puto TE LLEGO UN MENSAJE DESDE UN PUTO CHIP"); // Envia el contenido del mensaje SMS
  Sim800L.write(0x1A); // Envia el código de finalización del mensaje SMS
  esperarRespuesta(); // Espera la respuesta del módulo
}

void make_call() {
  Sim800L.println("ATD'Coloca aqui el nmero mas lada';"); // Envia el comando para realizar una llamada al número especificado
  esperarRespuesta(); // Espera la respuesta del módulo
}

void esperarRespuesta() {
  delay(1000); // Espera 1 segundo
  while(Sim800L.available()) { // Verifica si hay datos disponibles en el módulo SIM800L
    Serial.println(Sim800L.readString()); // Lee los datos del módulo y los imprime en el monitor serial
  }
  Sim800L.read(); // Lee un byte del módulo SIM800L
}
