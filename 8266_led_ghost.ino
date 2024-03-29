/*=====================================================================
 * http experiments      
 */

// Wifi Definitions
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <EEPROM.h>


//#define INIT_NV

ESP8266WebServer server(80);
IPAddress local_IP(192,168,4,1);

const char WiFiAPPSK[] = "dawson";

int char_index=0;
int column_index = 0;

#define DISPLAY_STRING_MAX_LENGTH 20

typedef enum
{
  TEXT_DIR_FWD = 1,
  TEXT_DIR_BACK
} text_dir_type;

typedef enum
{
  LETTER_DIR_1_BOTTOM = 1,
  LETTER_DIR_1_TOP = 2
} letter_dir_type;

typedef struct
{
  char            display_string[DISPLAY_STRING_MAX_LENGTH];
  text_dir_type   text_dir;
  int             led_delay_us;
  letter_dir_type letter_dir;
} nv_data_type;

nv_data_type nv_data;

bool debug=false;
  
// 74595 lines
#define DATA_PIN  5
#define CLK_PIN   4
#define LATCH_PIN 0

// We have 8 output LEDs in our current incarnation.
#define OUTPUT_LINES 8

int autolatch = 1;
int touchpot_enabled = 1;

// A:
// 0 1 1 0
// 1 0 0 1
// 1 0 0 1
// 1 1 1 1
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
int A[] = {0, 0x3f, 0x48, 0x48, 0x3f, 0};

// B:
// 1 1 1 0
// 1 0 0 1
// 1 0 0 1
// 1 1 1 0
// 1 0 0 1
// 1 0 0 1
// 1 1 1 0
int B[] = {0, 0x7F, 0x49, 0x49, 0x36, 0};

// C
// 0 1 1 1
// 1 0 0 0
// 1 0 0 0
// 1 0 0 0
// 1 0 0 0
// 1 0 0 0
// 0 1 1 1
int C[] = {0, 0x3E, 0x41, 0x41, 0x41, 0};


// D:
// 1 1 1 0
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
// 1 1 1 0
int D[] = {0, 0x7F, 0x41, 0x41, 0x3E, 0};


// E:
// 1 1 1 1
// 1 0 0 0
// 1 0 0 0
// 1 1 1 1
// 1 0 0 0 
// 1 0 0 0
// 1 1 1 1
int E[] = {0, 0x7F, 0x49, 0x49, 0x49, 0};

// F:
// 1 1 1 1
// 1 0 0 0
// 1 0 0 0
// 1 1 1 1
// 1 0 0 0 
// 1 0 0 0
// 1 0 0 0
int F[] = {0, 0x7F, 0x48, 0x48, 0x48, 0};

// G
// 0 1 1 0
// 1 0 0 1
// 1 0 0 0
// 1 0 0 0
// 1 0 1 1
// 1 0 0 1
// 0 1 1 0
int G[] = {0, 0x3E, 0x41, 0x45, 0x26, 0};

// H:
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
// 1 1 1 1
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
int H[] = {0, 0x7F, 0x08, 0x08, 0x7f, 0};

// I:
// 1 1 1 0
// 0 1 0 0
// 0 1 0 0 
// 0 1 0 0
// 0 1 0 0
// 0 1 0 0 
// 1 1 1 0
int I[] = {0, 0x41, 0x7F, 0x41, 0x0, 0};

// J:
// 0 0 0 1
// 0 0 0 1
// 0 0 0 1 
// 0 0 0 1
// 0 0 0 1
// 1 0 0 1
// 0 1 1 0
int J[] = {0, 0x02, 0x01, 0x01, 0x3E, 0};

// K:
// 1 0 0 0 1
// 1 0 0 1 0
// 1 0 1 0 0 
// 1 1 0 0 0
// 1 0 1 0 0
// 1 0 0 1 0
// 1 0 0 0 1
int K[] = {0, 0x7f, 0x08, 0x14, 0x22, 0x41};

// L:
// 1 0 0 0
// 1 0 0 0
// 1 0 0 0 
// 1 0 0 0
// 1 0 0 0
// 1 0 0 0
// 1 1 1 1
int L[] = {0, 0x7f, 0x01, 0x01, 0x01, 0};

