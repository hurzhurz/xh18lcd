void data_setup()
{
  
}

void data_loop()
{
  static unsigned long last_update = 0;

  if (motor_controller_data.last_update != 0 && (loop_time_now - motor_controller_data.last_update) > SERIAL_RECEIVE_TIMEOUT)
  {
    reset_motor_status();
  }

  if( motor_controller_data.last_update!=last_update )
  {
    if(last_update!=0 && motor_controller_data.last_update!=0)
    {
      unsigned long t_delta_ms = motor_controller_data.last_update - last_update;
      double speed = (double)motor_controller_data.ui16_wheel_speed_x10 / 10.0;
      odometer_trip_tmp += speed*t_delta_ms / 3600.0;
    }
    last_update = motor_controller_data.last_update;
  }
  while(odometer_trip_tmp>=100.0)
  {
    odometer_trip_tenthkm++;
    odometer_odo_tmp++;
    odometer_trip_tmp -= 100.0;
  }
  while(odometer_odo_tmp>=10)
  {
    odometer_km++;
    odometer_odo_tmp -= 10;
    eeprom_save_odometer();
  }
}

uint8_t get_battery_soc()
{
  float cell_voltage = (float)motor_controller_data.ui16_battery_voltage_x10 / (float)( 10*configuration_variables.ui8_battery_cells_number);
  if(cell_voltage>LI_ION_CELL_VOLTS_100)
    return 100;
  if(cell_voltage>LI_ION_CELL_VOLTS_90)
    return  90;
  if(cell_voltage>LI_ION_CELL_VOLTS_80)
    return  80;
  if(cell_voltage>LI_ION_CELL_VOLTS_70)
    return  70;
  if(cell_voltage>LI_ION_CELL_VOLTS_60)
    return  60;
  if(cell_voltage>LI_ION_CELL_VOLTS_50)
    return  50;
  if(cell_voltage>LI_ION_CELL_VOLTS_40)
    return  40;
  if(cell_voltage>LI_ION_CELL_VOLTS_30)
    return  30;
  if(cell_voltage>LI_ION_CELL_VOLTS_20)
    return  20;
  if(cell_voltage>LI_ION_CELL_VOLTS_10)
    return  10;
  if(cell_voltage>LI_ION_CELL_VOLTS_0)
    return  1;
  return 0;
}

