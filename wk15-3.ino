int loopDelay=20;
int tones[] = {
  0, 185, 207, 233, 247, 277, 311, 349, 370
};
int len=0;
int melody[128][2]={};

void Play(){
  int noteDuration=0;
  int pauseBetweenNotes=0;
	for (int i = 0; i < len; i++) {
   	  noteDuration = melody[i][1]*loopDelay*1.5;
      tone(10, melody[i][0], noteDuration);
      pauseBetweenNotes = noteDuration+5*loopDelay;
      delay(pauseBetweenNotes);
      noTone(10);
    }
}

byte recordBtnCurrentState=1;
byte recordBtnLastState=1;

byte buttonState;
byte recordIndex=0;
bool recording=false;
void setup() {
  for(byte i=2;i<10;i++)pinMode(i,INPUT_PULLUP);
  
  pinMode(11,OUTPUT);
  pinMode(12,INPUT_PULLUP);
  
  Serial.begin(9600);
}

void loop() {
  if(recordIndex>=127){
    recording=false;
    len=128;
  }

  int btns=0;
  for(byte i=2;i<10;i++){
    btns += buttonState = digitalRead(i);
      
    if(buttonState==1)continue;
    tone(10, tones[i-1], 200);
    if(recording){
      if(melody[recordIndex][0]==tones[i-1]){
      	melody[recordIndex][1]++;
      }else{
        if(recordIndex!=0 || melody[recordIndex][0]!=0) recordIndex++;
        melody[recordIndex][0]=tones[i-1];
        melody[recordIndex][1]=0;
      }
    }
  }
  if(btns==8 && recordIndex !=0){
    if(melody[recordIndex][0]==0){
      melody[recordIndex][1]++;
    }else{
      recordIndex++;
      melody[recordIndex][0]=0;
      melody[recordIndex][1]=0;
    }
  }
    

  
  
  recordBtnLastState = recordBtnCurrentState;
  recordBtnCurrentState = digitalRead(12);
  if(recordBtnLastState==0&&recordBtnCurrentState ==1){
    recording =!recording;
	digitalWrite(11,recording);
    if(recording){
      recordIndex=0;
      melody[recordIndex][0]=0;
    }else{
      len=recordIndex+1;
      noTone(10);
      Play();
    }
  }
  digitalWrite(11,recording);

  delay(loopDelay);
}