// M:
// 1 0 0 0 1
// 1 1 0 1 1
// 1 0 1 0 1
// 1 0 0 0 1
// 1 0 0 0 1
// 1 0 0 0 1
// 1 0 0 0 1
int M[] = {0, 0x7f, 0x20, 0x10, 0x20, 0x7F};

// N:
// 1 0 0 0 1
// 1 1 0 0 1
// 1 0 1 0 1
// 1 0 1 0 1
// 1 0 1 0 1
// 1 0 0 1 1
// 1 0 0 0 1
int N[] = {0, 0x7F,0x20, 0x1C, 0x02, 0x7F};

// O:
// 0 1 1 1 0
// 1 0 0 0 1
// 1 0 0 0 1
// 1 0 0 0 1
// 1 0 0 0 1
// 1 0 0 0 1
// 0 1 1 1 0
int O[] = {0, 0x3E, 041, 0x41, 0x41, 0x3E};


// P
// 1 1 1 0
// 1 0 0 1
// 1 0 0 1
// 1 1 1 0
// 1 0 0 0
// 1 0 0 0
// 1 0 0 0
int P[] = {0, 0x7f, 0x48, 0x48, 0x30, 0};

// Q:
// 0 1 1 1 0
// 1 0 0 0 1
// 1 0 0 0 1
// 1 0 0 0 1
// 1 0 1 0 1
// 1 0 0 1 1
// 0 1 1 1 1
int Q[] = {0, 0x3E, 041, 0x45, 0x43, 0x3F};

// R
// 1 1 1 0
// 1 0 0 1
// 1 0 0 1
// 1 1 1 0
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
int R[] = {0, 0x7f, 0x48, 0x48, 0x37, 0};

// S
// 0 1 1 0
// 1 0 0 1
// 1 0 0 0
// 0 1 1 0
// 0 0 0 1
// 1 0 0 1
// 0 1 1 0
int S[] = {0, 0x32, 0x49, 0x49, 026, 0};


// T
// 1 1 1 0
// 0 1 0 0
// 0 1 0 0
// 0 1 0 0
// 0 1 0 0
// 0 1 0 0
// 0 1 0 0
int T[] = {0, 0x40, 0x7f, 0x40, 0, 0};

// U
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
// 0 1 1 0
int U[] = {0, 0x7e, 0x01, 0x01, 0x7e, 0};

// V
// 1 0 0 0 1
// 1 0 0 0 1
// 0 1 0 1 0
// 0 1 0 1 0
// 0 1 0 1 0
// 0 0 1 0 0
// 0 0 1 0 0
int V[] = {0, 0x60, 0x1c, 0x03, 0x1c, 0x60};

// W
// 1 0 0 0 1
// 1 0 0 0 1
// 1 0 0 0 1
// 1 0 0 0 1
// 1 0 1 0 1
// 1 0 1 0 1
// 0 1 0 1 0
int W[] = {0, 0x7E, 0x01, 0x06, 0x01, 0x7E};

// X
// 1 0 0 0 1
// 0 1 0 1 0
// 0 1 0 1 0
// 0 0 1 0 0
// 0 1 0 1 0
// 0 1 0 1 0
// 1 0 0 0 1
int X[] = {0, 0x41, 0x36, 0x04, 0x36, 0x41};

// Y
// 1 0 0 0 1
// 0 1 0 1 0
// 0 1 0 1 0
// 0 0 1 0 0
// 0 0 1 0 0
// 0 0 1 0 0
// 0 0 1 0 0
int Y[] = {0, 0x40, 0x30, 0x0F, 0x30, 0x40};

// Z
// 1 1 1 1 1
// 0 0 0 1 0
// 0 0 0 1 0
// 0 0 1 0 0
// 0 1 0 0 0
// 0 1 0 0 0
// 1 1 1 1 1
int Z[] = {0, 0x41, 0x47, 0x49, 0x71, 0x41};

