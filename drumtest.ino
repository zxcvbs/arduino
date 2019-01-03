const int PADS =5;
int pinRead;
int velocity;
int velo;

//Multiplexer
int pin_Out_S0 = 8;
int pin_Out_S1 = 9;
int pin_Out_S2 = 10;
int pin_In_Mux1 = A0;
int Mux1_State[8] = {0};

//Pad Notes
#define midichannel 1;
byte PadNote[PADS] = {60,62,64,71,72}; //Middle C = 60 
byte status1;
int  MaxPlayTime[PADS]= {90,90,90,90,90};
int  Threshold[PADS]= {90,90,90,90,90};
int threshh= 90;



//Sustain logic
boolean activePad[PADS] = {
  0,0};                   // Array of flags of pad currently playing
int PinPlayTime[PADS] = {
  0,0};                     // Counter since pad started to play

void setup() {
  // put your setup code here, to run once:
//Mulitiplexer
  pinMode(pin_Out_S0, OUTPUT);
  pinMode(pin_Out_S1, OUTPUT);
  pinMode(pin_Out_S2, OUTPUT);


Serial.begin(57600);
// pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  updateMux1();
  for(int pin = 0; pin < PADS; pin ++) {

if (Mux1_State[pin]> Threshold[pin]){

  if((activePad[pin] == false)) {
  MIDI_TX(144,PadNote[pin],Mux1_State[pin]); //note on

  
  }
   else {
      PinPlayTime[pin] = PinPlayTime[pin] + 1;
    
    }

}

else if((activePad[pin] == true)) 


    {
      PinPlayTime[pin] = PinPlayTime[pin] + 1;
      if(PinPlayTime[pin] > MaxPlayTime[pin])
      {
        activePad[pin] = false;
        MIDI_TX(144,PadNote[pin],0); 
      }
    }
}
}





void updateMux1 () {
  for (int i = 0; i < 8; i++){
    digitalWrite(pin_Out_S0, HIGH && (i & B00000001));
    digitalWrite(pin_Out_S1, HIGH && (i & B00000010));
    digitalWrite(pin_Out_S2, HIGH && (i & B00000100));
    pinRead = analogRead(pin_In_Mux1);
    if ( pinRead> 20) {
    velocity= map(pinRead,0,1023 -200,0,127); // -200 threshold individual
    //velo = map(velocity,90,127,60,127);
    
} else velocity = 0 ;

Mux1_State[i] = velocity;
  }
}


void MIDI_TX(byte MESSAGE, byte PITCH, byte VELOCITY) 
{
  status1 = MESSAGE + midichannel;
  Serial.write(status1);
  Serial.write(PITCH);
  Serial.write(VELOCITY);

}
