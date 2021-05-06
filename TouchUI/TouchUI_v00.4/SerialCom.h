char serIChar;
String serIStr = "";
boolean serIStrComplete = false;

void serialSend(String);

void serialSetup(){
  Serial.begin(9600);
  Serial.println("moin");
}

void serialEvent() {
  while (Serial.available()){
    serIChar = (char)Serial.read();
    if (serIChar == '\n'){
      serIStrComplete = true; //every Println ends with "\r\n"
    }else if (serIChar == '\r'){
    }else{
      serIStr += serIChar;
    }
  }
}

String serialProcess() {
  if (serIStrComplete){
    String ret = serIStr;
    serIStr = "";
    serIStrComplete = false;
    serialSend(ret);
    return ret;
  } else {
    return "";
  }
  return "";
}

void serialSend(String msg) {
  Serial.println(msg);
}
