#include <LiquidCrystal.h>





/*
 super MIDI controller
 
 This sketch uses the serial transmit pin (pin 1) to send MIDI note data.

 
 The MIDI interface circuit:
 * digital in 1 connected to MIDI jack pin 5
 * MIDI jack pin 2 connected to ground
 * MIDI jack pin 4 connected to +5V through 220-ohm resistor
 Attach a MIDI cable to the jack, then to a MIDI synth, drum machine, or other midi device.
note: either obtain a female midi jack, or cut an existing cable and wire the cable into the board.
 created 4/12/2014
 by John Westbury

 */
 
//   keypins represent chromatic scale (c to b)
//                    C,C#,D ,D# ,E ,F,F#,G ,G#,A ,A#,B
 int keyPins[12] = { 53,51,49,47,45,43,41,39,37,35,33,31};
 int chordFormPins[8] = { 36,34,32,30,28,26,24,22};
 
 

  LiquidCrystal lcd = lcd(12,11,5,4,3,2);
 
 
 int pitch[12] = { 60,61,62,63,64,65,66,67,68,69,70,71};
 //int chLed[6] = { 8,9,10,11,12,13};
 int chCMD[6] = { 0x90,0x90,0x90,0x90,0x90,0x90};
 int currentChord[4] = {60,64,67,64};
 int currentPitch = 60;
 int currentChannelCommand =  0x90;
 static int currentBeat = -1;
 boolean chResetFlag[12] = {false,false,false,false,false,false,false,false,false,false,false,false};

int hit_c1;
int pinState = 0;
int ch1_pin=0;
int ch2_pin=1;
int ch3_pin=2;
int ch4_pin=3;
int ch5_pin=4;
int ch6_pin=5;
static unsigned int clock=100000000;
static unsigned int clkPerBeat=4550;

int tempRead=0;


static boolean MidichSelUpReset=false;
static boolean MidichSelDnReset=false;

static int chordForm=0;


void setup() {

lcd.begin(16,2);
lcd.clear();
lcd.print("ChordMate");
//pinMode(raiseMidiChPin, INPUT);
//pinMode(lowerMidiChPin, INPUT);
  
for (int j= 0;j<12;j++)
    pinMode(keyPins[j], INPUT);
    
    
for (int j= 0;j<8;j++)
    pinMode(chordFormPins[j], INPUT);    

    
    
      
  Serial1.begin(31250);
    Serial.begin(9600);

  
  

}

void loop() {
  
 

  
   
   /* Check the analog inputs and fire if appropriate      */
  
  for (int j =0;j<12;j++)
  {
    

    hit_c1 = digitalRead(keyPins[j]);    
    
  if (hit_c1 == HIGH )         
        {
          
        if(  !chResetFlag[j])
          {
              chResetFlag[j]=true;
              currentBeat=-1;       
              currentChannelCommand=0x90;//chCMD[j];
              currentPitch = pitch[j];
              chordForm=0;
       
               Serial.println("KEY");
              Serial.println(j);
          }
        }
        else
        chResetFlag[j]=false;
     
    
      
    }   
    
    for (int j =0;j<8;j++)
  {
    hit_c1 = digitalRead(chordFormPins[j]);    
    if (hit_c1 == HIGH)         
        chordForm=j;
 }   








  if (clock >= clkPerBeat)
    {
      
      noteOn(currentChannelCommand,currentChord[currentBeat],0);
      
       currentBeat += 1;
      if (currentBeat > 3)
          currentBeat =0;
          
          
      setChord();
      clock = 0;
//Serial.println("Beat");
Serial.println(currentBeat);

//      noteOn(currentChannelCommand,currentChord[currentBeat],0);  
      noteOn(currentChannelCommand,currentChord[currentBeat],127);      
      
     
  }
  clock++;
  
  
  
  
  
  
}

/*
   from arduino sample code - Note on
*/
//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are  less than 127:
void noteOn(int cmd, int pitch, int velocity) {
  Serial1.write(cmd);
  Serial1.write(pitch);
  Serial1.write(velocity);
}
void raisePitch(int channel)
{

  
   pitch[channel]++;
   
   if (pitch[channel]>0x5A) 
         pitch[channel]=0x1E;

    currentPitch= pitch[channel];
    noteOn(chCMD[channel],    pitch[channel], 0x7f);
   // delay(15);
   
}
void lowerPitch(int channel)
{

  
   pitch[channel]--;
   
   if (pitch[channel]<0x1E) 
         pitch[channel]=0x5A;

    currentPitch= pitch[channel];
    noteOn(chCMD[channel],    pitch[channel], 0x45);
   // delay(200);
   
}

void setChord(){
  
  
  //Major
       if (chordForm==0){
         currentChord[0] = currentPitch;
         currentChord[1] = currentPitch+4;
         currentChord[2] = currentPitch+7;
         currentChord[3] = currentPitch+4;
       }         
 //Minor      
       if (chordForm==1){
         currentChord[0] = currentPitch;
         currentChord[1] = currentPitch+3;
         currentChord[2] = currentPitch+7;
         currentChord[3] = currentPitch;
        }    
//Min 7       
       if (chordForm==2){
         currentChord[0] = currentPitch;
         currentChord[1] = currentPitch+3;
         currentChord[2] = currentPitch+7;
         currentChord[3] = currentPitch+10; 
       }
       
//Maj 7       
      if (chordForm==3){
         currentChord[0] = currentPitch;
         currentChord[1] = currentPitch+4;
         currentChord[2] = currentPitch+7;
         currentChord[3] = currentPitch+11;
       }    
       
//7th       
              if (chordForm==4){
         currentChord[0] = currentPitch;
         currentChord[1] = currentPitch+4;
         currentChord[2] = currentPitch+7;
         currentChord[3] = currentPitch+10;
       }         
//sus4       
       if (chordForm==5){
         currentChord[0] = currentPitch;
         currentChord[1] = currentPitch+5;
         currentChord[2] = currentPitch+7;
         currentChord[3] = currentPitch;
        }    
 
 //5th power chord      
       if (chordForm==6){
         currentChord[0] = currentPitch;
         currentChord[1] = currentPitch+7;
         currentChord[2] = currentPitch+1;
         currentChord[3] = currentPitch+7; 
       }
//6th       
      if (chordForm==7){
         currentChord[0] = currentPitch;
         currentChord[1] = currentPitch+9;
         currentChord[2] = currentPitch;
         currentChord[3] = currentPitch+9;
       }    
  
  
  
}

