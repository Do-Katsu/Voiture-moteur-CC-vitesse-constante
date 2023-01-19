#define pulse_per_revolutions 20
int capteur = 2;
int frequence; 
float vitesse; 
int moteur_pos=11;
int enable = 9;
int duty_cycle = 127;

int holes = 20;

float depass,risingTime,settelingTime,depassTime;
float RPM;

float cond=0;

float rpm1;
float rpm2;
float time1=millis();
float time2;
int calcRising=0;
int calcDepass=0;
int calcSetteling=0;
int rpmMax=0;

unsigned long start_time = 0;
unsigned long end_time = 0;
int steps = 0;
float steps_old=0;
float temp = 0;
float rps = 0;
float average =0;
float somme = 0;
float no=0;
float design_rpm=150; //design_vit/  pi d

unsigned long startat = millis();
int dela = 5;

float erreur=design_rpm;

const float Kp=1;
const float Ki=0;
const float Kd=0;

float erreur_last=erreur;
float erreur_somme= 0;
float erreur_delta=0;

float correction;

/*This part is all initializing by assigning pins*/
void setup()
{
Serial.begin (9600);

pinMode(9, OUTPUT);           // The pin of the ENABLE connected to the L293d circuit
pinMode(moteur_pos, OUTPUT);  // The pin that's connected to the motor through the L293d circuit
pinMode(2, INPUT_PULLUP);     // The pin connected to the optical sensor

  delay(1000);                // we apply a delay of 1 second to be able to see the motor starting
}

/*We start the loop here*/
void loop()
{
  /*First we start by commanding the motor*/
analogWrite (9, duty_cycle);    // the command of the motor depending on the value of duty_cycle
digitalWrite (11, HIGH);        // a digital 1 applied to the motor

/*Second part of the code where we interrupt the pin 2 (sensor) and put a counter to it*/
start_time=millis();
end_time=start_time+1000;       // a delay between the calculation of the wheel's frequency of 1 second
while(millis()<end_time)
{
  if(digitalRead(capteur))      // whenever the sensor sends a (1) this condition is met
  {
    steps++;                    // counter
    while(digitalRead(capteur));// the program stays here until the sensor stops sending a (1) signal
  }
}
/*In this part, we calculate the angular velocity*/
 rpm1=RPM;                      // this variable is for calculating the overshoot of the system                 
 temp=steps-steps_old;          // the difference between the counter in this iteration and the one before it
 steps_old=steps;               
 rps=(temp/holes);
 RPM = rps*60;                  // calculation of angular velocity
 rpm2=RPM;
 
  no++;
  somme+=RPM;
  average=somme/no;             // calculation of the average over time

  Serial.print("\nRPM: ");
  Serial.print(RPM);
  Serial.print("\tAverage: ");
  Serial.print(average);
  Serial.print("\tInput: ");
  Serial.print(duty_cycle);
  Serial.print("\tErreur: ");
  Serial.print(erreur);
  /*After calculatin the velocity, we apply the PID correction*/
  pid();
  duty_cycle += correction;
  /*We also apply a limiter/saturation in case of huge correction values*/
  limiter();

  /*In this part, we try to calculate different parameters of the system such as "overshoot", "rising time" and "setteling time"*/
 time2=millis();
 designParameters();
 time1=millis();
 if (calcRising==1)   Serial.print("\t Rising at: ");   Serial.print(risingTime);
 if (calcDepass>=9) 
 {
  Serial.print("\t Overshoot: ");   Serial.print(depass);
  Serial.print("\t Overshoot Time: ");   Serial.print(depassTime);
 }

  if (calcSetteling>=9) 
 {
    Serial.print("\t Setteling Time: ");   Serial.print(settelingTime);
 }

  
}
/*This function is used to calculate different parameters*/
void designParameters()
{
 if (calcRising==0)
 {
 if(RPM>0.90*design_rpm)
 {
    risingTime = ((time2-time1)/(rpm2-rpm1)*(0.90*design_rpm - rpm1) + time1) / 1000;
    calcRising=1;
 }
 }

   if(calcDepass<10)
   {
     rpmMax=max(RPM,rpmMax);
     if(rpmMax==RPM) depassTime=time2/1000;
    if (RPM<rpmMax) calcDepass++;
    depass = rpmMax;
   }
   
   calcSetteling++;
   if(abs(erreur/design_rpm)>=0.05) { settelingTime=time2/1000; calcSetteling=0;}
  
}
/*This function is used make the PID correction*/
void pid(){
  erreur = design_rpm - RPM;
  erreur_somme+=erreur;
  erreur_delta = erreur - erreur_last;
  erreur_last = erreur;
 // if ( abs(erreur)/design_rpm < 0.05 ) fprintf("rising time = %f",time);
  correction = Kp*erreur + Ki* erreur_somme + Kd* erreur_delta;
}
/*This function is used to limit the value of duty_cycle between 255 and 0*/
void limiter(){
if (duty_cycle > 255) duty_cycle = 255;
else if (duty_cycle < 0) duty_cycle = 0;

//correction = min(correction,255);
//correction = max(correction,0);

}
