//(c) digibird1 2016

//Control a 74HC595 IC

const int SER=7; //pin of the serial data line
const int SRCLK=5; //pin of the serial clock
const int RCLK=6; //pin of the clock whcih shifts the serial register into the data register

char incomingByte;
char Data[10];
int index;
String myString=""; 

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  pinMode(SER, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  
  digitalWrite(SER,LOW);
  digitalWrite(SRCLK,LOW);
  digitalWrite(RCLK,LOW);
}

/*This function shifts in the input byte into the shift register
*/
void shiftIn(byte InByte){
  Serial.print("Shift In: ");
   //shift into the serial register
 
  for(int i=0;i<8;i++){

/*    if(InByte & 1<<i){
      digitalWrite(SER,HIGH);
      Serial.print("1");
    }
    else{
      digitalWrite(SER,LOW);
      Serial.print("0");
    }*/
    digitalWrite(SER,InByte & 1<<7-i);
    delay(1);
    digitalWrite(SRCLK,HIGH);
    delay(1);
    digitalWrite(SRCLK,LOW);
    delay(1);
  }
delay(1000);
  
  //copy serial into data register
  digitalWrite(RCLK,HIGH);
  delay(1);
  digitalWrite(RCLK,LOW);
  delay(1);

  
  Serial.println(" done");
}

/* This function converts a string of eight '1' and '0' into a byte 
  It returns -1 if an error occures adn 0 if no error
*/
int convertStringToByte(String ByteString, byte &OutByte){
  Serial.println(ByteString.length());
  Serial.println(ByteString);
  if(ByteString.length()!=8){
    Serial.println("String != 8");
    return -1;
  }
  
  OutByte=0; 
  for(int i=0;i<8;i++){
    if(ByteString[i]=='1')OutByte=OutByte | 1<<7-i;
    else if(ByteString[i]=='0')OutByte=OutByte;
    else{
       Serial.println("String contains wrong character");
       return -1;
    }
  }
  return 0;
}

// the loop routine runs over and over again forever:
void loop() {
  

    while (Serial.available() > 0) {

      // read the incoming byte:
      incomingByte = Serial.read();
      Serial.println(incomingByte);
      if(incomingByte=='\n'){
        myString=String(Data);
        myString=myString.substring(0, myString.length()-1);
        Serial.println(myString);
        byte OutByte;
        if(convertStringToByte(myString,OutByte)==0){
          shiftIn(OutByte);
          //shiftIn(OutByte);//Write twice when the relay turns on some bits are not correct
          Serial.println((char)OutByte);
        }
        index=0;
      }
      else{
        Data[index]=incomingByte;
        index++;
        Data[index]='\0';//Terminate the char array
      }


 
  }

}