// empty:
int blank[] = {0, 0, 0, 0, 0, 0};

int *char_map[] = {A, B, C, D, E, F, G, H, I, J, 
                   K, L, M, N, O, P, Q, R, S, T,
                   U, V, W, X, Y, Z};
                   
//==============================================================================================
// FUNCTION:  write bit.
//  
// Outputs a bit to the shift register and clocks it in.
//==============================================================================================
void write_bit(int bit)
{
  digitalWrite(DATA_PIN, bit);
  digitalWrite(CLK_PIN, HIGH);
  digitalWrite(CLK_PIN, LOW);
}

//==============================================================================================
// FUNCTION:  latch_data
//
// stores the current shift registers in the latch memorie  
//==============================================================================================
void latch_data( void )
{
  digitalWrite(LATCH_PIN, HIGH);
  digitalWrite(LATCH_PIN, LOW);
}

//==============================================================================================
// FUNCTION:  write_and_latch_byte
//
// Writes an entire byte to the shift register, and then latches the data.  Note this function
// goes MSB first.
//==============================================================================================
#define BITS_IN_BYTE 8
void write_and_latch_byte( int data )
{
  int temp_bit;
  int temp_data;
  int i;
  
  data &= 0xFF;
    
  temp_data = data;
  
  for (i = 0; i < BITS_IN_BYTE; i++)
  {
    // we only want the MSB
    temp_bit = temp_data & 0x80;
    temp_bit = temp_bit >> 7;
    write_bit(temp_bit);

    // now shift our byte to get the next bit
    temp_data = temp_data << 1;     
  }

  latch_data();
  
}

//==============================================================================================
// FUNCTION:  write_and_latch_byte_lsb_first
//
// Writes an entire byte to the shift register, and then latches the data.  Note this function
// goes LSB first.
//==============================================================================================
void write_and_latch_byte_lsb_first( int data )
{
  int temp_bit;
  int temp_data;
  int i;
  
  data &= 0xFF;
    
  temp_data = data;
  
  for (i = 0; i < BITS_IN_BYTE; i++)
  {
    // we only want the LSB
    temp_bit = temp_data & 0x01;
    write_bit(temp_bit);

    // now shift our byte to get the next bit
    temp_data = temp_data >> 1;     
  }

  latch_data();
  
}

const String styleInfo = 
  "<style>\
    form\
    {\
      margin: 0 auto;\
      width: 400px;\
      padding: 1em;\
      border: 1px solid #CCC;\
      border-radius: 1em;\
    }\
    form div\
    {\
      margin-top: 1em;\
    }\
    label\
    {\
      display: inline-block;\
      width: 180px;\
      text-align: right;\
    }\
    input[type=text]\
    {\
      font: 1em sans-serif;\
      width: 200px;\
      box-sizing: border-box;\
      border: 1px solid #999;\
    }\
    input:focus\
    {\
      border-color: #000;\
    }\
    button { font: 1em sans-serif; margin-left:40% }\
    button:hover,button:focus\
    {\
      outline : none;\
      background: #000;\
      color: #FFF;\
    }\
  </style>";
    

