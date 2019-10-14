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

// H:
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
// 1 1 1 1
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
int H[] = {0, 0x7F, 0x08, 0x08, 0x7f, 0};

// E:
// 1 1 1 1
// 1 0 0 0
// 1 0 0 0
// 1 1 1 1
// 1 0 0 0 
// 1 0 0 0
// 1 1 1 1
int E[] = {0, 0x7F, 0x49, 0x49, 0x49, 0};

// L:
// 1 0 0 0
// 1 0 0 0
// 1 0 0 0 
// 1 0 0 0
// 1 0 0 0
// 1 0 0 0
// 1 1 1 1
int L[] = {0, 0x7f, 0x01, 0x01, 0x01, 0};

// O:
// 0 1 1 0 
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
// 1 0 0 1
// 0 1 1 0
int O[] = {0, 0x3E, 041, 0x41, 0x3E, 0};

// empty:
int empty[] = {0, 0, 0, 0, 0, 0};

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

  switch (char_index)
  {
    case 0:
      array = H;
    break;

    case 1:
      array = E;
    break;

    case 2:
    case 3:
      array = L;
    break;

    case 4:
      array = O;
    break;

    case 5:
      array = empty;
    break;
  }

  write_and_latch_byte(array[column_index]);
  
  // Serial.print("byte: ");
  // Serial.println(array[column_index]);

  column_index++;
  if (column_index == 6)
  {
    column_index = 0;
    char_index++;
    if (char_index == 6) char_index = 0;
  }

  delay(1);
  
}
