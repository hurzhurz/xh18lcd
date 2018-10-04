

uint8_t current_screen = -1;
unsigned long display_last_refresh = 0;

void display_set_backlight(uint8_t brightness)
{
  digitalWrite(LCD_LED_1, brightness>=LCD_BACKLIGHT_LOW);
  digitalWrite(LCD_LED_2, brightness>=LCD_BACKLIGHT_HIGH);
}

void display_setup()
{
  // Power on display backlight on the lower of two steps
  pinMode(LCD_LED_1, OUTPUT);
  pinMode(LCD_LED_2, OUTPUT);
  digitalWrite(LCD_LED_1, HIGH);
  digitalWrite(LCD_LED_2, LOW);
  display_set_backlight(LCD_BACKLIGHT_LOW);

  // /RD of LCD not used, set it to HIGH
  pinMode(LCD_RD, OUTPUT);
  digitalWrite(LCD_RD, HIGH);
  
  UTFT(ST7735_8, LCD_RS , LCD_WR , LCD_CS , LCD_RESET);

  // LCD of my XH18-LCD had an offset...
  UTFT_offset_x = LCD_OFFSET_X;
  UTFT_offset_y = LCD_OFFSET_Y;

  UTFT_InitLCD();
  display_hello_screen();
  
}

void display_hello_screen()
{
  if(current_screen!=SCREEN_HELLO)
  {
    current_screen = SCREEN_HELLO;
    display_last_refresh = loop_time_now;
    UTFT_clrScr();
    UTFT_setFont(BigFont);
    UTFT_setColor(VGA_WHITE);
    UTFT_print("Moin!", (LCD_SIZE_X-16*5)/2 , (LCD_SIZE_X-12*2)/2 );
  }
  else if((loop_time_now-display_last_refresh) >= 3000)
  {
    display_main_screen();
  }
}

void display_goodbye_screen()
{
  if(current_screen!=SCREEN_GOODBYE)
  {
    current_screen = SCREEN_GOODBYE;
    display_last_refresh = loop_time_now;
    UTFT_clrScr();
    UTFT_setFont(BigFont);
    UTFT_setColor(VGA_WHITE);
    UTFT_print("Ade!", (LCD_SIZE_X-16*5)/2 , (LCD_SIZE_X-12*2)/2 );
  }
  else if((loop_time_now-display_last_refresh) >= 2000 && !button_states[BUTTON_POWER].isPressed)
  {
    power_shutdown();
  }
}

// SCREEN_MAIN

void display_main_print_stepnum(uint8_t step)
{
    switch(step)
    {
      case 0:
        UTFT_print("Off", 5, 107);
        break;
      case 1:
        UTFT_print("1", 57, 107);
        break;
      case 2:
        UTFT_print("2", 77, 107);
        break;
      case 3:
        UTFT_print("3", 97, 107);
        break;
      case 4:
        UTFT_print("4", 117, 107);
        break;
      case 5:
        UTFT_print("5", 137, 107);
        break;
    }
}
void display_main_print_speed(uint16_t speed)
{
    UTFT_setColor(VGA_AQUA);
    UTFT_setFont(SevenSegNumFont);
    if(configuration_variables.ui8_units_type) // miles
      speed = (speed*5)/8;
    if(speed>999) speed=999;
    UTFT_printNumI_adv(speed, 20, 15, 3, '0');
    UTFT_fillCircle(20+2*32-1,15+50-2-2-1,2);
}

void display_main_print_odometer_text()
{
  UTFT_setFont(SmallFont);
  UTFT_setColor(VGA_WHITE);
  UTFT_print("ODO",  10, 70);
  UTFT_print("Trip", 10, 85);
  if(configuration_variables.ui8_units_type) // miles
  {
    UTFT_print("mi",   139, 70);
    UTFT_print("mi",   139, 85);
  }
  else
  {
    UTFT_print("km",   139, 70);
    UTFT_print("km",   139, 85);
  }
}
void display_main_print_odometer(uint16_t odometer_km)
{
  UTFT_setColor(VGA_WHITE);
  UTFT_setFont(SmallFont);
  if(configuration_variables.ui8_units_type) // miles
    odometer_km = (odometer_km*5)/8;
  UTFT_printNumI_adv(odometer_km,  90,70,6,' ');
}
void display_main_print_odometer_trip(uint16_t odometer_trip_tenthkm)
{
  UTFT_setColor(VGA_WHITE);
  UTFT_setFont(SmallFont);
  if(configuration_variables.ui8_units_type) // miles
    odometer_trip_tenthkm = (odometer_trip_tenthkm*5)/8;
  UTFT_printNumF_adv( ((double)odometer_trip_tenthkm)/10.0 ,1,90,85,'.',4,' ');
}

