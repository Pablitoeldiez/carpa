#include <Wire.h>
#include "RTClib.h"
 
// RTC_DS1307 rtc;
RTC_DS3231 rtc;
 
String daysOfTheWeek[7] = { "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado" };
String monthsNames[12] = { "Enero", "Febrero", "Marzo", "Abril", "Mayo",  "Junio", "Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre" };
int cooler = 8, sonido = 7, lampara = 2;
bool state = false, estadoFoco = false;
 
void setup() {
   Serial.begin(9600);
   delay(1000); 
   // Declaro la salida
   pinMode(cooler, OUTPUT);
   pinMode(lampara, OUTPUT);
   // Declaro la buzzer
   pinMode(sonido, OUTPUT);
   if (!rtc.begin()) {
      Serial.println(F("Couldn't find RTC"));
      while (1);
   }
 
   // Si se ha perdido la corriente, fijar fecha y hora
   if (rtc.lostPower()) {
      // Fijar a fecha y hora de compilacion
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      
      // Fijar a fecha y hora específica. En el ejemplo, 21 de Enero de 2016 a las 03:00:00
      // rtc.adjust(DateTime(2016, 1, 21, 3, 0, 0));
   }
}
 
void printDate(DateTime date)
{
   Serial.print(date.year(), DEC);
   Serial.print('/');
   Serial.print(date.month(), DEC);
   Serial.print('/');
   Serial.print(date.day(), DEC);
   Serial.print(" (");
   Serial.print(daysOfTheWeek[date.dayOfTheWeek()]);
   Serial.print(") ");
   Serial.print(date.hour(), DEC);
   Serial.print(':');
   Serial.print(date.minute(), DEC);
   Serial.print(':');
   Serial.print(date.second(), DEC);
   Serial.println();
}
bool isScheduledON(DateTime date)
{
   int weekDay = date.dayOfTheWeek();
   float minutes = date.minute();
   if ((minutes >= 00 && minutes <= 15) || (minutes >= 30 && minutes <= 45) )
   {
      return true;
   }
   return false;
}
bool foco(DateTime date){
  float hora = date.hour();
  Serial.print(hora);
  if(hora >= 0 && hora <= 6)
  {
    return true;
  } 
  return false;
}
void sonidito(){
  digitalWrite(sonido, HIGH);
  delay(100);
  digitalWrite(sonido, LOW);
  }
void loop() {
   // Obtener fecha actual y mostrar por Serial
   DateTime now = rtc.now();
   printDate(now);
      if (state == false && !isScheduledON(now))      // Apagado y debería estar encendido
   {
      digitalWrite(cooler, HIGH);
      sonidito();
      state = true;
      Serial.print("Activado");
   }
   else if (state == true && isScheduledON(now))  // Encendido y deberia estar apagado
   {
      digitalWrite(cooler, LOW);
      sonidito();
      state = false;
      Serial.print("Desactivar");
   }
   /////// Foco ///////////
      if(estadoFoco == false && !foco(now))
   {
      digitalWrite(lampara, HIGH);
      sonidito();
      estadoFoco = true;
   }
   else if (estadoFoco == true && foco(now))
   {
      digitalWrite(lampara, LOW);
      sonidito();
      estadoFoco = false;
   }
   
   
   delay(3000);
}
