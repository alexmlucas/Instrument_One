#include "Selection_Page.h"

Selection_Page::Selection_Page(Menu_Controller *menu_controller, Parameter_Container *parameter_container):Menu_Page(menu_controller, parameter_container){
  m_menu_controller = menu_controller;                                          // Assign class member pointers to incoming memory addresses.
  m_parameter_container = parameter_container;                                  // *** The parameter container is redundant here as we're pointing to the parameter value directly. ***
}

void Selection_Page::draw(Adafruit_SSD1306 &display){
  char string_buffer[16];                                                       // Buffer for reading strings from program memory.
  int selected_item = m_parameter_struct->value;                                // The selected item equals the value at the address pointed to by m_target_parameter.          
  
  display.clearDisplay();                                                       // Clear the display.
  display.setTextColor(WHITE);                                                  // Set text to white.
                                                          
                                                                                // Draw the page title.
  display.setCursor((PADDING),PADDING);                                         // Set the cursor position.
  strcpy_P(string_buffer, (char *)pgm_read_word(&(m_menu_text[0])));            // Copy title string into flash memory; always index 0.
  
  display.println(string_buffer);                                               // Write text to display.
  display.drawLine(0, 14, 128, 14, WHITE);                                      // Underline the title.
 
  for(int i = 1; i < m_number_of_menu_items; i++){                              // Iterate through the remaining strings in the array.    

    if(m_menu_controller->get_cursor_position() == i-1){                        // Print the cursor if in text location
      display.setCursor((PADDING),(LINE_HEIGHT * i) + (PADDING));               
      display.println('>');                                                     
    }                                                                           

    if(i == selected_item + 1){                                                 // If item is selected, set the text to black and draw a selection rectangle.
      display.setTextColor(BLACK);
      display.fillRect((PADDING + WHITE_SPACE - 2), (LINE_HEIGHT * i) + (PADDING - 2), 90, 12, WHITE);
    } 
    
    display.setCursor((PADDING + WHITE_SPACE),(LINE_HEIGHT * i) + (PADDING));   // Print the text.
    strcpy_P(string_buffer, (char *)pgm_read_word(&(m_menu_text[i])));
    display.println(string_buffer);
    display.setTextColor(WHITE);                                                // Set text to white, ready for next loop.
  }
  display.display();                                                            // Display the new image.                 
}

void Selection_Page::on_enter(){
  int cursor_position = m_menu_controller->get_cursor_position();                         // Get the current cursor position.                                 
  m_parameter_container->set_and_send_parameter(m_parameter_struct, cursor_position);     // Set the selected parameter to that of the cursor position.
  m_menu_controller->set_redraw_display_flag(true);                                       // Update the currently selected menu.
}