void display_main_print_6kmh(boolean state)
{
  if (state)
    UTFT_setColor_rgb(0, 255, 0);
  else
    UTFT_setColor_rgb(255, 0, 0);
  UTFT_fillRect(150, 5, 159, 14);
}
void display_main_print_light(boolean state)
{
  if (state)
    UTFT_setColor_rgb(0, 0, 255);
  else
    UTFT_setColor_rgb(255, 255, 255);
  UTFT_fillRect(150, 15, 159, 24);
}

void display_main_print_battery_border(boolean batlow)
{
    UTFT_setColor(batlow?VGA_RED:VGA_WHITE);
    UTFT_drawRect(121, 30, 155, 45);
}

void display_main_print_battery_bar(uint8_t percent)
{
  uint8_t bar_length = (uint8_t)(31*(uint16_t)percent / 100);
  if(bar_length>0)
  {
    if(percent<=25)
      UTFT_setColor(VGA_RED);
    else if(percent<=50)
      UTFT_setColor(VGA_YELLOW);
    else
      UTFT_setColor(VGA_GREEN);
    UTFT_fillRect(123, 32, (153-(31-bar_length)), 43);
  }
  if(bar_length<31)
  {
    UTFT_setColor(VGA_BLACK);
    UTFT_fillRect((153-(31-bar_length)+1), 32, 153, 43);
  }
}

void display_main_print_error(uint8_t error)
{
  char errstr[3]={0x20,0x20,0x00};
  if(error>0)
  {
    errstr[0]=error/16;
    errstr[0]+=(errstr[0]>9)?0x41:0x30;
    errstr[1]=error%16;
    errstr[1]+=(errstr[1]>9)?0x41:0x30;
  }
  UTFT_setColor(VGA_RED);
  UTFT_setBackColor(VGA_BLACK);
  UTFT_setFont(BigFont);
  UTFT_print(errstr,121,50);
}

void display_main_screen()
{
  static uint8_t old_step, old_battery, old_error;
  static boolean old_6kmh, old_light, old_battery_low;
  static uint16_t old_odometer, old_odometer_trip, old_speed;
  //static int old_speed;
  //int new_speed = (int)(getSpeed_kph()*10);
  uint8_t new_battery = get_battery_soc();
  

  
  if(new_battery>100)
    new_battery=100;
  if(current_screen!=SCREEN_MAIN)
  {
    button_states[BUTTON_UP].disableDouble=true;
    button_states[BUTTON_DOWN].disableDouble=true;
    button_states[BUTTON_POWER].disableDouble=false;
    button_states[BUTTON_INFO].disableDouble=true;
    
    current_screen = SCREEN_MAIN;
    UTFT_clrScr();
    UTFT_setFont(SmallFont);
    UTFT_setColor(VGA_WHITE);
    UTFT_setBackColor(VGA_BLACK);
    if(configuration_variables.ui8_units_type)
      UTFT_print("mph", 20+3*32+5, 15);
    else
      UTFT_print("kmh", 20+3*32+5, 15);

    display_main_print_error(old_error=motor_controller_data.ui8_error_code);
    display_main_print_battery_border(old_battery_low=(new_battery==0));
    display_main_print_battery_bar(old_battery=new_battery);
    display_main_print_speed(old_speed=motor_controller_data.ui16_wheel_speed_x10);
    display_main_print_odometer_text();
    display_main_print_odometer(old_odometer=odometer_km);
    display_main_print_odometer_trip(old_odometer_trip=odometer_trip_tenthkm);
    display_main_print_6kmh(old_6kmh=control_variables.ui8_walk_assist_level);
    display_main_print_light(old_light=control_variables.ui8_lights);

    UTFT_setColor(VGA_GRAY);
    UTFT_fillRect(0,104,159,105);
    for(uint8_t i=0;i<5;i++)
      UTFT_fillRect(54+i*20,106,55+i*20,127);
    UTFT_setFont(BigFont);
    UTFT_setColor(VGA_BLACK);
    UTFT_setBackColor(VGA_WHITE);
    display_main_print_stepnum(control_variables.ui8_assist_level);
    old_step = control_variables.ui8_assist_level;
    UTFT_setColor(VGA_WHITE);
    UTFT_setBackColor(VGA_BLACK);
    for(uint8_t i=0;i<=5;i++)
      if(i!=control_variables.ui8_assist_level)
        display_main_print_stepnum(i);

  } 
  else if((loop_time_now-display_last_refresh) < 250)
  {
    return;
  }
  if(old_error!=motor_controller_data.ui8_error_code)
  {
    display_main_print_error(old_error=motor_controller_data.ui8_error_code);
  }
  if(old_battery_low!=(new_battery==0))
  {
    display_main_print_battery_border(old_battery_low=(new_battery==0));
  }
  if(old_battery!=new_battery)
  {
    display_main_print_battery_bar(old_battery=new_battery);
  }
  if(old_speed!=motor_controller_data.ui16_wheel_speed_x10)
  {
    display_main_print_speed(old_speed=motor_controller_data.ui16_wheel_speed_x10);
  }

  if(old_odometer!=odometer_km)
  {
    display_main_print_odometer(old_odometer=odometer_km);
  }
  if(old_odometer_trip!=odometer_trip_tenthkm)
  {
    display_main_print_odometer_trip(old_odometer_trip=odometer_trip_tenthkm);
  }

  if(old_6kmh!=control_variables.ui8_walk_assist_level)
  {
    display_main_print_6kmh(old_6kmh=control_variables.ui8_walk_assist_level);
  }
  if(old_light!=control_variables.ui8_lights)
  {
    display_main_print_light(old_light=control_variables.ui8_lights);
  }

  if(old_step!=control_variables.ui8_assist_level)
  {
    UTFT_setFont(BigFont);
    UTFT_setColor(VGA_BLACK);
    UTFT_setBackColor(VGA_WHITE);
    display_main_print_stepnum(control_variables.ui8_assist_level);
    UTFT_setColor(VGA_WHITE);
    UTFT_setBackColor(VGA_BLACK);
    display_main_print_stepnum(old_step);
    old_step = control_variables.ui8_assist_level;
  }

  // debug
  // UTFT_setFont(SmallFont);
  // UTFT_setColor(VGA_WHITE);
  // UTFT_printNumI(motor_controller_data.last_package_id_sent,10, 50);

  display_last_refresh = loop_time_now;
}

