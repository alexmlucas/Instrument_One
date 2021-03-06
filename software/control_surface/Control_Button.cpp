#include "Control_Button.h"

Control_Button::Control_Button(int debounce_milliseconds):Simple_Button(debounce_milliseconds){
}

Control_Button::Control_Button(int pin, int debounce_milliseconds, Menu_Controller *menu_controller, Parameter_Container *parameter_container, Parameter *parameter_struct, int parameter_value):Simple_Button(pin, debounce_milliseconds){
  m_menu_controller = menu_controller;
  m_parameter_container = parameter_container;
  m_parameter_struct = parameter_struct;
  m_parameter_value = parameter_value;
  m_redraw_display = false;
  m_update_leds = false;
}

void Control_Button::check_button_pressed(){
  boolean button_state;

  if(digitalRead(m_pin) == HIGH){
    button_state = HIGH;
  } else{
    button_state = LOW;
  }

  if(button_state != m_current_state){                          // Has the button changed state?
    if((millis() - m_last_event_time) > m_debounce_ms){         // If the debounce time has been exceeded...
      m_current_state = button_state;                           // ... change state.
      m_last_event_time = millis();                             // Reset the timer ready for the next iteration.
      if(button_state == HIGH){                                 // If the button is HIGH...
        
        if(callback_function != NULL){                                                                // ... If there's a callback function...   
          callback_function(m_led, m_parameter_container, m_parameter_struct, m_parameter_value);     // ... call it!
        } else {
          m_parameter_container->set_and_send_parameter(m_parameter_struct, m_parameter_value);       // ... otherwise, set the parameter value, this will result in the serial char being transmitted.
        }

        if(m_redraw_display == true){
          m_menu_controller->set_redraw_display_flag(true);
        }
      }
    }
  }
}

void Control_Button::set_callback_func(void (*f)(Single_Led *, Parameter_Container *parameter_container, Parameter *parameter_struct, int parameter_value)){
  callback_function = f;
}

void Control_Button::set_led(Single_Led *led){
  m_led = led;
}
