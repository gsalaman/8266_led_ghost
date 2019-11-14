/*=====================================================================
 * Using a 74595 to illustrate Shift Register concepts. 
 * 
 * See associated wiki for the wiring schematic.       
 */

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
int G[] = {0, 0x3E, 0x41, 0x41, 0x26, 0};

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
  static int char_index=0;
  static int column_index = 0;
  int *array;
  char display_string[10] = "GLENN";
  int display_strlen;

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