// SCREEN_SETUP

void display_setup_print_name(uint8_t num)
{
  uint8_t posy = 3+3+15*SETUP_ITEM_PAGE_POSITION(num);
  if(num>=SETUP_ITEMS_NUM) return;
  struct_setup_item * item = &(setup_items[num]);
  UTFT_print(item->description, 6, posy);
}

void display_setup_print_value(uint8_t num)
{
  uint8_t posy = 3+3+15*SETUP_ITEM_PAGE_POSITION(num);
  if(num>=SETUP_ITEMS_NUM) return;
  UTFT_setFont(SmallFont);
  UTFT_setColor(VGA_WHITE);
  struct_setup_item * item = &(setup_items[num]);
  uint16_t tmpval = (item->is16bit)?( *((uint16_t*)(item->pointer))  ):(  *((uint8_t*)(item->pointer))  );
  UTFT_printNumI_adv(tmpval, 123, posy, 4, ' ');

}
void display_setup_screen_print_select_marker(uint8_t num)
{
  uint8_t posy = 3+3+3+15*SETUP_ITEM_PAGE_POSITION(num);
  UTFT_fillRect(107-5,posy,107,posy+5);
}

void display_setup_screen()
{
  static uint8_t old_selection=0;
  static uint8_t old_page=0;
  uint8_t page = SETUP_ITEM_PAGE_ID(setup_screen_selection);

  if(current_screen!=SCREEN_SETUP || old_page!=page)
  {
    current_screen = SCREEN_SETUP;
    display_last_refresh = loop_time_now;
    button_states[BUTTON_UP].disableDouble=true;
    button_states[BUTTON_DOWN].disableDouble=true;
    button_states[BUTTON_POWER].disableDouble=true;
    button_states[BUTTON_INFO].disableDouble=true;
    UTFT_clrScr();
    UTFT_setFont(SmallFont);
    UTFT_setColor(VGA_WHITE);
    UTFT_setBackColor(VGA_BLACK);
    
    // draw table borders
    UTFT_drawRect(3,3,156,124);
    for(uint8_t i=1;i<SETUP_ITEMS_PER_PAGE;i++)
      UTFT_drawLine(4, 3+1+15*i, 155, 3+1+15*i);
    UTFT_drawLine(110, 4, 110, 123);

    // print items
    for(uint8_t i=0;i<SETUP_ITEMS_PER_PAGE;i++)
    {
      uint8_t i_abs = page * SETUP_ITEMS_PER_PAGE + i;
      if(i_abs >= SETUP_ITEMS_NUM)
        break;
      display_setup_print_name(i_abs);
      display_setup_print_value(i_abs);
    }
    
    UTFT_setColor(VGA_BLUE);
    display_setup_screen_print_select_marker(setup_screen_selection);
    old_selection = setup_screen_selection;
    old_page = page;
  }
  if(old_selection != setup_screen_selection)
  {
    UTFT_setColor(VGA_BLACK);
    display_setup_screen_print_select_marker(old_selection);
    UTFT_setColor(VGA_BLUE);
    display_setup_screen_print_select_marker(old_selection = setup_screen_selection);
  }
}

void display_loop()
{
  switch(current_screen)
  {
    case SCREEN_HELLO:
      display_hello_screen();
      break;
    case SCREEN_MAIN:
      display_main_screen();
      break;
    case SCREEN_SETUP:
      display_setup_screen();
      break;
    case SCREEN_GOODBYE:
      display_goodbye_screen();
      break;
    default:
      display_main_screen();
  }
}

