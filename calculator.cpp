#include "mbed.h"
#include "C12832.h"
volatile int num1=1;
volatile int num2=1;
volatile int *pos = &num1;
int c=num1;
int a = 0;
volatile int status=1;
char str[5] = {'+','-','*','/','^'};
C12832 lcd(D11, D13, D12, D7, D10);
Ticker my_ticker;
class LED {//begin declaration of class LED
    //private://private data member declaration
    //DigitalOut red, blue, green; // Declaration of DigitalOut object 
    bool status;// status represent the state of the LED
    public://public declaration
    DigitalOut red, blue, green;
    LED(PinName rd, PinName b, PinName gr) : red(rd), blue(b), green(gr) {}//Constructor to provide pin name is assigned to DigitalOut 
    void off (){// member function for when LED is off
        red=1;
        blue=1;
        green=1; // all LEDs are active low so set to 1 would turn them off
        status = false;// LED is off
    }
    void onRed(){// member function for when LED is red
        red=0;// pin for red LED is active low
        status=true; // LED is on
    }
    void onBlue(){//// member function for when LED is blue
        blue=0;// pin for blue LED is active low
        status=true;// LED is on
    }
    void onYellow(){// member function for when LED is green
        green=0;
        red=0;// pin for green LED is active low
        status=true;//LED is on
    }
};  
LED lights(D5, D8, D9);
 float Arithmetic(){
switch (str[a]){
    case '+':
        return float(num1+num2);
    break;
    case '-':
        return float(num1-num2);
    break;
    case '*':
        return float(num1*num2);
    case '/':
        return float(num1)/ float (num2);
    case '^':
        if (num2==0){
            return 1.00;
        } else{
        int b=num1;
        int c=num1;
        for (int i=1;i<num2;i++){
         c=c*b;
        }
        return float(c);
        }
    break;
    default: return 0;// case default will not be reached due to limits put on the operator string 
}
}
    void update_display(){
    lcd.locate(20,12);
    lcd.printf("%d", num1);
    lcd.locate(40,12);
    lcd.printf("%d", num2);
    lcd.locate(30,12);
    lcd.printf("%c", str[a]);
    lcd.locate(60,12);
    lcd.printf("= %.2f",Arithmetic());
    }
void toggleISR()                       
{   if (pos==&num1){
        if (status == 1) {            //Checks the value of the status variable...
        status = 0;
        lcd.locate(20, 18);
        lcd.printf("_");
        update_display();        //toggles the output
    } else {
        lcd.cls();
        update_display();
        status = 1;
    }
    }   else if (pos==&num2) {
        if (status == 1) {            //Checks the value of the status variable...
        status = 0;
        lcd.locate(40, 18);
        lcd.printf("_");
        update_display();        // toggles output
    } else {
        lcd.cls();
        update_display();
        status = 1;
    }
    }   else if (pos==&a) {
        if (status == 1) {            //Checks the value of the status variable...
        status = 0;
        lcd.locate(30, 18);
        lcd.printf("_");
        update_display();        //toggle output
    } else {
        lcd.cls();
        update_display();
        status = 1;
    }
    } 
}   
    InterruptIn up(A2);
    InterruptIn down(A3);
    InterruptIn fire(D4); 
    void firePressedISR(){
        if (pos==&num1){
        pos=&num2;
        lights.off();
        lights.onYellow();
        } else if (pos==&num2) {
        pos=&a;
        lights.off();
        lights.onBlue();
        } else if (pos==&a) {
        pos=&num1;
        lights.off();
        lights.onRed();
        }
    }
void upPressedISR (){
    if (pos==&num1) {
        if (num1>=0 && num1<9){
            num1++;
            lights.onRed();
            update_display();
        }
    }   else if (pos==&num2){
            if (num2>=0 && num2<9){
           num2++;
            lights.onYellow();
            update_display();
            }    
        } else if (pos==&a){
            if (a>=0 && a<4){
            a++;  
            update_display();
            lights.onBlue();
            }
    }
}     
void downPressedISR (){
    if (pos==&num1) {
        if (num1>0 && num1<=9){
            num1--;
            lights.onRed();
            update_display();
        }
    } else if (pos==&num2){      
        if (num2>0 && num2<=9){
            num2--;
            lights.onYellow();
            update_display();
        }
    } else if (pos==&a) {
        if (a>0 && a<5){
        a--;
        update_display();
        lights.onBlue();
    }
    }
}       
int main (){
    lights.off();
    lcd.cls();
    lights.onRed();
    my_ticker.attach(&toggleISR,0.75);
    fire.rise(&firePressedISR);
    up.rise(&upPressedISR);
    down.rise(&downPressedISR);
    while (1) {};
};