void handleRoot( void )
{
  String myForm;
  String current_string=nv_data.display_string;
  String delay_str;
  
  delay_str = String(nv_data.led_delay_us);
  
  myForm = "<html><head><title>Glenn's Frisbee</title>";
  myForm += styleInfo;
  myForm += "</head>";

  myForm += "<body><h1> Welcome to Glenn's Frisbee!!!</h1>";
  myForm += "<form action=\"/input\" method=\"POST\">";

  myForm +=    "<div>";
  myForm +=       "<label for=\"text_box\">Display Text:</label>";
  myForm +=       "<input type=\"text\" id=\"text_box\" name=\"text\" placeholder=\"";
  myForm +=           current_string;
  myForm +=           "\"";
  myForm +=    "</div>";
  
  myForm +=    "<div>";
  myForm +=       "<label for=\"delay_box\">Led Delay (us):</label>";
  myForm +=       "<input type=\"text\" id=\"delay_box\" name=\"delay\" placeholder=\"";
  myForm +=           delay_str;
  myForm +=           "\"";
  myForm +=    "</div>";

  myForm +=    "<div>";
  myForm +=       "<input type=\"radio\" name=\"letter_dir\" value=\"1\"";
  if (nv_data.letter_dir == LETTER_DIR_1_BOTTOM) myForm += " checked";
  myForm +=            ">1-bottom<br>";
  myForm +=       "<input type=\"radio\" name=\"letter_dir\" value=\"2\"";
  if (nv_data.letter_dir == LETTER_DIR_1_TOP) myForm += " checked";
  myForm +=            ">1-top<br>";
  myForm +=    "</div>";

  myForm +=    "<div>";
  myForm +=       "<input type=\"radio\" name=\"text_dir\" value=\"1\"";
  if (nv_data.text_dir == TEXT_DIR_FWD) myForm += " checked";
  myForm +=            ">left-right<br>";
  myForm +=       "<input type=\"radio\" name=\"text_dir\" value=\"2\"";
  if (nv_data.text_dir == TEXT_DIR_BACK) myForm += " checked";
  myForm +=            ">right-left<br>";
  myForm +=    "</div>";
  
  myForm +=    "<div>";
  myForm +=       "<button type = \"submit\">Set values</button>";
  myForm +=    "</div>";
  
  myForm += "</form>";

  myForm += "</body></html>";
  
  server.send(200, "text/html", myForm);  
}

void handleInput()
{
  String   input_string;
  long int input_int;

  if (server.hasArg("text") && (server.arg("text") != NULL))
  {
    Serial.print("Setting display_string to ");
    Serial.println(server.arg("text"));

    char_index = 0;
    column_index = 0;
  
    // I'm going to go ahead and bulk copy the string here.  I'll deal with
    // upper case conversion and checking for bad characters as we display it.
    server.arg("text").toCharArray(nv_data.display_string, DISPLAY_STRING_MAX_LENGTH);
    
  }  // Text arg.

  if (server.hasArg("delay") && server.arg("delay") != NULL)
  {
    // if the integer conversion fails, we'll get a zero.  I think that's okay.
    nv_data.led_delay_us = server.arg("delay").toInt();

    Serial.print("Setting delay to ");
    Serial.println(nv_data.led_delay_us);
    
  }  // delay arg.

  if (server.hasArg("letter_dir") && server.arg("letter_dir") != NULL)
  {
    // if the integer conversion fails, we'll get a zero.
    input_int = server.arg("letter_dir").toInt();

    if ((input_int != LETTER_DIR_1_BOTTOM) && (input_int != LETTER_DIR_1_TOP))
    { 
      Serial.println("Invalid Letter Dir.  Setting to 1 (1=bottom)");
      nv_data.letter_dir = LETTER_DIR_1_BOTTOM;
    }
    else
    {
      nv_data.letter_dir = (letter_dir_type) input_int;
    }
    
    Serial.print("Setting letter dir to ");
    Serial.println(nv_data.letter_dir);
    
  }  // letter dir.

  if (server.hasArg("text_dir") && server.arg("text_dir") != NULL)
  {
    // if the integer conversion fails, we'll get a zero.
    input_int = server.arg("text_dir").toInt();

    if ((input_int != TEXT_DIR_FWD) && (input_int != TEXT_DIR_BACK))
    { 
      Serial.println("Invalid Text Dir.  Setting to 1 (FWD)");
      nv_data.text_dir = TEXT_DIR_FWD;
    }
    else
    {
      nv_data.text_dir = (text_dir_type) input_int;
    }
    
    Serial.print("Setting text dir to ");
    Serial.println(nv_data.text_dir);
    
  }  // text dir.
  
  // store the new data to NV for next time.
  EEPROM.put(0, nv_data);
  EEPROM.commit();

  server.sendHeader("Location", "/");
  server.send(303);
    
}

void handleNotFound( void )
{
  server.send(404, "text/plain", "404: Not found");
}