void reset_motor_status()
{
  motor_controller_data.ui16_motor_speed_erps = 0;
  motor_controller_data.ui8_error_code = 0;
  motor_controller_data.ui16_wheel_speed_x10 = 0;
  motor_controller_data.last_update = 0;
  motor_controller_data.last_package_id_sent = 0;
  motor_controller_data.last_package_id_confirm = 0;
  motor_controller_data.ui16_adc_battery_voltage = 0;
  motor_controller_data.ui16_battery_voltage_x10 = 0;

}
void reset_motor_control()
{
  //configuration_variables.ui8_assist_level = DEFAULT_VALUE_ASSIST_LEVEL;
  configuration_variables.ui8_number_of_assist_levels = DEFAULT_VALUE_NUMBER_OF_ASSIST_LEVELS;
  configuration_variables.ui16_wheel_perimeter = DEFAULT_VALUE_WHEEL_PERIMETER;
  configuration_variables.ui8_wheel_max_speed = DEFAULT_VALUE_WHEEL_MAX_SPEED;
  configuration_variables.ui8_units_type = DEFAULT_VALUE_UNITS_TYPE;
  configuration_variables.ui32_wh_x10_offset = DEFAULT_VALUE_WH_OFFSET;
  configuration_variables.ui32_wh_x10_100_percent = DEFAULT_VALUE_HW_X10_100_PERCENT;
  configuration_variables.ui8_show_numeric_battery_soc = DEAFULT_VALUE_SHOW_NUMERIC_BATTERY_SOC;
  configuration_variables.ui8_odometer_field_state = DEFAULT_VALUE_ODOMETER_FIELD_STATE;
  configuration_variables.ui8_target_max_battery_power = DEFAULT_VALUE_TARGET_MAX_BATTERY_POWER;
  configuration_variables.ui8_battery_cells_number = DEFAULT_VALUE_BATTERY_CELLS_NUMBER;
  configuration_variables.ui8_battery_max_current = DEFAULT_VALUE_BATTERY_MAX_CURRENT;
  configuration_variables.ui16_battery_low_voltage_cut_off_x10 = DEFAULT_VALUE_BATTERY_LOW_VOLTAGE_CUT_OFF_X10_0;
  configuration_variables.ui16_battery_voltage_reset_wh_counter_x10 = DEFAULT_VALUE_BATTERY_VOLTAGE_RESET_WH_COUNTER_X10;
  configuration_variables.ui16_battery_pack_resistance_x1000 = DEFAULT_VALUE_BATTERY_PACK_RESISTANCE;
  configuration_variables.ui8_motor_voltage_type = DEFAULT_VALUE_CONFIG_0 & 1;
  configuration_variables.ui8_motor_assistance_startup_without_pedal_rotation = (DEFAULT_VALUE_CONFIG_0>>1)&1;
  configuration_variables.ui8_pas_max_cadence = DEFAULT_VALUE_PAS_MAX_CADENCE;
  configuration_variables.ui8_assist_level_power[0] = DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_1;
  configuration_variables.ui8_assist_level_power[1] = DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_2;
  configuration_variables.ui8_assist_level_power[2] = DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_3;
  configuration_variables.ui8_assist_level_power[3] = DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_4;
  configuration_variables.ui8_assist_level_power[4] = DEFAULT_VALUE_ASSIST_LEVEL_FACTOR_5;
  configuration_variables.ui8_startup_motor_power_boost_state = DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_STATE;
  configuration_variables.ui8_startup_motor_power_boost_time = DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_TIME;
  configuration_variables.ui8_startup_motor_power_boost_fade_time =DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_FADE_TIME ;
  configuration_variables.ui8_startup_motor_power_boost[0] = DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_ASSIST_LEVEL_1;
  configuration_variables.ui8_startup_motor_power_boost[1] = DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_ASSIST_LEVEL_2;
  configuration_variables.ui8_startup_motor_power_boost[2] = DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_ASSIST_LEVEL_3;
  configuration_variables.ui8_startup_motor_power_boost[3] = DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_ASSIST_LEVEL_4;
  configuration_variables.ui8_startup_motor_power_boost[4] = DEFAULT_VALUE_STARTUP_MOTOR_POWER_BOOST_ASSIST_LEVEL_5;
  configuration_variables.ui16_adc_motor_temperature_10b = 0;
  configuration_variables.ui8_throttle_adc_measures_motor_temperature = (DEFAULT_VALUE_CONFIG_0>>2)&1;
  configuration_variables.ui8_motor_temperature_min_value_to_limit = DEFAULT_VALUE_MOTOR_TEMPERATURE_MIN_VALUE_LIMIT;
  configuration_variables.ui8_motor_temperature_max_value_to_limit = DEFAULT_VALUE_MOTOR_TEMPERATURE_MAX_VALUE_LIMIT;
  configuration_variables.ui8_temperature_field_config = (DEFAULT_VALUE_CONFIG_0>>3)&3;
  configuration_variables.ui8_lcd_power_off_time_minutes = DEFAULT_VALUE_LCD_POWER_OFF_TIME;
  configuration_variables.ui8_lcd_backlight_on_brightness = DEFAULT_VALUE_LCD_BACKLIGHT_ON_BRIGHTNESS;
  configuration_variables.ui8_lcd_backlight_off_brightness = DEFAULT_VALUE_LCD_BACKLIGHT_OFF_BRIGHTNESS;
  configuration_variables.ui8_offroad_func_enabled = DEFAULT_VALUE_OFFROAD_FUNC_ENABLED;
  configuration_variables.ui8_offroad_enabled_on_startup = DEFAULT_VALUE_OFFROAD_MODE_ENABLED_ON_STARTUP;
  configuration_variables.ui8_offroad_speed_limit = DEFAULT_VALUE_OFFROAD_SPEED_LIMIT;
  configuration_variables.ui8_offroad_power_limit_enabled = DEFAULT_VALUE_OFFROAD_POWER_LIMIT_ENABLED;
  configuration_variables.ui8_offroad_power_limit_div25 = DEFAULT_VALUE_OFFROAD_POWER_LIMIT_DIV25; 
}


void update_setting_incdec(boolean inc_dec, uint8_t selection)
{
  struct_setup_item * item = &(setup_items[selection]);
  long tmpval = (item->is16bit)?( *((uint16_t*)(item->pointer))  ):(  *((uint8_t*)(item->pointer))  );

  if(inc_dec)
    tmpval += item->step;
  else
    tmpval -= item->step;

  tmpval = constrain(tmpval,item->min,item->max);

  if(item->is16bit)
    *((uint16_t*)(item->pointer)) = tmpval;
  else
    *((uint8_t*)(item->pointer)) = tmpval;
}




