/*********************************************************
**
**  ARDU-MOTOR
**  sketch monitorización del motor del barco con arduino
**
**   Modelo testeado YANMAR 1GM
**
**  -Revisa flujo de agua refrigeración
**  -Temperatura motro
**  -RPM
**  -Carga alternador
**
*********************************************************

Mapa de pines:
------------------------------------
00: Rx Serial
01: Tx Serial
02:<Contador sensor de flujo [FLOWCOUNTER] interrup
03:<
04:
05:
06:
07: 
08: 
09: 
10:
11: 
12: 
13: 
A0: Temperatura sensor flujo [THERMISTORPIN] in
A1: 
A2:
A3:
A4: SDA para LCD
A5: SCL para LCD

*/

volatile int nPulsos; // Contador general de pulsos
int Calc;             // Variable general de cálculo

//-----------------------------------------------------------------------------------------
//
//  Sensor de flujo+ temperatura Sea YF-S201
//
//
#define SERIESRESISTOR 10000  // the value of the 'other' resistor 10K  
 
#define THERMISTORPIN A0 // Pin analógico sensor de temperatura del YF-S102 cable verde
#define FLOWCOUNTER 2    // Pin digital sensor efecto hall del YF-S102 cable amarillo

//....................... ALARMAS .......................
byte alarmas = 0; // Indicador de alarmas detectadas (bitmap)
//
//  00000000
//  |||||||+--- bit(1): si 1 Alarma flujo agua infuficiente
//  ||||||+-----bit(2): si 1 Alarma temperatura motor elevada
//  |||||+------bit(3): 
//  ||||+-------bit(4): 
//  |||+--------bit(5): 
//
//..............................................................

 
//----------------------------------------------------
//
// subrutina para contar pulsos
//
void rpm () 
{ 
  nPulsos++;  // Incrementa contador de pulsos
} 

//----------------------------------------------------
//
// SETUP
//
void setup()
{ 

  Serial.begin(9600); // Debug

  
  // Inicializar sensores YF-S201
  pinMode(FLOWCOUNTER, INPUT); 
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached
  pinMode(THERMISTORPIN, INPUT);
  
} 


//-----------------------------------------------------
//
// LOOP
//
void loop ()    
{
  refrigeracion(); // Monitorizacion refrigeración
}

//
// Refrigeracion >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//
void refrigeracion()
{
  // Contar Hz sensor de flujo
  nPulsos = 0;   // A 0 contador de pulsos

  sei();         // Habilita interrupciones
  delay (1000);  // Muestreo interrupción 1 segundo
  cli();         // Deshabilita interrupciones

  Calc = (NbTopsFan / 7.5); // Pulsos / 7.5Q, = l/min. 
  
  // Calcular temperatura sensor flujo
  float reading;
 
  reading = analogRead(THERMISTORPIN);
 
  Serial.print("Analog reading "); 
  Serial.println(reading);
 
  // Convertir el valor a resistencia en Ohms
  reading = (1023 / reading)  - 1;
  reading = SERIESRESISTOR / reading;
  
  //--> TO DO convertir valor Ohms a Temperatura

  
  
  Serial.print (Calc, DEC); 
  Serial.print (" L/hour\r\n");
  Serial.print (reading, DEC); 
  Serial.print (" Temp.\r\n"); 

}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Refrigeracion
