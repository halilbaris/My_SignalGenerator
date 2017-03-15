//Моя версия генератора сигналлов, разработанна для рабочих целей.
//Страница проэкта  http://srukami.inf.ua/signalgenerator.html
#include <PWM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#define led  9   //пин для генератора сигналов (не менять)
//#################№№№№№№####пользовательские настройки
#define levo 10  //кнопка ЛЕВО(можно любой пин)
#define ok 11    //кнопка ОК(можно любой пин)
#define pravo 12 //кнопка ПРАВО(можно любой пин) 
Adafruit_PCD8544 display = Adafruit_PCD8544(2, 3, 4, 6, 7);//пины к которым у вас подключен дисплей          
int PWM = 100;//стартовое значение ШИМ от 0 до 255        
int32_t frequency = 6000; //стартовое значение частоты в Гц
byte cont=40;//контрастность дисплея
//###########################################################
byte hag=0;
int mnog=0;
boolean flag=0;
byte mass[85];
byte x=0;     
void setup(){
InitTimersSafe(); 
display.begin();
display.setContrast(cont);
display.display();
ADCSRA = 0b11100010;
ADMUX = 0b01100100;
bool success = SetPinFrequencySafe(led, frequency);
delay(50);
      }
      
void Zamer(){
  for(byte i=0;i<84;i++){ 
  while ((ADCSRA & 0x10)==0);
  ADCSRA|=0x10;
  mass[i]=ADCH;
  }
}
void Zamer1(){
  for(byte i=0;i<84;i++){ 
  while ((ADCSRA & 0x10)==0);
  ADCSRA|=0x10;
  mass[i]=ADCH;
  delayMicroseconds(5);
  }
}
void Zamer2(){
  for(byte i=0;i<84;i++){ 
  while ((ADCSRA & 0x10)==0);
  ADCSRA|=0x10;
  mass[i]=ADCH;
  delayMicroseconds(50);
  }
}

void loop() {
if(frequency>5000){Zamer();}
if(frequency<=5000){Zamer1();}
if(frequency<=1000){Zamer2();}
if(frequency>40000){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Sorry");
    display.println("Oscilloscope");
    display.println("disabled");    
          }
if(frequency<=40000){//отрисовка графика
      display.clearDisplay();
      x=0;
      for(int y=1;y<83;y++){
      display.drawLine(x, 37-mass[y]/7, x+1, 37-mass[y+1]/7, BLACK);
      x=x+1;   
      }
}
if (flag==0){//флаг выборов режима настройки ШИМ или Частоты
            if(digitalRead(levo)==HIGH){
              frequency=frequency-mnog;
              if(frequency<0){frequency=0;}
              bool success = SetPinFrequencySafe(led, frequency);
              delay(3);//защита от дребезга 
            }
            if(digitalRead(pravo)==HIGH){
              frequency=frequency+mnog;
              bool success = SetPinFrequencySafe(led, frequency);
              delay(3);//защита от дребезга 
            }  
}
if (flag==1){//флаг выборов режима настройки ШИМ или Частоты
            if(digitalRead(levo)==HIGH){
              PWM=PWM-10;
              if(PWM<0){PWM=255;} 
              delay(3);//защита от дребезга
            
            }
            if(digitalRead(pravo)==HIGH){
              PWM=PWM+10;
              if(PWM>255){PWM=0;} 
              delay(3);//защита от дребезга 
            }  
}      
if(digitalRead(ok)==HIGH){//переключение разряда выбора частоты 
  delay(3);//защита от дребезга
  hag++;
  if(hag>=5){hag=0;}
}     
display.setCursor(0,40);
display.print(frequency); 
display.print("Hz");
display.print("*");    
      if(hag==0){//выбор множителя частоты
          display.print("1"); 
          mnog=1;
          flag=0;
      }
      if(hag==1){//выбор множителя частоты
          display.print("10"); 
          mnog=10;
      }
      if(hag==2){//выбор множителя частоты
          display.print("100"); 
          mnog=100;
      }
      if(hag==3){//выбор множителя частоты
          display.print("1K"); 
          mnog=1000;
      } 
      if(hag==4){//выбор  PWM
          display.print("PWM");
          display.print(PWM); 
          flag=1;
      } 
pwmWrite(led, PWM);
delay(300);  
display.display();
}


