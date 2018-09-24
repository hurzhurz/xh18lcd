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
void eeprom_save_odometer();
void eeprom_load_odometer();
void eeprom_save_configuration();
void eeprom_load_configuration();
void eeprom_setup();

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
uint16_t calc_crc16(uint8_t *buffer, uint8_t size);
void serial_loop();
void serial_setup();
#define SERIAL_SEND_INTERVAL (1000/15) // 15 messaged per second
#define SERIAL_RECEIVE_TIMEOUT ((1000/10)*10) // 10 messaged per second, timeout x10

// DATA


#define WHEELPERIMETER_CONSTRAIN(x) constrain(x,720,2400)
#define MAXSPEED_CONSTRAIN(x) constrain(x,14,32)

uint16_t odometer_km = 0;
uint16_t odometer_trip_tenthkm = 0;
uint8_t odometer_odo_tmp = 0;
double odometer_trip_tmp = 0.0;

void data_setup();
void data_loop();

uint8_t get_battery_soc();

void reset_motor_status();
void reset_motor_control();
void prepare_tx_data(uint8_t * tx_buffer);
void parse_rx_data(uint8_t * rx_buffer);
void update_setting_incdec(boolean inc_dec, uint8_t selection);


typedef struct _motor_controller_data
{
  uint16_t ui16_adc_battery_voltage;
  uint16_t ui16_battery_voltage_x10;
  uint8_t ui8_battery_current_x5;
  uint8_t ui8_motor_controller_state_1;
  uint8_t ui8_adc_throttle;
  uint8_t ui8_throttle;
  uint8_t ui8_adc_pedal_torque_sensor;
  uint8_t ui8_pedal_torque_sensor;
  uint8_t ui8_pedal_human_power;
  uint8_t ui8_duty_cycle;
  uint8_t ui8_error_code;
  uint16_t ui16_wheel_speed_x10;
  uint8_t ui8_motor_controller_state_2;
  uint8_t ui8_braking;
  uint8_t ui8_pedal_cadence;

  uint16_t ui16_motor_speed_erps;
  uint8_t ui8_foc_angle;
  uint8_t ui8_temperature_current_limiting_value;
  uint8_t ui8_motor_temperature;

  uint8_t last_package_id_sent;
  uint8_t last_package_id_confirm;
  unsigned long last_update;
} struct_motor_controller_data;

struct_motor_controller_data motor_controller_data;

typedef struct _control_variables
{
  uint8_t ui8_assist_level;
  uint8_t ui8_lights;
  uint8_t ui8_walk_assist_level;
  uint8_t ui8_offroad_mode;
  uint8_t ui8_cruise_control;
} struct_control_variables;

struct_control_variables control_variables = {.ui8_assist_level = 1, .ui8_lights = 0, .ui8_walk_assist_level = 0, .ui8_offroad_mode = 0, .ui8_cruise_control=0};

typedef struct _configuration_variables
{
  uint8_t ui8_number_of_assist_levels;
  uint16_t ui16_wheel_perimeter;
  uint8_t ui8_wheel_max_speed;
  uint8_t ui8_units_type;
  uint32_t ui32_wh_x10_offset;
  uint32_t ui32_wh_x10_100_percent;
  uint8_t ui8_show_numeric_battery_soc;
  uint8_t ui8_odometer_field_state;
  uint8_t ui8_target_max_battery_power;
  uint8_t ui8_battery_cells_number;
  uint8_t ui8_battery_max_current;
  uint16_t ui16_battery_low_voltage_cut_off_x10;
  uint16_t ui16_battery_voltage_reset_wh_counter_x10;
  uint16_t ui16_battery_pack_resistance_x1000;
  uint8_t ui8_motor_voltage_type;
  uint8_t ui8_motor_assistance_startup_without_pedal_rotation;
  uint8_t ui8_pas_max_cadence;
  uint8_t ui8_assist_level_power [5];
  uint8_t ui8_startup_motor_power_boost_state;
  uint8_t ui8_startup_motor_power_boost_time;
  uint8_t ui8_startup_motor_power_boost_fade_time;
  uint8_t ui8_startup_motor_power_boost [5];
  uint16_t ui16_adc_motor_temperature_10b;
  uint8_t ui8_throttle_adc_measures_motor_temperature;
  uint8_t ui8_motor_temperature_min_value_to_limit;
  uint8_t ui8_motor_temperature_max_value_to_limit;
  uint8_t ui8_temperature_field_config;
  uint8_t ui8_lcd_power_off_time_minutes;
  uint8_t ui8_lcd_backlight_on_brightness;
  uint8_t ui8_lcd_backlight_off_brightness;
  uint8_t ui8_offroad_func_enabled;
  uint8_t ui8_offroad_enabled_on_startup;
  uint8_t ui8_offroad_speed_limit;
  uint8_t ui8_offroad_power_limit_enabled;
  uint8_t ui8_offroad_power_limit_div25;
} struct_configuration_variables;
struct_configuration_variables configuration_variables;