void setup_wifi( void )
{
  bool ret_val;

  //WiFi.softAPConfig(local_IP, gateway, subnet);
  
  ret_val = WiFi.softAP("Glenn's Frisbee");
  //WiFi.softAP(ssid,pass,channel,hidden,max_connects);

  Serial.print("Soft-AP config: ");
  if (ret_val) Serial.println("Succeeded!");
  else Serial.println("FAILED");
}

//==============================================================================================
// FUNCTION:  setup
//==============================================================================================
void setup() 
{
  int i;
  
  Serial.begin(9600);

  EEPROM.begin(sizeof(nv_data_type));
  EEPROM.get(0,nv_data);

  #ifdef INIT_NV
  strcpy(nv_data.display_string, "GLENN");
  nv_data.text_dir = TEXT_DIR_FWD;
  nv_data.led_delay_ms = 1;
  EEPROM.put(0,nv_data);
  EEPROM.commit();
  #endif

  Serial.println("\nNV Config:");
  Serial.print("  Display string:  ");
  Serial.println(nv_data.display_string);
  Serial.print("  text_dir: ");
  Serial.println(nv_data.text_dir);
  Serial.print("  led delay (ms): ");
  Serial.println(nv_data.led_delay_us);
  Serial.print("  letter_dir: ");
  Serial.println(nv_data.letter_dir);
  
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  // Make sure all lines are low
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  
  // make sure our shift register and latches are all 0.
  for (i=0; i<OUTPUT_LINES; i++)
  {
    write_bit(0);
  }
  latch_data();

  setup_wifi();

  server.on("/", HTTP_GET, handleRoot);
  server.on("/input", HTTP_POST, handleInput);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
  
  delay(500);
  
  // now a quick LED test...write a 1 to all registers.
  for (i=0; i<OUTPUT_LINES; i++)
  {
    write_bit(1);
    latch_data();
    delay(500);
  }

  // ...and now put them back to all zeros before starting for real.
  for (i=0; i<OUTPUT_LINES; i++)
  {
    write_bit(0);
  }
  
  delay(1000);
  
  latch_data();


  Serial.println("Initialization done.");
}

//==============================================================================================
// FUNCTION:  loop
//==============================================================================================
#define NUM_COLUMNS 6
void loop() 
{
  int *array;
  int display_strlen;
  char display_char;

  //process_wifi();
  server.handleClient();
  
  display_strlen = strlen(nv_data.display_string);

  if (char_index == display_strlen)
  {
    array = blank;
  }
  else
  {
    display_char = nv_data.display_string[char_index];
    
    //Serial.print("display char:");
    //Serial.println(display_char);

    if ((display_char >= 'A') && (display_char <= 'Z')) 
    {
      // upper case...use 'A' as a reference into our array.
      array = char_map[display_char - 'A'];  
    }
    else if ((display_char >= 'a') && (display_char <= 'z'))
    {
      // lower case char.  Use 'a' to reference into our array.
      array = char_map[display_char - 'a'];
    }
    else 
    {
      // invalid character.  Make it a blank.
      array = blank;
    }
  }

  // by default, we're latching in from LED 1 to 8...meaning that write_and_latch_byte
  // will display the row with 1 being the bottom and 8 being the top.  
  if (nv_data.letter_dir == LETTER_DIR_1_BOTTOM)
  {
    write_and_latch_byte(array[column_index]);
  }
  // Otherwise, we want to do the writes LSB first, so that 8 is the bottom and 1 is the top.
  else
  {
    write_and_latch_byte_lsb_first(array[column_index]);
  }
  
  //Serial.print("byte: ");
  //Serial.println(array[column_index]);

  if (nv_data.text_dir == TEXT_DIR_FWD)
  {
    column_index++;
    if (column_index == 6)
    {
      column_index = 0;
      char_index++;
      if (char_index == display_strlen + 1) char_index = 0;
    }
  }
  else
  {
    if (column_index == 0)
    {
      column_index = 5;
      if (char_index == 0)
      {
        char_index = display_strlen;
      }
      else
      {
        char_index--;
      }
    }
    else
    {
      column_index--;
    }
  }

  delayMicroseconds(nv_data.led_delay_us);
  
}
