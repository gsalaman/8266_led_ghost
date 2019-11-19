/*=====================================================================
 * http experiments      
 */

// Wifi Definitions
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char WiFiAPPSK[] = "dawson";

int char_index=0;
int column_index = 0;
char display_string[20] = "GLENN";

bool debug=false;
  
// 74595 lines
#define DATA_PIN  5
#define CLK_PIN   0
#define LATCH_PIN 4

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
// Writes an entire byte to the shift register, and then latches the data.  
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

void oldsetupWiFi()
{
  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "Glenn's Frisbee " + macID;

  Serial.print("WiFi configured.  AP name: ");
  Serial.println(AP_NameString);

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);

  Serial.print("** AP_NameChar: ");
  Serial.println(AP_NameChar);
  Serial.print("APPSK: ");
  Serial.println(WiFiAPPSK);

  
}

void process_request( String request )
{
  int  text_index = 0;
  int  request_index;
  
  char frisbee_text[request.length() + 1];
  memset(frisbee_text, 0, request.length() + 1);

  request_index = request.indexOf("/frisbee");
  if (request_index == -1)
  {
    Serial.println("didn't find frisbee string");
  }
  else
  {
    Serial.println("found frisbee string");

    // copy string until we find a space
    for (request_index = request_index+9; request_index < request.length(); request_index++)
    {
      if (request.charAt(request_index) == ' ')
      {
        Serial.print("Setting text to: ");
        Serial.println(frisbee_text);

        strncpy(display_string, frisbee_text, 20);
        char_index = 0;
        column_index = 0;
        return;
        
      }
      else
      {
        // dangerous assumption:  the text we get is lower case, and we want upper.  
        // Add checking to make sure we've got valid chars later.
        frisbee_text[text_index] = request.charAt(request_index) - 32;
      }
      
      text_index++;
    }  // end of copying string
    
  } // end of "else found frisbee string"
  
}

#if 0
void process_wifi( void )
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // process the request
  process_request(req);
  
  // Prepare the response. Start with the common header:
  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Content-Type: text/html\r\n\r\n");
  client.print("<!DOCTYPE HTML>\r\n<html>\r\n");
  client.print("<h2><font color=#f6a343>Glenn's Frisbee</h2>\r\n");
  client.print("Text set to ");
  client.print(display_string);
  client.print("<br>");
  client.print("</html>\n");

  delay(1);
  Serial.println("Client disonnected");
  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed

  
}
#endif

void handleRoot( void )
{
  server.send(200, "text/plain", "Hello World!!!");  
}

void handleNotFound( void )
{
  server.send(404, "text/plain", "404: Not found");
}

void setup_wifi( void )
{
  bool ret_val;
  
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

  server.on("/", handleRoot);
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

  //process_wifi();
  server.handleClient();
  
  display_strlen = strlen(display_string);

  if (char_index == display_strlen)
  {
    array = blank;
  }
  else
  {
    array = char_map[ (display_string[char_index] - 'A') ];
  }
  
  write_and_latch_byte(array[column_index]);
  
  // Serial.print("byte: ");
  // Serial.println(array[column_index]);

  column_index++;
  if (column_index == 6)
  {
    column_index = 0;
    char_index++;
    if (char_index == display_strlen + 1) char_index = 0;
  }

  delay(1);
  
}
