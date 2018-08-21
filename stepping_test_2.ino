int enA = 10;
int enB = 9;
int in1A = 6;
int in1B = 5;
int in2A = 4;
int in2B = 3;
int sensVals[4];



void setup() {
 Serial.begin(9600);
 pinMode(enA, OUTPUT); 
 pinMode(in1A, OUTPUT);
 pinMode(in2B, OUTPUT);
 pinMode(enB, OUTPUT); 
 pinMode(in2A, OUTPUT);
 pinMode(in2B, OUTPUT);
 
 pinMode(8,INPUT);
 digitalWrite(8, LOW);
 pinMode(4,INPUT);
 digitalWrite(4, LOW);
 
 pinMode(2,INPUT);
 digitalWrite(2, LOW);
 pinMode(11,INPUT);
 digitalWrite(11, LOW);
}


void movingDrive() {
sensVals[0]= digitalRead(4);
sensVals[1]= digitalRead(8);
sensVals[2]= digitalRead(2);
sensVals[3]= digitalRead(1);


  
for ( int i = 0; i <= 2; i++) {
  Serial.print( "s1 :");
  Serial.println (sensVals[0]);
  Serial.print( "s2 :");
  Serial.println (sensVals[1]);
  Serial.print( "s3 :");
  Serial.println (sensVals[2]);
  Serial.print( "s4 :");
  Serial.println (sensVals[3]);
 
if  (sensVals[0] ==0 && sensVals[1] == 1) {
   digitalWrite(in1A, LOW);
   digitalWrite(in1B, HIGH);
   analogWrite(enA, 255);
   delay(20);
   Serial.print( " moving up " );
}

else if (sensVals[0] == 1 && sensVals[1] == 0) {
   digitalWrite(in1A, HIGH);
   digitalWrite(in1B, LOW);
   analogWrite(enA, 255);
   delay(20);
   Serial.print( " moving down ");
}
else if (sensVals[2] == 1 && sensVals[3] == 0) {
   digitalWrite(in2A, HIGH);
   digitalWrite(in2B, LOW);
   analogWrite(enB, 255);
   delay(20);
   Serial.print( " moving right ");
}
else if (sensVals[2] == 0 && sensVals[3] == 1) {
   digitalWrite(in2A, LOW);
   digitalWrite(in2B, HIGH);
   analogWrite(enB, 255);
   delay(20);
   Serial.print( " moving left ");
}

else {
   digitalWrite(in1A, LOW);
   digitalWrite(in1B, LOW);
   analogWrite(enA, 255);
   digitalWrite(in2A, LOW);
   digitalWrite(in2B, LOW);
   analogWrite(enB, 255);
   delay(20);
   Serial.print( " stop moving ");
}

}

}

void loop() {
 movingDrive();
 delay(200);
}



