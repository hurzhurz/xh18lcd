

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


uint16_t crc16(uint8_t data, uint16_t * crc)
{
    *crc = *crc ^ (uint16_t)data;
    for (uint8_t i = 0; i<8; i++)
    {
        if (*crc & 0x0001)
            *crc = (*crc >> 1) ^ 0xA001;
        else
            *crc >>= 1;
    }
}
uint16_t calc_crc16(uint8_t *buffer, uint8_t size)
{
  uint16_t crc = 0xffff;
  for (uint8_t i = 0; i<size; i++)
    crc16(buffer[i], &crc);
  return crc;
}

void serial_setup()
{
  Serial_begin(9600);
}

void serial_loop()
{
  uint8_t count = Serial_available();
  if (count > 0)
  {
    static uint8_t rx_buffer[22];
    uint8_t tx_buffer[10];
    uint16_t crc;
    static uint8_t received = 0;
    while ( count-- )
    {
      uint8_t data = Serial_read();
      if (received == 0 && data != 0x43)
        continue;
      rx_buffer[received++] = data;
      if (received < 22)
        continue;
      crc = ((uint16_t)rx_buffer[21] << 8 | (uint16_t)rx_buffer[20]);
      if (  crc != calc_crc16(rx_buffer, 20) )
      {
        received = 0;
        continue;
      }
      received = 0;
      
      parse_rx_data(rx_buffer);

      tx_buffer[0]=0x59;
      prepare_tx_data(tx_buffer);
      crc = calc_crc16(tx_buffer, 8);
      tx_buffer[8] = (uint8_t) (crc & 0xff);
      tx_buffer[9] = (uint8_t) (crc >> 8) & 0xff;
      HardwareSerial_flush();
      Print_print_sn(HardwareSerial_write, tx_buffer, 10);
    }
  }



}

