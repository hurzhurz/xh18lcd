// definitions and configuration

#define POWER_LATCH PD7

#define BUTTON_K1_ONOFF PB7
#define BUTTON_K2 PB6

#define GRIP_HALL_SENSOR A7

#define LCD_LED_1 PD2
#define LCD_LED_2 PD3

#define LCD_CS PA2
#define LCD_RESET PA3
#define LCD_RS PA4
#define LCD_WR PA5
#define LCD_RD PA6







// MAIN

unsigned long loop_time_now;
void setup();
void loop();




// POWER
void power_setup();

void power_loop();
void power_shutdown();

// EEPROM
void eeprom_save();
void eeprom_setup();
struct eeprom_data
{
  uint8_t max_speed;
  uint8_t wheel_size;
  uint16_t odometer;
} eeprom_data;

// BUTTON
#define PRESSTIME_LONG 1000
#define PRESSTIME_DOUBLE 500

#define GRIP_DOWN 0
#define GRIP_MID  1
#define GRIP_UP   2
#define isButtonPressed(x) (digitalRead(x)==LOW)
void button_setup();
void button_loop();
uint8_t getGripState();

#define BUTTON_POWER  0
#define BUTTON_INFO   1
#define BUTTON_UP     2
#define BUTTON_DOWN   3

#define BUTTON_COUNT  4

#define PRESSSTATE_NONE         0
#define PRESSSTATE_SINGLE       1
#define PRESSSTATE_SINGLE_WAIT  2
#define PRESSSTATE_DOUBLE       3
#define PRESSTYPE_SHORT   0
#define PRESSTYPE_LONG    1


struct button_state
{
  boolean isHandled;
  boolean isPressed;
  boolean disableDouble;
  uint8_t pressState;
  uint8_t pressType;
  unsigned long pressStartTime;
  unsigned long pressDuration;
} button_states[BUTTON_COUNT];

// LCD


#define SCREEN_HELLO    0
#define SCREEN_MAIN     1
#define SCREEN_SETUP    2
#define SCREEN_GOODBYE  3

#define SETUP_SELECTION_SPEED 0
#define SETUP_SELECTION_WHEEL 1

#define LCD_OFFSET_X -1
#define LCD_OFFSET_Y 2


#define LCD_SIZE_X 160
#define LCD_SIZE_Y 128

#define LCD_BACKLIGHT_OFF   0
#define LCD_BACKLIGHT_LOW   1
#define LCD_BACKLIGHT_HIGH  2

#include <UTFT.h>
extern uint8_t SmallFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t BigFont[];

uint8_t setup_screen_selection = 0;

void display_set_backlight(uint8_t brightness);
void display_hello_screen();
void display_main_screen();
void display_goodbye_screen();
void display_setup();
void display_loop();

// SERIAL

uint8_t F_chk_8( uint8_t bval, uint8_t cval );
void serial_loop();
void serial_setup();
#define SERIAL_SEND_INTERVAL (1000/15) // 15 messaged per second
#define SERIAL_RECEIVE_TIMEOUT ((1000/8)*3) // 8 messaged per second, timeout x3

// DATA

#define WHEELSIZE_DEFAULT 28
#define MAXSPEED_DEFAULT 25
#define WHEELSIZE_CONSTRAIN(x) constrain(x,9,29)
#define MAXSPEED_CONSTRAIN(x) constrain(x,14,32)

uint16_t odometer_km = 0;
uint16_t odometer_trip_tenthkm = 0;
uint8_t odometer_odo_tmp = 0;
double odometer_trip_tmp = 0.0;

void data_setup();
void data_loop();

void reset_motor_status();
void reset_motor_control();
void prepare_tx_data(uint8_t * tx_buffer);
void parse_rx_data(uint8_t * rx_buffer);
double getSpeed_kph();
uint8_t getBattery_percent();
void update_setting_incdec(boolean inc_dec, uint8_t selection);

struct data_for_motor
{
  boolean headlight;
  boolean enable_6kph;
  uint8_t assist_level;
  uint8_t wheel_size;
  uint8_t max_speed;
  unsigned long last_sent;
} motor_control;
struct data_from_motor
{
  uint8_t battery;
  boolean low_voltage;
  boolean motor_running;
  boolean pedals_spinning;
  uint8_t pedal_torque;
  uint8_t error_code;
  double rotations_per_second;
  unsigned long last_update;
} motor_status;






