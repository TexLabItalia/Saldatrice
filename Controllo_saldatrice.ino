//	Telwin TELMIG 170/1 TURBO
//	2022
// Modificato da TexLab Italia

unsigned int GasTimePre;    //Tutti i tempi sono in millesecondi
unsigned int GasTimePost;
unsigned int FeedTime;
unsigned int PowerTime;
unsigned long CurrentMillis;
unsigned long GasOffMillis;
unsigned long FeedOffMillis;
unsigned long PowerOnMillis;
unsigned long PoweroffMillis;
unsigned long SpotMillis;
unsigned long FrenoMillis;
unsigned long SpotFrenoMillis;



bool Spot = false;
bool trigger = false;
bool LastTrigger;
bool SpotTrig = false;
bool one = false;
bool two = false;
bool three = false;
int FrenoTime = 1000;
int previousMillis  = 0;
int currentMillis   = 0;
int valButtonOld 	= HIGH;

void setup() {
  pinMode(4, INPUT_PULLUP); // auto pulls to +5v this means all logic on this pin will be backwards
  pinMode(5, INPUT_PULLUP); // auto pulls to +5v this means all logic on this pin will be backwards
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(6, HIGH);//gas
  digitalWrite(7,HIGH);//feed
  digitalWrite(8, HIGH);//power
  digitalWrite(9, HIGH);//Freno
  Serial.begin(9600);


}

void loop() {
CurrentMillis = millis();
trigger = !digitalRead(4);
if(!trigger)//just so you arent reading the analog inputs when you don't need to
{
//Serial.println(analogRead(A3));//this is postflow time 250ms to 5000ms
//Serial.println(analogRead(A1));//this is burnback time 25ms to 275ms
//Serial.println(analogRead(A2));//this is spot time time 250ms to 5000ms
//Serial.println(analogRead(A0));//this is preflow time time 250ms to 5000ms
GasTimePre = map(analogRead(A0), 0,1024,250,5000);//all times in milliseconds
GasTimePost =  map(analogRead(A3), 0,1024,250,5000);
FeedTime =  map(analogRead(A2), 0,1024,250,5000);
PowerTime =  map(analogRead(A1), 0,1024,25,275);
Spot = !digitalRead(5);
//Serial.println(Spot);
}


if((trigger == false && trigger != LastTrigger))//on trigger released run once
{
  two = true;
  three = true;
    LastTrigger = false;
 // Serial.println("OOP");
   digitalWrite(7,HIGH);//feed 
PoweroffMillis = CurrentMillis + PowerTime;
 GasOffMillis = CurrentMillis + GasTimePost;
 FrenoMillis = CurrentMillis + FrenoTime;
}

if(trigger == true && trigger != LastTrigger)//on trigger pressed down run once
{
one = true;
  two = false;
  three = false;
    LastTrigger = true;
// Serial.println("POO");
   digitalWrite(6, LOW);//gas
  PowerOnMillis = CurrentMillis + GasTimePre;
  if(Spot)
  {
    SpotTrig = true;
    SpotMillis = CurrentMillis + GasTimePre + FeedTime;
    SpotFrenoMillis = CurrentMillis + GasTimePre + FeedTime + FrenoTime;
  }
}


if(SpotTrig && CurrentMillis > SpotMillis)
{
  two = true;
  three = true;
  SpotTrig = false;
//  Serial.println("OOP");
   digitalWrite(7,HIGH);//feed 
PoweroffMillis = CurrentMillis + PowerTime;
 GasOffMillis = CurrentMillis + GasTimePost;
 FrenoMillis = CurrentMillis + FrenoTime;
}

if (trigger == true)
if(one && CurrentMillis > PowerOnMillis)
{
  one = false;

    digitalWrite(7,LOW);//feed
  digitalWrite(8, LOW);//power
   digitalWrite(9, LOW);  //freno
    
}

if(two && CurrentMillis > PoweroffMillis)
{
  two = false;
  
    digitalWrite(8, HIGH);//power
}

if(three && CurrentMillis > GasOffMillis)
{
   three = false;
   
    digitalWrite(6, HIGH);//gas
    digitalWrite(7, HIGH);//feed
    digitalWrite(8, HIGH);//power
 
    
}
if (trigger == false)
 if (CurrentMillis > FrenoMillis)
{
   digitalWrite(9, HIGH);//freno
}
  
  if (Spot == true)
 if (CurrentMillis > SpotFrenoMillis)
{
   digitalWrite(9, HIGH);//freno
}
 
// Questa parte serve a fare in modo che se il pulsante viene premuto per meno tempo del Preflusso
// il relè del preflusso si spenga subito
int valButton = digitalRead(4);
  if(valButton==LOW && valButtonOld==HIGH){
    // Il pulsante è stato premuto
    previousMillis = millis();
  }
  
  if(valButton==HIGH && valButtonOld==LOW){
    // Il pulsante è stato rilasciato
    currentMillis  = millis();
    //Serial.print("Tempo pressione:");
    //Serial.println(currentMillis - previousMillis );
    if (currentMillis - previousMillis < GasTimePre )
      
{
   digitalWrite(6, HIGH);//gas
    digitalWrite(7, HIGH);//feed
    digitalWrite(8, HIGH);//power
   digitalWrite(9, HIGH);//freno
}
 }
 valButtonOld = valButton;
 }
