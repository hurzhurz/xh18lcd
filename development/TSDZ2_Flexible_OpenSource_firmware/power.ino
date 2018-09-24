void(* resetFunc) (void) = 0;//declare reset function at address 0

void power_setup()
{
  // XH18-LCD soft power latching, not related to LCD
  pinMode(POWER_LATCH, OUTPUT);
  digitalWrite(POWER_LATCH, HIGH);
}

void power_loop()
{
  if(button_states[BUTTON_POWER].pressState != PRESSSTATE_NONE && button_states[BUTTON_POWER].pressDuration > 3000)
  {
    button_states[BUTTON_POWER].isHandled=true;
    display_goodbye_screen();
  }
}

void power_shutdown()
{
    digitalWrite(POWER_LATCH, LOW);
    delay(1000);
    UTFT_setFont(SmallFont);
    UTFT_print("Still there, reset...", 2, 110 );
    delay(1000);
    resetFunc();
}