#define SETUP_ITEMS_NUM 15
#define SETUP_ITEMS_PER_PAGE 8
#define SETUP_PAGES_NUM (( (uint8_t)(SETUP_ITEMS_NUM-1) / SETUP_ITEMS_PER_PAGE) +1 )
#define SETUP_ITEM_PAGE_POSITION(x) ( (uint8_t)x % SETUP_ITEMS_PER_PAGE)
#define SETUP_ITEM_PAGE_ID(x) ( (uint8_t)x / SETUP_ITEMS_PER_PAGE )

typedef struct _setup_flags
{
  uint8_t reset_config;
  uint8_t reset_trip;
  uint8_t reset_odometer;
} struct_setup_flags;
struct_setup_flags setup_flags = {.reset_config = 0, .reset_trip = 0};

typedef struct _setup_item
{
  char description[13]; // 12 chars possible + 0x00
  uint8_t * pointer;
  bool is16bit;
  uint16_t step;
  uint16_t min;
  uint16_t max;
} struct_setup_item;

struct_setup_item setup_items[SETUP_ITEMS_NUM] = {
  {
    .description = "Max Speed",
    .pointer = (uint8_t*) & (configuration_variables.ui8_offroad_speed_limit),
    .is16bit = 0,
    .step = 1,
    .min = 14,
    .max = 32
  },
  {
    .description = "MaxPower*25W",
    .pointer = (uint8_t*) & (configuration_variables.ui8_offroad_power_limit_div25),
    .is16bit = 0,
    .step = 1,
    .min = 0,
    .max = 40
  },
  {
    .description = "Wheel Perim",
    .pointer = (uint8_t*) & (configuration_variables.ui16_wheel_perimeter),
    .is16bit = 1,
    .step = 10,
    .min = 720,
    .max = 2400
  },
  {
    .description = "0=km 1=mi",
    .pointer = (uint8_t*) & (configuration_variables.ui8_units_type),
    .is16bit = 0,
    .step = 1,
    .min = 0,
    .max = 1
  },
  {
    .description = "Bat Max Curr",
    .pointer = (uint8_t*) & (configuration_variables.ui8_battery_max_current),
    .is16bit = 0,
    .step = 1,
    .min = 1,
    .max = 30
  },
  {
    .description = "Bat Cell Num",
    .pointer = (uint8_t*) & (configuration_variables.ui8_battery_cells_number),
    .is16bit = 0,
    .step = 1,
    .min = 1,
    .max = 20
  },
  {
    .description = "Bat Cutoff V",
    .pointer = (uint8_t*) & (configuration_variables.ui16_battery_low_voltage_cut_off_x10),
    .is16bit = 1,
    .step = 5,
    .min = 0,
    .max = 600
  },
  {
    .description = "BatPower*25W",
    .pointer = (uint8_t*) & (configuration_variables.ui8_target_max_battery_power),
    .is16bit = 0,
    .step = 1,
    .min = 0,
    .max = 40
  },
  {
    .description = "Mo 0=48 1=36",
    .pointer = (uint8_t*) & (configuration_variables.ui8_motor_voltage_type),
    .is16bit = 0,
    .step = 1,
    .min = 0,
    .max = 1
  },
  {
    .description = "Ass w/o Pade",
    .pointer = (uint8_t*) & (configuration_variables.ui8_motor_assistance_startup_without_pedal_rotation),
    .is16bit = 0,
    .step = 1,
    .min = 0,
    .max = 1
  },
  {
    .description = "Max Cadence",
    .pointer = (uint8_t*) & (configuration_variables.ui8_pas_max_cadence),
    .is16bit = 0,
    .step = 1,
    .min = 0,
    .max = 255
  },
  {
    .description = "Offroad mode",
    .pointer = (uint8_t*) & (control_variables.ui8_offroad_mode),
    .is16bit = 0,
    .step = 1,
    .min = 0,
    .max = 1
  },
  {
    .description = "Reset Config",
    .pointer = (uint8_t*) & (setup_flags.reset_config),
    .is16bit = 0,
    .step = 1,
    .min = 0,
    .max = 1
  },
  {
    .description = "Reset Trip",
    .pointer = (uint8_t*) & (setup_flags.reset_trip),
    .is16bit = 0,
    .step = 1,
    .min = 0,
    .max = 1
  },
  {
    .description = "Reset ODO",
    .pointer = (uint8_t*) & (setup_flags.reset_odometer),
    .is16bit = 0,
    .step = 1,
    .min = 0,
    .max = 1
  }
};


