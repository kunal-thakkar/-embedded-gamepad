/*
================================================================================

    File........... NES Controller Test Code
    Purpose........ To demonstrate how to interface to an NES controller
    Author......... Joseph Corleto
    E-mail......... corleto.joseph @ gmail.com
    Started........ 04/13/2016
    Finished....... 04/14/2016
    Updated........ --/--/----
 
================================================================================
   Notes
================================================================================
- The NES controller contains one 8-bit 4021 shift register inside. 
- This register takes parallel inputs and converts them into a serial output.
- This code first latches the data and then shifts in the first bit on the data line. 
  Then it clocks and shifts in on the data line until all bits are received.  
- What is debugged are the button states of the NES controller.
- A logical "1" means the button is not pressed. A logical "0" means the button is pressed.  
- This code shifts the first bit of data into the LSB.
- The order of shifting for the buttons is shown in the table below:
    Bit# | Button   
    --------------
      0  |  A  
    --------------
      1  |  B  
    --------------
      2  |  Select   
    --------------
      3  |  Start  
    --------------
      4  |  Up  
    --------------
      5  |  Down  
    --------------
      6  |  Left   
    --------------
      7  |  Right   
    --------------        
*/
//===============================================================================
//  Header Files
//===============================================================================
//===============================================================================
//  Constants
//===============================================================================
// Here we have a bunch of constants that will become clearer when we look at the
// readNesController() function. Basically, we will use these contents to clear
// a bit. These are chosen according to the table above.
const int A_BUTTON         = 0;
const int B_BUTTON         = 1;
const int SELECT_BUTTON    = 2;
const int START_BUTTON     = 3;
const int UP_BUTTON        = 4;
const int DOWN_BUTTON      = 5;
const int LEFT_BUTTON      = 6;
const int RIGHT_BUTTON     = 7;

//===============================================================================
//  Variables
//===============================================================================
int nesRegister  = 0;    // We will use this to hold current button states
//===============================================================================
//  Pin Declarations
//===============================================================================
//Inputs:
int nesData       = 4;    // The data pin for the NES controller
//Outputs:
int nesClock      = 2;    // The clock pin for the NES controller
int nesLatch      = 3;    // The latch pin for the NES controller

//===============================================================================
//  Initialization
//===============================================================================
void setup() 
{
  // Initialize serial port speed for the serial terminal
  Serial.begin(9600);
  
  // Set appropriate pins to inputs
  pinMode(nesData, INPUT);
  
  // Set appropriate pins to outputs
  pinMode(nesClock, OUTPUT);
  pinMode(nesLatch, OUTPUT);
  
  // Set initial states
  digitalWrite(nesClock, LOW);
  digitalWrite(nesLatch, LOW);
}

//===============================================================================
//  Main
//===============================================================================
void loop() 
{
  // This function call will return the states of all NES controller's register
  // in a nice 8 bit variable format. Remember to refer to the table and
  // constants above for which button maps where!
  nesRegister = readNesController();
  
  // Slight delay before we debug what was pressed so we don't spam the serial monitor.
  delay(180);  
  // To give you an idea on how to use this data to control things for your
  // next project, look through the serial terminal code below. Basically,
  // just choose a bit to look at and decide what to do whether HIGH (not pushed)
  // or LOW (pushed). What is nice about this test code is that we mapped all
  // of the bits to the actual button name so no useless memorizing!
  if(nesRegister != 255){
    Serial.print("nesRegister: ");
    Serial.println(nesRegister);
  }
  if (bitRead(nesRegister, A_BUTTON) == 0)
    Serial.println("A_BUTTON");
    
  if (bitRead(nesRegister, B_BUTTON) == 0)
    Serial.println("B_BUTTON");
    
  if (bitRead(nesRegister, START_BUTTON) == 0)
    Serial.println("START_BUTTON");
  
  if (bitRead(nesRegister, SELECT_BUTTON) == 0)
    Serial.println("SELECT_BUTTON");
    
  if (bitRead(nesRegister, UP_BUTTON) == 0)
    Serial.println("UP_BUTTON");
    
  if (bitRead(nesRegister, DOWN_BUTTON) == 0)
    Serial.println("DOWN_BUTTON");
    
  if (bitRead(nesRegister, LEFT_BUTTON) == 0)
    Serial.println("LEFT_BUTTON");  
  
  if (bitRead(nesRegister, RIGHT_BUTTON) == 0)
    Serial.println("RIGHT_BUTTON");
}

//===============================================================================
//  Functions
//===============================================================================
///////////////////////
// readNesController //
///////////////////////
int readNesController() 
{  
  // Pre-load a variable with all 1's which assumes all buttons are not
  // pressed. But while we cycle through the bits, if we detect a LOW, which is
  // a 0, we clear that bit. In the end, we find all the buttons states at once.
  int tempData = 255;
  // Quickly pulse the nesLatch pin so that the register grab what it see on its parallel data pins.
  digitalWrite(nesLatch, HIGH);
  digitalWrite(nesLatch, LOW);
  for(int i = 0; i < 8; i++) {
    // Upon latching, the "i" bit is available to look at. We see if it is low, 
    // and if it is, we clear out variable's "i" bit to indicate this is so.
    if (digitalRead(nesData) == LOW)
      bitClear(tempData, i);
 digitalWrite(nesClock, HIGH);
  digitalWrite(nesClock, LOW);
   }
  return tempData;
}
