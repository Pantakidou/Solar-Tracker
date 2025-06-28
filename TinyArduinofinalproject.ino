const int pin_servo_base = 4; 
const int pin_servo_top = 5;
const int pin_sensor_TL = A1;
const int pin_sensor_TR = A0;
const int pin_sensor_BL = A3;
const int pin_sensor_BR = A2;

int max_TL = 0;
int min_TL = 1000000;

int threshold_TL = 500;

int max_TR = 0;
int min_TR = 1000000;

int threshold_TR = 500;

int max_BL = 0;
int min_BL = 1000000;

int threshold_BL = 500;

int max_BR = 0;
int min_BR = 1000000;

int threshold_BR = 500;

int angle_servo_base = 90;
int angle_servo_top = 90;


void setup() {
  // put your setup code here, to run once:
pinMode(pin_sensor_BL, INPUT);
pinMode(pin_sensor_BR, INPUT);
pinMode(pin_sensor_TL, INPUT);
pinMode(pin_sensor_TR, INPUT);

pinMode(pin_servo_base, OUTPUT);
pinMode(pin_servo_top, OUTPUT);

calibrate_sensors();

}

void loop() {
  int valTL = analogRead(pin_sensor_TL);
  int valTR = analogRead(pin_sensor_TR);
  int valBL = analogRead(pin_sensor_BL);
  int valBR = analogRead(pin_sensor_BR);

  int VerticalUp = (valTL + valTR)/2;
  int VerticalDown = (valBL + valBR)/2;

  int HoriLeft = (valTL + valBL) / 2;
  int HoriRight = (valTR + valBR) / 2;


//Control Law:
if ( VerticalUp > (threshold_TL + threshold_TR) / 2 && VerticalDown < (threshold_BR + threshold_BL) / 2 ) 
{ angle_servo_top = angle_servo_top-1; }

else if ( VerticalUp < (threshold_TL + threshold_TR) / 2 && VerticalDown > (threshold_BR + threshold_BL) / 2  )
{ angle_servo_top = angle_servo_top+1; }

else if ( HoriLeft > (threshold_TL + threshold_BL) / 2 && HoriRight < (threshold_TR + threshold_BR) / 2 ) 
{ angle_servo_base = angle_servo_base+1; }

else if ( HoriLeft < (threshold_TL + threshold_BL) / 2 && HoriRight < (threshold_TR + threshold_BR) / 2 )
{ angle_servo_base = angle_servo_top-1; }

//Saturation:
if(angle_servo_base > 180 )
{ angle_servo_base = 180; }

if(angle_servo_base < 0)
{ angle_servo_base = 0;   }

if(angle_servo_top > 180 )
{ angle_servo_top = 180; }

if(angle_servo_top < 0)
{ angle_servo_top = 0;   }

servo_write(pin_servo_base, angle_servo_base);
servo_write(pin_servo_top, angle_servo_top);
}

void calibrate_sensors(){
  long t1 = millis();
  while(millis() - t1 <= 10000){
    int v_TL = analogRead(pin_sensor_TL);
     if (v_TL > max_TL)
     { max_TL = v_TL;  }
     if (v_TL < min_TL)
     { min_TL = v_TL;  }
  
   int v_TR = analogRead(pin_sensor_TR);
     if (v_TR > max_TR)
     { max_TR = v_TR; }
     if (v_TR < min_TR)
     { min_TR = v_TR; }

   int v_BL = analogRead(pin_sensor_BL);
    if (v_BL > max_BL)
    { max_BL = v_BL; }
    if (v_BL < min_BL)
    { min_BL = v_BL; }

   int v_BR = analogRead(pin_sensor_BR);
    if (v_BR > max_BR) 
    { max_BR = v_BR; }
    if (v_BR < min_BR)
    { min_BR = v_BR; } 
  }

  threshold_TL = (max_TL + min_TL)/2;
  threshold_TR = (max_TR + min_TR)/2;
  threshold_BL = (max_BL + min_BL)/2;
  threshold_BR = (max_BR + min_BR)/2;
}

void servo_write(int pin, int angle){
  int makis = map(angle,0,180,1000,2000);
  digitalWrite(pin, HIGH);
  delayMicroseconds(makis);
  digitalWrite(pin, LOW);
  delayMicroseconds(20000 - makis);
}