void prepare_tx_data(uint8_t * tx_buffer)
{

    // set assist level value
    tx_buffer[1] = control_variables.ui8_assist_level<1 ? 0 : configuration_variables.ui8_assist_level_power[control_variables.ui8_assist_level-1];
    // set lights, walk assist, offroad state
    tx_buffer[2] = (control_variables.ui8_lights & 1) | ((control_variables.ui8_walk_assist_level & 1) << 1) | ((control_variables.ui8_offroad_mode & 1) << 2);
    // battery max current in amps
    tx_buffer[3] = configuration_variables.ui8_battery_max_current;
    // motor power in 10 watts unit
    tx_buffer[4] = configuration_variables.ui8_target_max_battery_power;


    // now send a variable for each package sent but first verify if the last one was received otherwise, keep repeating
    // keep cycling so all variables are sent
#define VARIABLE_ID_COUNT 9
    if(motor_controller_data.last_package_id_confirm == motor_controller_data.last_package_id_sent)
    {
      motor_controller_data.last_package_id_sent = (motor_controller_data.last_package_id_sent + 1) % VARIABLE_ID_COUNT;
    }
    tx_buffer[5] = motor_controller_data.last_package_id_sent;

    switch (motor_controller_data.last_package_id_sent)
    {
      case 0:
        // battery low voltage cut-off
        tx_buffer[6] = (uint8_t) (configuration_variables.ui16_battery_low_voltage_cut_off_x10 & 0xff);
        tx_buffer[7] = (uint8_t) (configuration_variables.ui16_battery_low_voltage_cut_off_x10 >> 8);
        break;
      case 1:
        // wheel perimeter
        tx_buffer[6] = (uint8_t) (configuration_variables.ui16_wheel_perimeter & 0xff);
        tx_buffer[7] = (uint8_t) (configuration_variables.ui16_wheel_perimeter >> 8);
        break;
      case 2:
        // wheel max speed
        tx_buffer[6] = configuration_variables.ui8_wheel_max_speed;
        // PAS_MAX_CADENCE_RPM
        tx_buffer[7] = configuration_variables.ui8_pas_max_cadence;
        break;
      case 3:
        // bit 0: cruise control
        // bit 1: motor voltage type: 36V or 48V
        // bit 2: MOTOR_ASSISTANCE_CAN_START_WITHOUT_PEDAL_ROTATION
        tx_buffer[6] = ((control_variables.ui8_cruise_control & 1) |
                           ((configuration_variables.ui8_motor_voltage_type & 1) << 1) |
                            ((configuration_variables.ui8_motor_assistance_startup_without_pedal_rotation & 1) << 2) |
                            ((configuration_variables.ui8_throttle_adc_measures_motor_temperature & 1) << 3));
        tx_buffer[7] = configuration_variables.ui8_startup_motor_power_boost_state;
        break;
      case 4:
        // startup motor power boost
        tx_buffer[6] = configuration_variables.ui8_startup_motor_power_boost [((control_variables.ui8_assist_level) - 1)];
        // startup motor power boost time
        tx_buffer[7] = configuration_variables.ui8_startup_motor_power_boost_time;
        break;
      case 5:
        // startup motor power boost fade time
        tx_buffer[6] = configuration_variables.ui8_startup_motor_power_boost_fade_time;
        tx_buffer[7] = 0; // unused?
        break;
      case 6:
        // motor over temperature min and max values to limit
        tx_buffer[6] = configuration_variables.ui8_motor_temperature_min_value_to_limit;
        tx_buffer[7] = configuration_variables.ui8_motor_temperature_max_value_to_limit;
        break;
      case 7:
        // offroad mode configuration
        tx_buffer[6] = ((configuration_variables.ui8_offroad_func_enabled & 1) |
                              ((configuration_variables.ui8_offroad_enabled_on_startup & 1) << 1)); 
        tx_buffer[7] = configuration_variables.ui8_offroad_speed_limit;
        break;
      case 8:
        // offroad mode power limit configuration
        tx_buffer[6] = configuration_variables.ui8_offroad_power_limit_enabled & 1; 
        tx_buffer[7] = configuration_variables.ui8_offroad_power_limit_div25;
        break;
      default:
        motor_controller_data.last_package_id_sent = 0;
        tx_buffer[6] = 0;
        tx_buffer[7] = 0;
    }
}

void parse_rx_data(uint8_t * rx_buffer)
{
    //Serial_println_s("yay!");

    motor_controller_data.last_package_id_confirm = rx_buffer[1];

    motor_controller_data.ui16_adc_battery_voltage = rx_buffer[2];
    motor_controller_data.ui16_adc_battery_voltage |= ((uint16_t) (rx_buffer[3] & 0x30)) << 4;
    motor_controller_data.ui16_battery_voltage_x10 = (uint16_t)(((uint32_t)motor_controller_data.ui16_adc_battery_voltage * ADC_BATTERY_VOLTAGE_PER_ADC_STEP_X10000) / 1000);
    
    motor_controller_data.ui16_motor_speed_erps = (((uint16_t) rx_buffer[17]) << 8) + ((uint16_t) rx_buffer[16]);
    motor_controller_data.ui8_error_code = rx_buffer[8];
    motor_controller_data.ui16_wheel_speed_x10 = (((uint16_t) rx_buffer[6]) << 8) + ((uint16_t) rx_buffer[5]);
    motor_controller_data.last_update = loop_time_now;
    

}

