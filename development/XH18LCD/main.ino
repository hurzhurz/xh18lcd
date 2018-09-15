void setup()
{
  power_setup(); // soft power latch

  serial_setup();
  eeprom_setup();
  
  button_setup();

  


  serial_setup();
  
  display_setup();

  data_setup();

}

void loop()
{
  loop_time_now = millis();

  button_loop();
  
  
  power_loop();
  // do some nonsense
  //UTFT_clrScr();

  data_loop();

  if(motor_control.enable_6kph && (current_screen!=SCREEN_MAIN || button_states[BUTTON_DOWN].pressState!=PRESSSTATE_SINGLE || !button_states[BUTTON_DOWN].isPressed || button_states[BUTTON_DOWN].pressType!=PRESSTYPE_LONG ) )
    motor_control.enable_6kph=false;

  if(current_screen == SCREEN_MAIN)
  {
    struct button_state *btn;
    
    btn = &(button_states[BUTTON_UP]);
    if(!btn->isHandled && btn->pressState == PRESSSTATE_SINGLE && btn->pressType == PRESSTYPE_SHORT)
    {
      btn->isHandled=true;
      motor_control.assist_level++;
      if(motor_control.assist_level>5)
        motor_control.assist_level = 5;
    }
    
    btn = &(button_states[BUTTON_DOWN]);
    if(!btn->isHandled && btn->pressState == PRESSSTATE_SINGLE && btn->pressType == PRESSTYPE_SHORT)
    {
      btn->isHandled=true;
      motor_control.assist_level--;
      if(motor_control.assist_level>5) // check underflow
        motor_control.assist_level=0;
    }
    if(!btn->isHandled && btn->pressState == PRESSSTATE_SINGLE && btn->pressType == PRESSTYPE_LONG)
    {
      btn->isHandled=true;
      motor_control.enable_6kph=true;
    }

    btn = &(button_states[BUTTON_POWER]);
    if(!btn->isHandled && btn->pressState == PRESSSTATE_SINGLE && btn->pressType == PRESSTYPE_SHORT)
    {
      btn->isHandled=true;
      motor_control.headlight=!motor_control.headlight;
    }

    btn = &(button_states[BUTTON_POWER]);
    if(!btn->isHandled && btn->pressState == PRESSSTATE_DOUBLE && btn->pressType == PRESSTYPE_SHORT)
    {
      static uint8_t lcdbright = LCD_BACKLIGHT_LOW;
      btn->isHandled=true;
      lcdbright++;
      if(lcdbright>LCD_BACKLIGHT_HIGH)
        lcdbright=LCD_BACKLIGHT_OFF;
      display_set_backlight(lcdbright);
    }
    
    btn = &(button_states[BUTTON_INFO]);
    if(!btn->isHandled && btn->pressState == PRESSSTATE_SINGLE && btn->pressType == PRESSTYPE_LONG)
    {
      btn->isHandled=true;
      display_setup_screen();
    }
    
  }
  else if(current_screen == SCREEN_SETUP)
  {
    struct button_state *btn_i=&(button_states[BUTTON_INFO]);
    struct button_state *btn_p=&(button_states[BUTTON_POWER]);
    struct button_state *btn_u=&(button_states[BUTTON_UP]);
    struct button_state *btn_d=&(button_states[BUTTON_DOWN]);
    if(!btn_i->isHandled && btn_i->pressState == PRESSSTATE_SINGLE && btn_i->pressType == PRESSTYPE_LONG)
    {
      btn_i->isHandled=true;
      eeprom_save();
      display_main_screen();
    }
    else if(!btn_p->isHandled && btn_p->pressState == PRESSSTATE_SINGLE && btn_p->pressType == PRESSTYPE_SHORT)
    {
      btn_p->isHandled=true;
      setup_screen_selection--;
      if(setup_screen_selection>7)//underflow
        setup_screen_selection=7;
    }
    else if(!btn_i->isHandled && btn_i->pressState == PRESSSTATE_SINGLE && btn_i->pressType == PRESSTYPE_SHORT)
    {
      btn_i->isHandled=true;
      setup_screen_selection++;
      if(setup_screen_selection>7)//overflow
        setup_screen_selection=0;
    }
    else if(!btn_u->isHandled && btn_u->pressState == PRESSSTATE_SINGLE && btn_u->pressType == PRESSTYPE_SHORT)
    {
      btn_u->isHandled=true;
      update_setting_incdec(true,setup_screen_selection);
      display_setup_print_value(setup_screen_selection);
    }
    else if(!btn_d->isHandled && btn_d->pressState == PRESSSTATE_SINGLE && btn_d->pressType == PRESSTYPE_SHORT)
    {
      btn_d->isHandled=true;
      update_setting_incdec(false,setup_screen_selection);
      display_setup_print_value(setup_screen_selection);
    }
  }

  display_loop();
  
  serial_loop();
}
