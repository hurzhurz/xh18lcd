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


void eeprom_defaults()
{
  eeprom_data.max_speed=MAXSPEED_DEFAULT;
  eeprom_data.wheel_size=WHEELSIZE_DEFAULT;
  eeprom_data.odometer=0;
}


uint8_t eeprom_calc_chksum()
{
  uint8_t chk=0xAA;
  for(uint8_t i=0;i<sizeof(eeprom_data);i++)
    chk = F_chk_8( ((uint8_t *)eeprom_data)[i] , chk);
  return chk;
}

boolean eeprom_read()
{
  for(uint8_t i=0;i<sizeof(eeprom_data);i++)
  {
    ((uint8_t *)eeprom_data)[i] = _MEM_(EEPROM_START_ADDR+i); // EEPROM[i];
  }
  return (eeprom_calc_chksum() == _MEM_(EEPROM_START_ADDR+sizeof(eeprom_data)));// EEPROM[i]);
}


void eeprom_write()
{
    // unlock EEPROM 
    FLASH_DUKR = FLASH_DUKR_KEY1;
    FLASH_DUKR = FLASH_DUKR_KEY2;
    while (!(FLASH_IAPSR & (1 << FLASH_IAPSR_DUL)));
    // write data from struct 
    for(uint8_t i=0;i<sizeof(eeprom_data);i++) {
        _MEM_(EEPROM_START_ADDR+i) = ((uint8_t *)eeprom_data)[i];
        while (!(FLASH_IAPSR & (1 << FLASH_IAPSR_EOP)));
    }
    // write checksum 
    _MEM_(EEPROM_START_ADDR+sizeof(eeprom_data)) = eeprom_calc_chksum();
    while (!(FLASH_IAPSR & (1 << FLASH_IAPSR_EOP)));
    // lock EEPROM 
    FLASH_IAPSR &= ~(1 << FLASH_IAPSR_DUL);
}


void eeprom_save()
{
  eeprom_data.wheel_size = motor_control.wheel_size;
  eeprom_data.max_speed = motor_control.max_speed;
  eeprom_data.odometer=odometer_km;
  eeprom_write();
}

void eeprom_setup()
{
  if(!eeprom_read())
  {
    eeprom_defaults();
    eeprom_write();
  }
}


