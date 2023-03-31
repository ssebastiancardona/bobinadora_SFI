

//Variables for the menu encoder
int counter = 0; 
int page=1;
int Ready=1;
int submenu=0;
int last_counter = 0; 
bool clk_State;
bool Last_State; 
bool dt_State;  
int pushed = 0;

//The pin for the push button
#define push 10

void setup() {
  pinMode (push,INPUT);       //Define the pin as input
  
  lcd.init();                 //Init the LCD
  lcd.backlight();            //Activate backlight
  lcd.createChar(0, note);    //Create the note symbol
  lcd.createChar(1, arrow);   //Create the arrow symbol
  lcd.home();                 //Home the LCD
  
  PCICR |= (1 << PCIE0);    //enable PCMSK0 scan                                                 
  PCMSK0 |= (1 << PCINT0);  //Set pin D8 trigger an interrupt on state change. 
  PCMSK0 |= (1 << PCINT1);  //Set pin D9 trigger an interrupt on state change.  
  DDRB &= B11111100;        //8, 9 as input for the encoder clock and data pins

  Last_State =   (PINB & B00000001); //pin 8 state (clock pin)? 
  
  //Prepare the DFplayer module comunication and settings  
  Serial.begin(115200);  
  mp3.begin();
  uint16_t volume = mp3.getVolume();
  mp3.setVolume(20);  
  uint16_t count = mp3.getTotalTrackCount();

  //Print the initial text. Delete these lines if you don't want that
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(0);  
  lcd.print(" ELECTRONOOBS ");
  lcd.write(0);
  lcd.setCursor(0,1);  
  lcd.print(" Coffee machine ");
  delay(3000);

  //Print the first page menu.
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(1);  
  lcd.print("Choose coffee");
  lcd.setCursor(0,1);  
  lcd.print(" Sugar level");
}

 
void loop (){

  //Add limit for the counter. Each line of the menu has 5 points. Since my menu has 4 lines the maximum counter will be from 0 to 20
  //If you add more lines for the menu, increase this value
  if(counter > 20)
  {
    counter=20;
  }
  if(counter < 0)
  {
    counter=0;
  }
}//end void

//Interruption vector

ISR(PCINT0_vect){
  
  clk_State =   (PINB & B00000001); //pin 8 state, clock pin? 
  dt_State  =   (PINB & B00000010); 
  if (clk_State != Last_State){     
     // If the data state is different to the clock state, that means the encoder is rotating clockwise
     if (dt_State != clk_State) { 
       counter ++;
     }
     else {
       counter --;
     } 
   } 
   Last_State = clk_State; // Updates the previous state of the data with the current state
  
}