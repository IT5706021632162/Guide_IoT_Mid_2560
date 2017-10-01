int ledPin = D2; 
void setup() { 
  // put your setup code here, to run once: 
 pinMode(ledPin, OUTPUT); 
} 
void loop() { 
  // put your main code here, to run repeatedly: 
  for (int dim =0 ; dim <255; dim++){ 
   analogWrite(ledPin,dim); 
   delay(5); 
  } 
   for (int dim =255 ; dim >0; dim--){ 
   analogWrite(ledPin,dim); 
   delay(5); 
  } 
} 
