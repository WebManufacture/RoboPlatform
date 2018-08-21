int enA = 10;
int in1 = 6;
int in2 = 5;
int sensVals[2];



void setup() {
 Serial.begin(9600);
 pinMode(enA, OUTPUT); 
 pinMode(in1, OUTPUT);
 pinMode(in2, OUTPUT);
 
 pinMode(8,INPUT);
 digitalWrite(8, LOW);
 pinMode(4,INPUT);
 digitalWrite(4, LOW);
}


void movingDrive() {
sensVals[0]= digitalRead(4);
sensVals[1]= digitalRead(8);


 //digitalWrite(in1, LOW);
 //digitalWrite(in2, LOW);
 //analogWrite (enA, 0);
  
for ( int i = 0; i <= 2; i++) {
  Serial.print( "s1 :");
  Serial.println (sensVals[0]);
  Serial.print( "s2 :");
  Serial.println (sensVals[1]);
 
if  (sensVals[0] ==0 && sensVals[1] == 1) {
   digitalWrite(in1, LOW);
   digitalWrite(in2, HIGH);
   analogWrite(enA, 255);
   delay(20);
   Serial.print( " moving right " );
}

else if (sensVals[0] == 1 && sensVals[1] == 0) {
   digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);
   analogWrite(enA, 255);
   delay(20);
   Serial.print( " moving left ");
}

else {
   digitalWrite(in1, LOW);
   digitalWrite(in2, LOW);
   analogWrite(enA, 255);
   delay(20);
   Serial.print( " stop moving ");
}

}

}

void loop() {
 movingDrive();
 delay(1000);
 //8 минут = 480000 мс
}



