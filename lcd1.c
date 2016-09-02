#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define D6_port PORTD    // lcd D7 connection
#define D7_bit PORTD7
#define D7_ddr DDRD

#define D6_port PORTD    // lcd D6 connection
#define D6_bit PORTD6
#define D6_ddr DDRD

#define D5_port PORTD    // lcd D5 connection
#define D5_bit PORTD5
#define D5_ddr DDRD

#define D4_port PORTD    // lcd D4 connection
#define D4_bit PORTD4
#define D4_ddr DDRD

#define E_port PORTB     // lcd Enable pin
#define E_bit PORTB1
#define E_ddr DDRB

#define RS_port PORTB    // lcd Register Select pin
#define RS_bit PORTB0
#define RS_ddr DDRB      // LCD module information

#define Lineone 0x00     // start of line 1
#define Linetwo 0x40     // start of line 2

#define lcd_clear 0b00000001                 // replace all characters with ASCII 'space'
#define lcd_Home 0b00000010                  // return cursor to first position on first line
#define lcd_EntryMode 0b00000110             // shift cursor from left to right on read/write
#define lcd_DisplayOff 0b00001000            // turn display off
#define lcd_DisplayOn 0b00001100             // display on, cursor off, don't blink character
#define lcd_FunctionReset 0b00110000         // reset the LCD
#define lcd_FunctionSet4bit 0b00101000       // 4-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor 0b10000000             // set cursor position



uint8_t program_author[] = "Bhavna";
uint8_t program_version[] = "Vidisha";
uint8_t program_date[] = "Sep 16, 2013";


void lcd_write_4(uint8_t);
void lcd_write_instruction_4d(uint8_t);
void lcd_write_character_4d(uint8_t);
void lcd_write_string_4d(uint8_t *);
void lcd_init_4d(void);



int main(void)
{
    D7_ddr |= (1<<D7_bit);    // 4 data lines - output
    D6_ddr |= (1<<D6_bit);
    D5_ddr |= (1<<D5_bit);
    D4_ddr |= (1<<D4_bit);    // configure the microprocessor pins for the control lines
    
    E_ddr |= (1<<E_bit);      // E line - output
    RS_ddr |= (1<<RS_bit);    // RS line - output
    
    // initialize the LCD controller as determined by the defines (LCD instructions)
    
    while(1)
    {
        lcd_init_4d();        // initialize the LCD display for a 4-bit interface // display the first line of information
        
        lcd_write_string_4d(program_author);    // set cursor to start of second line
        _delay_ms(2000);
        
        lcd_init_4d();
        
        lcd_write_string_4d(program_version); // endless loop
        _delay_ms(2000);
    }
    
    return 0;
}



void lcd_init_4d(void)
{
    // Power-up delay
    
    _delay_ms(100);                     // initial 40 mSec delay
    RS_port &= ~(1<<RS_bit);            // select the Instruction Register (RS low)
    E_port &= ~(1<<E_bit);              // make sure E is initially low // Reset the LCD controller
    
    lcd_write_4(lcd_FunctionReset);     // first part of reset sequence
    _delay_ms(10);                      // 4.1 mS delay (min)
    
    lcd_write_4(lcd_FunctionReset);     // second part of reset sequence
    _delay_us(200);                     // 100uS delay (min)
    
    lcd_write_4(lcd_FunctionReset);     // third part of reset sequence
    _delay_us(200);                     // this delay is omitted in the data sheet
    
    lcd_write_4(lcd_FunctionSet4bit);   // set 4-bit mode
    _delay_us(80);                      // 40uS delay (min)  // Function Set instruction
    
    lcd_write_instruction_4d(lcd_FunctionSet4bit);      // set mode, lines, and font
    _delay_us(80);                                      // 40uS delay (min)
    
    lcd_write_instruction_4d(lcd_DisplayOff);           // turn display OFF
    _delay_us(80);                                      // 40uS delay (min) // Clear Display instruction
    
    lcd_write_instruction_4d(lcd_clear);                // clear display RAM
    _delay_ms(4);                                       // 1.64 mS delay (min) // Entry Mode Set instruction
    
    lcd_write_instruction_4d(lcd_EntryMode);            // set desired shift characteristics
    _delay_us(80);                                      // 40uS delay (min)
    
    lcd_write_instruction_4d(lcd_DisplayOn);            // turn the display ON
    _delay_us(80);                                      // 40uS delay (min)
}




void lcd_write_string_4d(uint8_t theString[])
{
    volatile int i = 0;                                 // character counter
    while (theString[i] != 0)
    {
        lcd_write_character_4d(theString[i]);
        i++;
        _delay_us(80);                                    // 40 uS delay (min)
    }
}



void lcd_write_character_4d(uint8_t theData)
{
    RS_port |= (1<<RS_bit);                 // select the Data Register (RS high)
    E_port &= ~(1<<E_bit);                  // make sure E is initially
    lcd_write_4(theData);                   // write the upper 4-bits of the data
    lcd_write_4(theData << 4);              // write the lower 4-bits of the data
}



void lcd_write_instruction_4d(uint8_t theInstruction)
{
    RS_port &= ~(1<<RS_bit);                // select the Instruction Register (RS low)
    E_port &= ~(1<<E_bit);                  // make sure E is initially low
    lcd_write_4(theInstruction);            // write the upper 4-bits of the data
    lcd_write_4(theInstruction << 4);       // write the lower 4-bits of the data
}



void lcd_write_4(uint8_t theByte)
{
    D6_port &= ~(1<<D7_bit);                        // assume that data is '0'
    if (theByte & 1<<7) D6_port |= (1<<D7_bit);     // make data = '1' if necessary
    
    D6_port &= ~(1<<D6_bit);                        // repeat for each data bit
    if (theByte & 1<<6) D6_port |= (1<<D6_bit);
    
    D5_port &= ~(1<<D5_bit);
    if (theByte & 1<<5) D5_port |= (1<<D5_bit);
    
    D4_port &= ~(1<<D4_bit);
    if (theByte & 1<<4) D4_port |= (1<<D4_bit);     // write the data // 'Address set-up time' (40 nS)
    
    E_port |= (1<<E_bit);                           // Enable pin high
    _delay_us(1);                                           // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)
    E_port &= ~(1<<E_bit);                          // Enable pin low
    _delay_us(1);                                           // implement 'Data hold time' (10 nS) and 'Enable cycle time' (500 nS)
}



