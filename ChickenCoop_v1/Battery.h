#define BATTERY_PIN A7

String get_Voltage_Battery_String(){

  long val = 0.0;

  for(int i = 0; i < 10; i++){
    val += analogRead(BATTERY_PIN);
    delay(2);
  }

  int sensorValue = val / 10;
  
  float voltage = sensorValue * (5.02 / 1023.0);

  float gesamt = ((4.7 * 4.0) * voltage) / 4.7;

  String sRet = F("Battery: ");
  sRet += (String)gesamt;
  sRet += F(" V");
  
  return sRet;
}
