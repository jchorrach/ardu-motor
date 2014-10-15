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

volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;                               

//-----------------------------------------------------------------------------------------
//
//  Sensor de flujo+ temperatura Sea YF-S201
//
//
#define SERIESRESISTOR 10000  // the value of the 'other' resistor 10K  
 
#define THERMISTORPIN A0 // Pin analógico sensor de temperatura del YF-S102 cable verde
#define FLOWCOUNTER 2    // Pin digital sensor efecto hall del YF-S102 cable amarillo

//....................... SEND_SMS_ALARM .......................
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

 
void rpm ()     //This is the function that the interupt calls 
{ 
  NbTopsFan++;  //This function measures the rising and falling edge of the 
 
//hall effect sensors signal
} 

// The setup() method runs once, when the sketch starts
void setup() //
{ 

  Serial.begin(9600); //This is the setup function where the serial port is 

  pinMode(FLOWCOUNTER, INPUT); //initializes digital pin 2 as an input
 
  // Inicializar YF-S201
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached
  pinMode(THERMISTORPIN, INPUT);
  
} 


// the loop() method runs over and over again,
// as long as the Arduino has power
void loop ()    
{
  refrigeracion();
}

//
// Refrigeracion >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//
void refrigeracion()
{
  // Contar Hz sensor de flujo
  NbTopsFan = 0;   //Set NbTops to 0 ready for calculations

  sei();      //Enables interrupts
  delay (1000);   //Wait 1 second
  cli();      //Disable interrupts

  Calc = (NbTopsFan / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate 
  
  // Calcular temperatura sensor flujo
  float reading;
 
  reading = analogRead(THERMISTORPIN);
 
  Serial.print("Analog reading "); 
  Serial.println(reading);
 
  // convert the value to resistance
  reading = (1023 / reading)  - 1;
  reading = SERIESRESISTOR / reading;

  
  //in L/hour 
  Serial.print (Calc, DEC); //Prints the number calculated above
  Serial.print (" L/hour\r\n"); //Prints "L/hour" and returns a  new line
  Serial.print (reading, DEC); //Prints the number calculated above
  Serial.print (" Temp.\r\n"); //Prints "L/hour" and returns a  new line

}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Refrigeracion
