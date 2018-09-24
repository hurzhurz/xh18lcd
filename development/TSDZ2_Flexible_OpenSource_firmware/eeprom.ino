#define _MEM_(mem_addr)         (*(volatile uint8_t *)(mem_addr))
#define _SFR_(mem_addr)         (*(volatile uint8_t *)(0x5000 + (mem_addr)))
#define EEPROM_START_ADDR       0x4000
#define EEPROM_END_ADDR         0x407F
#define FLASH_IAPSR             _SFR_(0x5F)
#undef FLASH_IAPSR_DUL
#undef FLASH_IAPSR_EOP
#define FLASH_IAPSR_DUL 3
#define FLASH_IAPSR_EOP 2
#define FLASH_DUKR              _SFR_(0x64)
#define FLASH_PUKR_KEY1         0x56
#define FLASH_PUKR_KEY2         0xAE
#define FLASH_DUKR_KEY1         FLASH_PUKR_KEY2
#define FLASH_DUKR_KEY2         FLASH_PUKR_KEY1
//    FLASH_DUKR = FLASH_DUKR_KEY1;
//    FLASH_DUKR = FLASH_DUKR_KEY2;

// https://lujji.github.io/blog/bare-metal-programming-stm8-part2/#EEPROM
// https://github.com/lujji/stm8-bootloader/blob/master/stm8s.h


uint8_t eeprom_calc_chksum()
{
  uint8_t chk=0xAA;
  for(uint8_t i=0;i<sizeof(configuration_variables);i++)
    chk = F_chk_8( ((uint8_t *)configuration_variables)[i] , chk);
  return chk;
}

#define EEPROM_OFFSET_ODOMETER 0x00
#define EEPROM_OFFSET_CONFIGURATION 0x10

boolean eeprom_read(uint8_t * data, uint16_t size, uint16_t offset, bool check_crc16)
{
    uint16_t crc = 0;
    for(uint16_t i=0;i<size;i++)
    {
      data[i] = _MEM_(EEPROM_START_ADDR+offset+i);
    }
    if(!check_crc16)
      return 1;
    crc |= (uint16_t)_MEM_(EEPROM_START_ADDR+offset+size);
    crc |= (uint16_t)_MEM_(EEPROM_START_ADDR+offset+size+1) << 8;
    return (crc == calc_crc16(data, size));
}


boolean eeprom_write(uint8_t * data, uint16_t size, uint16_t offset, bool write_crc16)
{
    uint16_t crc;
    // unlock EEPROM 
    FLASH_DUKR = FLASH_DUKR_KEY1;
    FLASH_DUKR = FLASH_DUKR_KEY2;
    while (!(FLASH_IAPSR & (1 << FLASH_IAPSR_DUL)));
    // write data from struct 
    for(uint16_t i=0;i<size;i++)
    {
        _MEM_(EEPROM_START_ADDR+offset+i) = data[i];
        while (!(FLASH_IAPSR & (1 << FLASH_IAPSR_EOP)));
    }
    if(write_crc16)
    {
      // write checksum
      crc = calc_crc16(data, size);
      _MEM_(EEPROM_START_ADDR+offset+size) = (uint8_t) (crc & 0xff);
      while (!(FLASH_IAPSR & (1 << FLASH_IAPSR_EOP)));
      _MEM_(EEPROM_START_ADDR+offset+size+1) = (uint8_t) (crc >> 8) & 0xff;
      while (!(FLASH_IAPSR & (1 << FLASH_IAPSR_EOP)));
    }
    // lock EEPROM 
    FLASH_IAPSR &= ~(1 << FLASH_IAPSR_DUL);
}

void eeprom_save_odometer()
{
  eeprom_write(((uint8_t *)&odometer_km), sizeof(odometer_km), EEPROM_OFFSET_ODOMETER, 1);
  //Serial_println_s("oo!");
}
void eeprom_load_odometer()
{
  if(!eeprom_read(((uint8_t *)&odometer_km), sizeof(odometer_km), EEPROM_OFFSET_ODOMETER, 1))
  {
    odometer_km=0;
    eeprom_save_odometer();
  }
}
void eeprom_save_configuration()
{
  eeprom_write(((uint8_t *)configuration_variables), sizeof(configuration_variables), EEPROM_OFFSET_CONFIGURATION, 1);
  //Serial_println_s("cc!");
}

void eeprom_load_configuration()
{
  if(!eeprom_read(((uint8_t *)configuration_variables), sizeof(configuration_variables), EEPROM_OFFSET_CONFIGURATION, 1))
  {
    reset_motor_control();
    eeprom_save_configuration();
  }
}

void eeprom_setup()
{
  eeprom_load_configuration();
  eeprom_load_odometer();
}

