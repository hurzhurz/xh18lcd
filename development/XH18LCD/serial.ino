

uint8_t F_chk_8( uint8_t bval, uint8_t cval ) {
  return ( bval + cval ) % 256;
}

uint8_t calc_chk(uint8_t *buffer, uint8_t size)
{
  uint8_t chk = 0;
  for (uint8_t i = 0; i < size; i++)
  {
    chk = F_chk_8(buffer[i], chk);
  }
  return chk;
}

void serial_setup()
{
  Serial_begin(9600);
}


void serial_loop()
{
  if ((loop_time_now - motor_control.last_sent) > SERIAL_SEND_INTERVAL)
  {
    uint8_t tx_buffer[7];
    tx_buffer[0] = 0x59; // start byte
    prepare_tx_data(tx_buffer);
    tx_buffer[6] = calc_chk(tx_buffer, 6);
    Print_print_sn(HardwareSerial_write, tx_buffer, 7);
    motor_control.last_sent = loop_time_now;
  }
  uint8_t count = Serial_available();
  if (count > 0)
  {
    static uint8_t rx_buffer[9];
    static uint8_t received = 0;
    while ( count-- )
    {
      uint8_t data = Serial_read();
      if (received == 0 && data != 0x43)
        continue;
      rx_buffer[received++] = data;
      if (received < 9)
        continue;
      if (rx_buffer[8] != calc_chk(rx_buffer, 8))
      {
        received = 0;
        continue;
      }
      received = 0;
      parse_rx_data(rx_buffer);
    }
  }

  if (motor_status.last_update != 0 && (loop_time_now - motor_status.last_update) > SERIAL_RECEIVE_TIMEOUT)
  {
    reset_motor_status();
  }

}

