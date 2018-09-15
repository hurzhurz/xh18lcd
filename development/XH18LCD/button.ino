void button_setup()
{
  pinMode(BUTTON_K1_ONOFF, INPUT); // external pullup
  pinMode(BUTTON_K2, INPUT); // external pullup
  
  for(uint8_t i=0; i<BUTTON_COUNT; i++)
  {
    button_states[i].isHandled = false;
    button_states[i].isPressed = false;
    button_states[i].disableDouble = false;
    button_states[i].pressState = PRESSSTATE_NONE;
    button_states[i].pressType = PRESSTYPE_SHORT;
    button_states[i].pressStartTime = 0;
    button_states[i].pressDuration = 0;
  }

  // assume power button still pressed
  button_states[BUTTON_POWER].isHandled = true;
  button_states[BUTTON_POWER].isPressed = true;
}

void button_update(uint8_t button, boolean state, unsigned long now)
{
  struct button_state *this = &(button_states[button]);
  // update duration
  if(this->isPressed)
    this->pressDuration = now - this->pressStartTime;
  // if already handeled, do mostly nothin g
  if(this->isHandled)
  {
    if(!state)
    { // reset to defaults if no more pressed
      this->isHandled = false;
      this->isPressed = false;
      this->pressState = PRESSSTATE_NONE;
    }
    return;
  }
  if(this->pressState == PRESSSTATE_NONE)
  {
    if(state && !this->isPressed) // start of new press
    {
      this->isPressed = true;
      this->pressStartTime = now;
      this->pressDuration = 0;
    }
    else if(!state && this->isPressed) // no more pressed
    {
      this->isPressed = false;
      if(this->pressDuration >= PRESSTIME_LONG)
      {
        this->pressState = PRESSSTATE_SINGLE;
        this->pressType = PRESSTYPE_LONG;
      }
      else if(this->pressDuration >= PRESSTIME_DOUBLE || this->disableDouble)
      {
        this->pressState = PRESSSTATE_SINGLE;
        this->pressType = PRESSTYPE_SHORT;
      }
      else
      {
        this->pressState = PRESSSTATE_SINGLE_WAIT;
        this->pressType = PRESSTYPE_SHORT;
      }
    }
    else if(state && this->isPressed) // still pressed
    {
      if(this->pressDuration >= PRESSTIME_LONG)
      {
        this->pressState = PRESSSTATE_SINGLE;
        this->pressType = PRESSTYPE_LONG;
      }
    }
  }
  else if(this->pressState == PRESSSTATE_SINGLE_WAIT)
  {
    if(!this->isPressed && (now - this->pressStartTime) > PRESSTIME_DOUBLE)
    {
      this->pressState = PRESSSTATE_SINGLE;
    }
    else
    {
      if(state && !this->isPressed) // start of new press
      {
        this->isPressed = true;
        this->pressStartTime = now;
        this->pressDuration = 0;
      }
      else if(!state && this->isPressed) // no more pressed
      { 
        this->isPressed = false;
        this->pressState = PRESSSTATE_DOUBLE;
        if(this->pressDuration >= PRESSTIME_LONG)
        {
          this->pressType = PRESSTYPE_LONG;
        }
        else
        {
          this->pressType = PRESSTYPE_SHORT;
        }
      }
      else if(state && this->isPressed) // still pressed
      {
        if(this->pressDuration >= PRESSTIME_LONG)
        {
          this->pressState = PRESSSTATE_DOUBLE;
          this->pressType = PRESSTYPE_LONG;
        }
      }
    }
  }
  else
  {
    if(!state && this->isPressed)
    {
      this->isPressed = false;
    }
    else if(!state && !this->isPressed)
    {
      this->pressState = PRESSSTATE_NONE;
      this->isPressed = false;
      this->isHandled = false;
    }
  }
}

void button_loop()
{
  uint8_t grip = getGripState();
  button_update(BUTTON_POWER, isButtonPressed(BUTTON_K1_ONOFF), loop_time_now);
  button_update(BUTTON_INFO, isButtonPressed(BUTTON_K2), loop_time_now);
  button_update(BUTTON_UP, (grip==GRIP_UP), loop_time_now);
  button_update(BUTTON_DOWN, (grip==GRIP_DOWN), loop_time_now);
}

uint8_t getGripState()
{
  int analog = analogRead(GRIP_HALL_SENSOR);
  if(analog<200)
    return GRIP_DOWN;
  if(analog>840)
    return GRIP_UP;
  return GRIP_MID;
}


