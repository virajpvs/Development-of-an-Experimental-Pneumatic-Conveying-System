//This Arduino code is developed by I Lab
//Hit the SUBSCRIBE button for following our tutorial on arduino.
//You Tube Channel ID: https://www.youtube.com/c/IngenieroLab?sub_confirmation=1.
//Follow our facebook page: https://www.facebook.com/ingenierorobotico

//BTS7960 motor driver sketch 


int R_IS = 15;
int R_EN = 5;
int ledPin1 = 2;
//int R_PWM = 12;
int L_IS = 18;
int L_EN = 19;
//int L_PWM = 13;
int ledPin2 = 4;

const int freq = 5000;
const int ledChannel1 = 0;
const int ledChannel2 = 1;
const int resolution = 8;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 pinMode(R_IS, OUTPUT);
 pinMode(R_EN, OUTPUT);
 pinMode(ledPin1, OUTPUT);
 pinMode(L_IS, OUTPUT);
 pinMode(L_EN, OUTPUT);
 pinMode(ledPin2, OUTPUT);
 digitalWrite(R_IS, LOW);
 digitalWrite(L_IS, LOW);
 digitalWrite(R_EN, HIGH);
 digitalWrite(L_EN, HIGH);

   // configure LED PWM functionalitites
  ledcSetup(ledChannel1, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin1, ledChannel1);

   // configure LED PWM functionalitites
  ledcSetup(ledChannel2, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin2, ledChannel2);
}

void loop() {
    // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    ledcWrite(ledChannel1, dutyCycle);
    Serial.println(dutyCycle);
    ledcWrite(ledChannel2, 0);
    delay(15);
  }

    for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    ledcWrite(ledChannel2, dutyCycle);
    Serial.println(dutyCycle);
    ledcWrite(ledChannel1, 0);   
    delay(15);
  }


}
