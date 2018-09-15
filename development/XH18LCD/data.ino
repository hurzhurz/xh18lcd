void data_setup()
{
  reset_motor_control();
  reset_motor_status();
  odometer_km = eeprom_data.odometer;
}

void data_loop()
{
  static unsigned long last_update = 0;

  if( motor_status.last_update!=last_update )
  {
    if(last_update!=0 && motor_status.last_update!=0)
    {
      unsigned long t_delta_ms = motor_status.last_update - last_update;
      double speed_kph = getSpeed_kph();
      odometer_trip_tmp += speed_kph*t_delta_ms / 3600.0;
    }
    last_update = motor_status.last_update;
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
    eeprom_save();
  }
  if(odometer_km!=eeprom_data.odometer)
  {
    eeprom_save();
  }
}

void reset_motor_status()
{
  motor_status.battery = 0;
  motor_status.low_voltage = false;
  motor_status.motor_running = false;
  motor_status.pedals_spinning = false;
  motor_status.pedal_torque = 0;
  motor_status.error_code = 0;
  motor_status.rotations_per_second = 0.0;
  motor_status.last_update = 0;
}
void reset_motor_control()
{
  motor_control.headlight = false;
  motor_control.enable_6kph = false;
  motor_control.assist_level = 1;
  motor_control.wheel_size = WHEELSIZE_CONSTRAIN(eeprom_data.wheel_size); //inch
  motor_control.max_speed = MAXSPEED_CONSTRAIN(eeprom_data.max_speed); //kph
  motor_control.last_sent = 0;
}

double getSpeed_kph()
{
  double speed;
  if (motor_status.rotations_per_second <= 0.01)
    return 0.0;
  speed = ((double)motor_control.wheel_size * 2.54 * 3.14159 * motor_status.rotations_per_second * 3600.0 / 100000.0);
  if (speed > 99.0)
    speed = 99.0;
  return speed;
}
uint8_t getBattery_percent()
{
  uint8_t bat = motor_status.battery;
  // 0A = 100%;
  return (uint8_t)(((uint16_t)motor_status.battery * 100) / 0x0A);
}

void update_setting_incdec(boolean inc_dec, uint8_t selection)
{
  switch(selection)
  {
    case SETUP_SELECTION_SPEED: // 0
      motor_control.max_speed += inc_dec ? 1 : -1;
      motor_control.max_speed = MAXSPEED_CONSTRAIN(motor_control.max_speed);
      break;
    case SETUP_SELECTION_WHEEL: // 1
      motor_control.wheel_size += inc_dec ? 1 : -1;
      motor_control.wheel_size = WHEELSIZE_CONSTRAIN(motor_control.wheel_size);
      break;
  }
}

void prepare_tx_data(uint8_t * tx_buffer)
{
    tx_buffer[1] = 0; // control flags
    if (motor_control.headlight)   tx_buffer[1] |= (1 << 0);
    if (motor_control.enable_6kph) tx_buffer[1] |= (1 << 5);
    switch (motor_control.assist_level)
    {
      case 1:
        tx_buffer[1] |= (1 << 7);
        break;
      case 2:
        tx_buffer[1] |= (1 << 6);
        break;
      case 3:
        tx_buffer[1] |= (1 << 1);
        break;
      case 4:
        tx_buffer[1] |= (1 << 2);
        break;
      case 5:
        tx_buffer[1] |= (1 << 3);
        break;
      default:
        tx_buffer[1] |= (1 << 4); // off
    }

    tx_buffer[2] = 0; // unused
    tx_buffer[3] = motor_control.wheel_size;
    tx_buffer[4] = 0; // unused
    tx_buffer[5] = motor_control.max_speed;
}


void parse_rx_data(uint8_t * rx_buffer)
{
    uint16_t rotation_duration;

    motor_status.battery = rx_buffer[1];
    motor_status.low_voltage = (boolean) (rx_buffer[2] & (1 << 0));
    motor_status.motor_running = (boolean) (rx_buffer[2] & (1 << 2));
    motor_status.pedals_spinning = (boolean) (rx_buffer[2] & (1 << 3));
    motor_status.pedal_torque = (rx_buffer[4] <= rx_buffer[3]) ? 0 : (rx_buffer[4] - rx_buffer[3]) ;
    motor_status.error_code = rx_buffer[5];
    rotation_duration = (((((uint16_t)rx_buffer[7]) << 8) & 0xFF00) | ((uint16_t)rx_buffer[6]) & 0xFF);
    if (rotation_duration > 1750 || rotation_duration < 1)
      motor_status.rotations_per_second = 0;
    else
      motor_status.rotations_per_second = 1000.0 / ((double)rotation_duration * 2.04);
    motor_status.last_update = loop_time_now;
}