#define LI_ION_CELL_VOLTS_100   4.06
#define LI_ION_CELL_VOLTS_90    4.00 // (100+80)/2
#define LI_ION_CELL_VOLTS_80    3.93
#define LI_ION_CELL_VOLTS_70    3.86 // (80+60)/2
#define LI_ION_CELL_VOLTS_60    3.78
#define LI_ION_CELL_VOLTS_50    3.69 // (60+40)/2
#define LI_ION_CELL_VOLTS_40    3.60
#define LI_ION_CELL_VOLTS_30    3.49 // (40+20)/2
#define LI_ION_CELL_VOLTS_20    3.38
#define LI_ION_CELL_VOLTS_10    3.25
#define LI_ION_CELL_VOLTS_0 3.00

#define ADC_BATTERY_VOLTAGE_PER_ADC_STEP_X10000 863



#define DEFAULT_VALUE_ASSIST_LEVEL                                  3
#define DEFAULT_VALUE_NUMBER_OF_ASSIST_LEVELS                       5
#define DEFAULT_VALUE_WHEEL_PERIMETER                               2250 // 2050 // 26'' wheel: 2050mm perimeter (2 + (8 << 8))
#define DEFAULT_VALUE_WHEEL_MAX_SPEED                               50
#define DEFAULT_VALUE_UNITS_TYPE                                    0 // 0 = km/h
#define DEFAULT_VALUE_WH_OFFSET                                     0
#define DEFAULT_VALUE_HW_X10_100_PERCENT                            0
#define DEAFULT_VALUE_SHOW_NUMERIC_BATTERY_SOC                      0
#define DEFAULT_VALUE_ODOMETER_FIELD_STATE                          0
#define DEFAULT_VALUE_BATTERY_MAX_CURRENT                           16 // 16 amps
#define DEFAULT_VALUE_TARGET_MAX_BATTERY_POWER                      20 // 20 * 25 = 500
#define DEFAULT_VALUE_BATTERY_CELLS_NUMBER                          10 // 13 --> 48V
#define DEFAULT_VALUE_BATTERY_LOW_VOLTAGE_CUT_OFF_X10_0             300 // 48v battery, LVC = 39.0 (3.0 * 13): (134 + (1 << 8))
#define DEFAULT_VALUE_PAS_MAX_CADENCE                               110 // 110 RPM
#define DEFAULT_VALUE_CONFIG_0                                      0 // ui8_motor_voltage_type = 0; ui8_motor_assistance_startup_config = 0
#define DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_1                         16 // 400W x
#define DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_2                         32
#define DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_3                         48
#define DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_4                         64
#define DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_5                         80
#define DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_STATE               1
#define DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_ASSIST_LEVEL_1      8 // 200W
#define DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_ASSIST_LEVEL_2      22
#define DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_ASSIST_LEVEL_3      34
#define DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_ASSIST_LEVEL_4      46
#define DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_ASSIST_LEVEL_5      58

#define DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_TIME                25 // 2.5 seconds
#define DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_FADE_TIME           25 // 2.5 seconds
#define DEFAULT_VALUE_MOTOR_TEMPERATURE_MIN_VALUE_LIMIT             75 // 75 degrees celsius
#define DEFAULT_VALUE_MOTOR_TEMPERATURE_MAX_VALUE_LIMIT             85
#define DEFAULT_VALUE_BATTERY_VOLTAGE_RESET_WH_COUNTER_X10          417 // 48v battery, 54.2 volts fully charged = 54.2: (30 + (2 << 8))
#define DEFAULT_VALUE_LCD_POWER_OFF_TIME                            15 // 15 minutes, each unit 1 minute
#define DEFAULT_VALUE_LCD_BACKLIGHT_ON_BRIGHTNESS                   16 // 16 = 80%
#define DEFAULT_VALUE_LCD_BACKLIGHT_OFF_BRIGHTNESS                  1 // 1 = 5%
#define DEFAULT_VALUE_BATTERY_PACK_RESISTANCE                       130 // 48v battery, 13S5P measured 130 milli ohms
#define DEFAULT_VALUE_OFFROAD_FUNC_ENABLED                          0
#define DEFAULT_VALUE_OFFROAD_MODE_ENABLED_ON_STARTUP               0
#define DEFAULT_VALUE_OFFROAD_SPEED_LIMIT                           25
#define DEFAULT_VALUE_OFFROAD_POWER_LIMIT_ENABLED                   0
#define DEFAULT_VALUE_OFFROAD_POWER_LIMIT_DIV25 10 //10 * 25 = 250W


