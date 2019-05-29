#include <Wire.h>
#include <Servo.h> 
#include <math.h>

struct MeasurmentTime {
  double ElapsedTime;
  double Time;
  double TimePrev;
};

struct PID {
  double PwmLeft;
  double PwmRight;
  double Error;
  double PreviousError;
  double PID;
  double PID_P = 0;
  double PID_I = 0;
  double PID_D = 0;
  double KP = 1.95; // 3.55
  double KI = 0; // 0.005
  double KD = 0.01; // 2.05
};

class ADXL345 {
public:
  void Initialising();

  void Measurment();

  double RollFiltred();

  double PitchFiltred();

  double RollDx();

  double PitchDy();
  
  void LowPassFilter();
  
private:
  int adxl345_ = 0x53;
  double xOut_, yOut_, zOut_;
  double roll_, pitch_, rollFiltred_, pitchFiltred_=0;
  double dx_, dy_;
};

class ESC {
public:
  void Init(int pin);

  void ESCWriteMicroseconds(double frequency);

  void Frequency(double frequency);

  double Frequency();
private:
  int pin_;
  double frequency_;
  Servo esc_;
};

void ESC::Init(int pin) {
  pin_ = pin;
  esc_.attach(pin_);
}

void ESC::ESCWriteMicroseconds(double frequency) {
  esc_.writeMicroseconds(frequency);
}

void ESC::Frequency(double frequency){
  frequency_ = frequency;
}

double ESC::Frequency() {
  return frequency_;
}

void ADXL345::Initialising() {
  Wire.begin();
  
  Wire.beginTransmission(adxl345_);
  Wire.write(0x2D); 
  
  Wire.write(8);
  Wire.endTransmission();
  delay(10);
}

void ADXL345::Measurment() {
  Wire.beginTransmission(adxl345_);
  Wire.write(0x32);
  Wire.endTransmission(false);
  Wire.requestFrom(adxl345_, 6, true);
  xOut_ = ( Wire.read() | Wire.read() << 8);
  xOut_ = xOut_ / 256; 
  yOut_ = ( Wire.read() | Wire.read() << 8);
  yOut_ = yOut_ / 256;
  zOut_ = ( Wire.read() | Wire.read() << 8); 
  zOut_ = zOut_ / 256;

  roll_ = atan(yOut_ / sqrt(pow(xOut_, 2) + pow(zOut_, 2))) * 180 / PI;
  pitch_ = atan(-1 * xOut_ / sqrt(pow(yOut_, 2) + pow(zOut_, 2))) * 180 / PI;

  LowPassFilter();
}

void ADXL345::LowPassFilter() {
  rollFiltred_ = 0.94 * rollFiltred_ + 0.06 * roll_;
  pitchFiltred_ = 0.94 * pitchFiltred_ + 0.06 * pitch_;
}

double ADXL345::RollFiltred() {
  return rollFiltred_;
}

double ADXL345::PitchFiltred() {
  return pitchFiltred_;
}

double ADXL345::RollDx() {
  return dx_;
}

double ADXL345::PitchDy() {
  return dy_;
}

ADXL345 adxl345;
ESC escMotors[4];
PID pid;
MeasurmentTime measurmentTime;

double desiredAngle = 0;
double throttle = 1500;

void setup() {
  Serial.begin(250000);
  
  adxl345.Initialising();

  escMotors[0].Init(7);
  escMotors[1].Init(8);
  escMotors[2].Init(12);
  escMotors[3].Init(13);

  measurmentTime.Time = millis();

  for(int i = 0; i < 4; i++ ) {
    escMotors[i].ESCWriteMicroseconds(2000);
  }
  delay(3000);
  for(int i = 0; i < 4; i++ ) {
    escMotors[i].ESCWriteMicroseconds(1000);
  }
  delay(7000);
}

void loop() {
  measurmentTime.TimePrev = measurmentTime.Time;
  measurmentTime.Time = millis();
  measurmentTime.ElapsedTime = (measurmentTime.Time - measurmentTime.TimePrev) / 1000;

  adxl345.Measurment();

  pid.Error = adxl345.PitchFiltred() - desiredAngle;
  
  pid.PID_P = pid.Error * pid.KP;
  if(-3 < pid.Error < 3) {
    pid.PID_I = pid.PID_I +(pid.KI * pid.Error);  
  }
  pid.PID_D = pid.KD * ((pid.Error - pid.PreviousError)/measurmentTime.ElapsedTime);

  pid.PID = pid.PID_P + pid.PID_I + pid.PID_D;

  if(pid.PID < -1000) {
    pid.PID=-1000;
  }
  if(pid.PID > 1000) {
    pid.PID=1000;
  }

  pid.PwmLeft = throttle + pid.PID;
  pid.PwmRight = throttle - pid.PID;

  if(pid.PwmRight < 1000) {
  pid.PwmRight = 1000;
  }
  if(pid.PwmRight > 2000) {
    pid.PwmRight=2000;
  }

  if(pid.PwmLeft < 1000) {
    pid.PwmLeft= 1000;
  }
  if(pid.PwmLeft > 2000) {
    pid.PwmLeft=2000;
  }
  Serial.print(pid.PwmLeft);
  Serial.print(":PWM Left motor ");
  Serial.print(pid.PwmRight);
  Serial.println(":PWM Right motor ");

  escMotors[0].ESCWriteMicroseconds(pid.PwmLeft); 
  escMotors[1].ESCWriteMicroseconds(pid.PwmRight);
  escMotors[2].ESCWriteMicroseconds(1400); 
  escMotors[3].ESCWriteMicroseconds(1400);
  
  pid.PreviousError = pid.Error;

}
