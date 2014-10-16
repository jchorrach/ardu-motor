/*********************************************************
**
**  ARDU-MOTOR
**  sketch monitorización del motor del barco con arduino
**
**   Modelo testeado YANMAR 1GM
**
**  -Revisa flujo de agua refrigeración
**  -Temperatura motor
**  -RPM
**  -Carga alternador
**
*********************************************************

Mapa de pines:
------------------------------------
00: Rx Serial
01: Tx Serial
02:<Contador sensor de flujo [FLOWCOUNTER] interrup
03:<Contador sensor de hall rpm motor [RPMCOUNTER] interrup
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

volatile int nPulsos0; // Contador general de pulsos flujo
volatile int nPulsos1; // Contador general de pulsos motor

int Calc;             // Variable general de cálculo

//-----------------------------------------------------------------------------------------
//
//  Sensor de flujo+ temperatura Sea YF-S201
//
//
#define SERIESRESISTOR 10000  // the value of the 'other' resistor 10K  
 
#define THERMISTORPIN A0 // Pin analógico sensor de temperatura del YF-S102 cable verde
#define FLOWCOUNTER 2    // Pin digital sensor efecto hall del YF-S102 cable amarillo

//-----------------------------------------------------------------------------------------
//
//  Sensor hall para RPM motor
//
//
#define RPMCOUNTER 3    // Pin digital sensor hall para contar rpm motor

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
// subrutinas para contar pulsos
//
void rpm0 () 
{ 
  nPulsos0++;  // Incrementa contador de pulsos
} 

void rpm1 () 
{ 
  nPulsos1++;  // Incrementa contador de pulsos
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
  attachInterrupt(0, rpm0, RISING); 
  pinMode(THERMISTORPIN, INPUT);
  
  // Inicializar sensor hall RPM
  pinMode(RPMCOUNTER, INPUT); 
  attachInterrupt(0, rpm1, RISING); 
  
} 


//-----------------------------------------------------
//
// LOOP
//
void loop ()    
{
  
  nPulsos0 = 0;   // A 0 contador de pulsos flujo
  nPulsos1 = 0;   // A 0 contador de pulsos rpm

  sei();         // Habilita interrupciones
  delay (1000);  // Muestreo interrupción 1 segundo
  cli();         // Deshabilita interrupciones

  refrigeracion(); // Monitorizacion refrigeración
}

//
// Refrigeracion >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//
void refrigeracion()
{


  Calc = nPulsos0 / 7.5; //-> Pulsos/se (hz) / 7.5 = Q l/min. 
  
  // Calcular temperatura sensor flujo
  float leeAnalog;
 
  leeAnalog = analogRead(THERMISTORPIN);
 
  Serial.print("Analog reading "); 
  Serial.println(reading);
 
  // Convertir el valor a resistencia en Ohms
  leeAnalog = (1023 / leeAnalog)  - 1;
  leeAnalog = SERIESRESISTOR / leeAnalog;
  
  //--> TO DO convertir valor leeAnalog de Ohms a Temperatura

  
  
  Serial.print (Calc, DEC); 
  Serial.print (" L/min\r\n");
  Serial.print (leeAnalog, DEC); 
  Serial.print (" Temp.\r\n"); 

}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Refrigeracion
