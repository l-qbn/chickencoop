#define DOOR_OPEN_PIN 7
#define DOOR_CLOSE_PIN 6
#define DOOR_OPENED_SWITCH_PIN 9
#define DOOR_CLOSED_SWITCH_PIN 8

#define DEBUG_DOOR false

//DOOR STATUS three status 0 = CLOSED, 1 = OPEN, 2 = MOVING

long last_Closed;
long last_Opened;

int setup_Door(){

  pinMode(DOOR_OPEN_PIN, OUTPUT);
  pinMode(DOOR_CLOSE_PIN, OUTPUT);
  pinMode(DOOR_OPENED_SWITCH_PIN, INPUT_PULLUP);
  pinMode(DOOR_CLOSED_SWITCH_PIN, INPUT_PULLUP);
  
  return 0;
}

int get_Door_Status(){

    bool d_o = digitalRead(DOOR_OPENED_SWITCH_PIN);
    bool d_c = digitalRead(DOOR_CLOSED_SWITCH_PIN);

    if(d_o == d_c){
      return 2;
    }
    else if(d_o && !d_c){
      return 1;
    }
    else if(!d_o && d_c){
      return 0;
    }
}

String get_Door_Status_String(){

  int ds = get_Door_Status();

  DateTime td_LastClosed(last_Closed);
  DateTime td_LastOpened(last_Opened);
  
  String retString;

  if(0 == ds){
    retString += F("CLOSED");
  }else if(1 == ds){
    retString += F("OPEN");
  }else{
    retString += F("MOVING / UNKNOWN");
  }
  
  retString += F("<br>last close: ");
  retString += td_LastClosed.month();
  retString += F(".");
  retString += td_LastClosed.day();
  retString += F(". ");
  retString += td_LastClosed.hour();
  retString += F(":");
  retString += td_LastClosed.minute();
  retString += F("<br>last open: ");
  retString += td_LastOpened.month();
  retString += F(".");
  retString += td_LastOpened.day();
  retString += F(". ");
  retString += td_LastOpened.hour();
  retString += F(":");
  retString += td_LastOpened.minute();
  
  return retString;
}

String open_Door(long dt){
  
  int ds = get_Door_Status();

  switch(ds){
    case 0:
    case 2:
      last_Opened = dt;
      break;
    }

  digitalWrite(DOOR_CLOSE_PIN, LOW);
  delay(1000);
  digitalWrite(DOOR_OPEN_PIN, HIGH);

  if(DEBUG_DOOR){
    Serial.println(digitalRead(DOOR_OPEN_PIN));
    Serial.println(digitalRead(DOOR_CLOSE_PIN));
  }

  return "Opening...";
}

String close_Door(long dt){

  int ds = get_Door_Status();

  switch(ds){
    case 1:
    case 2:
      last_Closed = dt;
      break;
    }

  digitalWrite(DOOR_OPEN_PIN, LOW);
  delay(1000);
  digitalWrite(DOOR_CLOSE_PIN, HIGH);

  if(DEBUG_DOOR){
    Serial.println(digitalRead(DOOR_OPEN_PIN));
    Serial.println(digitalRead(DOOR_CLOSE_PIN));
  }

  return "Closing...";
}

