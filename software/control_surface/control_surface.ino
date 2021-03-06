// Display includes
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Control includes
#include "Control_Button.h"
#include "Shift_Register_Control_Button.h"
#include "Shift_Register_Menu_Button.h"
#include "Simple_Encoder.h"
#include "Single_Led.h"
#include "Rg_Led.h"
#include "Rgb_Led.h"

// Menu includes
#include "Menu_Page.h"
#include "Main_Page.h"
#include "List_Page.h"
#include "Selection_Page.h"
#include "Value_Page.h"
#include "Splash_Page.h"
#include "Menu_Controller.h"
#include "Parameter_Container.h"

#define OLED_RESET 15

// Create an instance of the display
Adafruit_SSD1306 display(OLED_RESET);

// Define the debounce time in milliseconds
#define DEBOUNCE_TIME 50

// Define the USB MIDI Note Velocity
#define NOTE_VELOCITY 100

// Define the encoder pins
#define ENCODER_PIN_A 5
#define ENCODER_PIN_B 6

// Define the button/switch pins
#define ACCESS_SWT_PIN 11
#define PLAY_BTN_PIN 20
#define STOP_BTN_PIN 14
#define BACK_BTN_PIN 13
#define ENTER_BTN_PIN 12

// Define button bits
#define RECONNECT_BTN_BIT 0
#define POWER_BTN_BIT 1
#define SONG_1_BTN_BIT 2
#define SONG_2_BTN_BIT 3
#define SONG_3_BTN_BIT 4
#define SONG_4_BTN_BIT 5

// Define the shift register pins.
#define SHIFT_REG_LATCH 8
#define SHIFT_REG_DATA 9
#define SHIFT_REG_CLOCK 7

// Define LED pins
#define ZN_LED_R A9
#define ZN_LED_G A8
#define ZN_LED_B A7
#define WB_LED_R A3
#define WB_LED_G A2
#define PL_LED_G 10

// Define play button states
#define PLAYING 0
#define PAUSED 1
#define STOPPED 2

// Define the min & max cursor values
#define MIN_CURSOR_VALUE 0
#define MAX_CURSOR_VALUE 2

// Define characters to be sent as serial strings.
const char RECONNECT_CHAR PROGMEM = 'a'; 
const char POWER_CHAR PROGMEM = 'b';
const char ACCESS_SWT_CHAR PROGMEM = 'c';
const char PLAY_CHAR PROGMEM = 'd';
const char STOP_CHAR PROGMEM = 'e';
const char SONG_1_CHAR PROGMEM = 'f';
const char SONG_2_CHAR PROGMEM = 'g';
const char SONG_3_CHAR PROGMEM = 'h';
const char SONG_4_CHAR PROGMEM = 'i';
const char BACK_CHAR PROGMEM = 'j';
const char ENTER_CHAR PROGMEM = 'k';

const static char song[] PROGMEM = "Song";
const static char guitar[] PROGMEM = "Guitar";
const char backing[] PROGMEM = "Backing";
const char master[] PROGMEM = "Master";
const static char zone[] PROGMEM = "Zone";
const static char mix_levels[] PROGMEM = "Mix Levels";
const char red_zone[] PROGMEM = "Red Zone";
const char green_zone[] PROGMEM = "Green Zone";
const char blue_zone[] PROGMEM = "Blue Zone";

const static char classic_rock[] PROGMEM = "Classic Rock";
const static char hard_rock[] PROGMEM = "Hard Rock";
const static char acoustic[] PROGMEM = "Acoustic";
const char scale[] PROGMEM = "Scale";
const char red_scale[] PROGMEM = "Red Scale";
const char green_scale[] PROGMEM = "Green Scale";
const char blue_scale[] PROGMEM = "Blue Scale";
const char root[] PROGMEM = "Root";
const char red_root[] PROGMEM = "Red Root";
const char green_root[] PROGMEM = "Green Root";
const char blue_root[] PROGMEM = "Blue Root";
const static char connecting[] PROGMEM = "Connecting";
const static char to_wristband[] PROGMEM = "to wristband,";
const static char please_wait[] PROGMEM = "please wait...";
const char sorry[] PROGMEM = "Sorry, I couldn't";
const char not_connect[] PROGMEM = "connect to";
const char the_wristband[] PROGMEM = "the wristband.";
const char not_load_song[] PROGMEM = "load song. Connect";
const char check_memory[] PROGMEM = "USB stick & reboot.";


//const char scale_param[] PROGMEM = "Major, Minor, Blues, Pent. Major, Pent. Minor, Major Chord, Minor Chord";
//const char root_param[] PROGMEM = "A, A#, B, C, C#, D, D#, E, F, F#, G, G#";

const char test_array[] PROGMEM = "test";
//const char flash_test PROGMEM = 'f';

// An array of constant pointers to constant chars?
const static char *const main_menu_txt[] PROGMEM = {song, guitar, zone, mix_levels};
const char *const guitar_menu_txt[] PROGMEM = {guitar, classic_rock, hard_rock, acoustic};
const char *const zone_menu_txt[] PROGMEM = {zone, red_zone, green_zone, blue_zone};
const char *const red_zone_menu_txt[] PROGMEM = {red_zone, scale, root};
const char *const green_zone_menu_txt[] PROGMEM = {green_zone, scale, root};
const char *const blue_zone_menu_txt[] PROGMEM = {blue_zone, scale, root};
const char *const red_scale_menu_txt[] PROGMEM = {red_scale};
const char *const green_scale_menu_txt[] PROGMEM = {green_scale};
const char *const blue_scale_menu_txt[] PROGMEM = {blue_scale};
const char *const red_root_menu_txt[] PROGMEM = {red_root};
const char *const green_root_menu_txt[] PROGMEM = {green_root};
const char *const blue_root_menu_txt[] PROGMEM = {blue_root};

const char *const mix_levels_menu_txt[] PROGMEM = {mix_levels, guitar, backing, master};
const char *const guitar_level_menu_txt[] PROGMEM = {guitar}; 
const char *const backing_level_menu_txt[] PROGMEM = {backing}; 
const char *const master_level_menu_txt[] PROGMEM = {master};

const static char *const reconnect_menu_txt[] PROGMEM = {connecting, to_wristband, please_wait};
const char *const connection_fail_menu_txt[] PROGMEM = {sorry, not_connect, the_wristband};
const char *const song_fail_menu_txt[] PROGMEM = {sorry, not_load_song, check_memory};

const char* const mix_levels_param_txt[] PROGMEM = {"0", "0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1"};
const char* const scales_param_txt[] PROGMEM = {"Pent. Major", "Pent. Minor", "Blues", "Major Chord", "Minor Chord"};
const char* const root_param_txt[] PROGMEM = {"E", "F", "F#", "G", "G#", "A", "A#", "B", "C", "C#", "D", "D#"};

// Define a variable to hold the data from the shift register
byte shift_reg_byte = 0;

// Create Simple_Encoder instance.
Simple_Encoder selection_encoder;

// A pointer to an array of constant pointers to constant chars.
const char *const *menu_text_pointer;

// Byte for storing incoming serial data.
char incoming_byte_1;
char incoming_byte_2;
char incoming_byte_3;

// String representing the current menu location.
char current_menu_location[] = {""};

// Buffer for reading menu text.
char string_buffer[30];

// Read and monitior encoder position.
int current_encoder_position, last_encoder_position;

Menu_Controller menu_controller(MIN_CURSOR_VALUE, MAX_CURSOR_VALUE);      // Create an instance of Menu_Controller.
Parameter_Container parameter_container;                                  // Create an instance of Parameter_Container.
Parameter_Container *p_parameter_container;

// *** Initialise Menu_Page(s) ***
Main_Page main_menu(&menu_controller, &parameter_container);
Selection_Page guitar_menu(&menu_controller, &parameter_container);
List_Page zone_menu(&menu_controller, &parameter_container);
List_Page mix_levels_menu(&menu_controller, &parameter_container);
List_Page red_zone_menu(&menu_controller, &parameter_container);
List_Page green_zone_menu(&menu_controller, &parameter_container);
List_Page blue_zone_menu(&menu_controller, &parameter_container);
Value_Page red_scale_menu(&menu_controller, &parameter_container);
Value_Page red_root_menu(&menu_controller, &parameter_container);
Value_Page green_scale_menu(&menu_controller, &parameter_container);
Value_Page green_root_menu( &menu_controller, &parameter_container);
Value_Page blue_scale_menu(&menu_controller, &parameter_container);
Value_Page blue_root_menu(&menu_controller, &parameter_container);
Value_Page guitar_level_menu(&menu_controller, &parameter_container);
Value_Page backing_level_menu(&menu_controller, &parameter_container);
Value_Page master_level_menu(&menu_controller, &parameter_container);
Splash_Page reconnect_menu(&menu_controller, &parameter_container);
Splash_Page connection_fail_menu(&menu_controller, &parameter_container);
Splash_Page song_fail_menu(&menu_controller, &parameter_container);

// *** Initialise Pointers to Sub Menu_Page(s) ***  
Menu_Page *p_main_sub_menus[] = {&guitar_menu, &zone_menu, &mix_levels_menu};                        
Menu_Page *p_zone_sub_menus[] = {&red_zone_menu, &green_zone_menu, &blue_zone_menu};
Menu_Page *p_red_zone_sub_menus[] = {&red_scale_menu, &red_root_menu};
Menu_Page *p_green_zone_sub_menus[] = {&green_scale_menu, &green_root_menu};
Menu_Page *p_blue_zone_sub_menus[] = {&blue_scale_menu, &blue_root_menu};    
Menu_Page *p_mix_levels_sub_menus[] = {&guitar_level_menu, &backing_level_menu, &master_level_menu};

Menu_Page *p_current_menu_page;                                           // Create a pointer to the currently selected menu page.
Menu_Page *p_previous_menu_page;                                          // Create a pointer to the previously selected menu page.
Menu_Page *p_wristband_return_page;                                       // Creat a pointer to the page to retrn to after wristabnd conneciton messages have been displayed.

// *** Create Simple_Button instances ***
Control_Button access_switch(ACCESS_SWT_PIN, DEBOUNCE_TIME, &menu_controller, &parameter_container, &parameter_container.m_zone, 0);
Control_Button play_button(PLAY_BTN_PIN, DEBOUNCE_TIME, &menu_controller, &parameter_container, &parameter_container.m_play, 1);
Control_Button stop_button(STOP_BTN_PIN, DEBOUNCE_TIME, &menu_controller, &parameter_container, &parameter_container.m_play, 0);
Menu_Button back_button(BACK_BTN_PIN, DEBOUNCE_TIME, &menu_controller);
Menu_Button enter_button(ENTER_BTN_PIN, DEBOUNCE_TIME, &menu_controller);

// *** Create Shift_Register_Button instances ***
Shift_Register_Control_Button reconnect_button(RECONNECT_BTN_BIT, DEBOUNCE_TIME, &menu_controller, &parameter_container, &parameter_container.m_reconnect, 0);
Shift_Register_Control_Button power_button(POWER_BTN_BIT, DEBOUNCE_TIME, &menu_controller, &parameter_container, &parameter_container.m_power, 0);
Shift_Register_Control_Button song_1_button(SONG_1_BTN_BIT, DEBOUNCE_TIME, &menu_controller, &parameter_container, &parameter_container.m_song, 0);
Shift_Register_Control_Button song_2_button(SONG_2_BTN_BIT, DEBOUNCE_TIME, &menu_controller, &parameter_container, &parameter_container.m_song, 1);
Shift_Register_Control_Button song_3_button(SONG_3_BTN_BIT, DEBOUNCE_TIME, &menu_controller, &parameter_container, &parameter_container.m_song, 2);
Shift_Register_Control_Button song_4_button(SONG_4_BTN_BIT, DEBOUNCE_TIME, &menu_controller, &parameter_container, &parameter_container.m_song, 3);

// *** Create Led instances ***
int zone_led_pins[] = {ZN_LED_R, ZN_LED_G, ZN_LED_B};
int wristband_led_pins[] = {WB_LED_R, WB_LED_G};

int RG_RED[] = {255, 0};
int RG_GREEN[] = {0, 255};
int RG_OFF[] = {0, 0};

int RGB_RED[] = {255, 0, 0};
int RGB_GREEN[] = {0, 255, 0};
int RGB_BLUE[] = {0, 0, 255};
int RGB_OFF[] = {0, 0, 0};

int *p_rgb_red = RGB_RED; 
int *p_rgb_blue = RGB_BLUE; 
bool FLASH_ALL[] = {true, true, true};

Single_Led play_led;
Rg_Led wristband_leds;
Rgb_Led zone_leds;

// Timer variables for deactivating Splash Pages
int time_splash_loaded_ms;
int SPLASH_DISPLAY_TIME = 3000;
int current_time_ms;
bool splash_page_loaded;

// Variable for tracking whether or not a song is loaded.
bool song_loaded = false;

void setup() {
  // *** Assign menu text to Menu_Page(s) ***
  int text_size;
  text_size = sizeof(main_menu_txt)/sizeof(main_menu_txt[0]);                       // Main Menu
  main_menu.set_menu_text(main_menu_txt, text_size);
  text_size = sizeof(guitar_menu_txt)/sizeof(guitar_menu_txt[0]);                   // Guitar Menu
  guitar_menu.set_menu_text(guitar_menu_txt, text_size);
  text_size = sizeof(zone_menu_txt)/sizeof(zone_menu_txt[0]);                       // Zone Menu
  zone_menu.set_menu_text(zone_menu_txt, text_size);
  text_size = sizeof(mix_levels_menu_txt)/sizeof(mix_levels_menu_txt[0]);           // Mix Levels Menu
  mix_levels_menu.set_menu_text(mix_levels_menu_txt, text_size);
  text_size = sizeof(red_zone_menu_txt)/sizeof(red_zone_menu_txt[0]);               // Red Zone Menu
  red_zone_menu.set_menu_text(red_zone_menu_txt, text_size);
  text_size = sizeof(green_zone_menu_txt)/sizeof(green_zone_menu_txt[0]);           // Green Zone Menu
  green_zone_menu.set_menu_text(green_zone_menu_txt, text_size);
  text_size = sizeof(blue_zone_menu_txt)/sizeof(blue_zone_menu_txt[0]);             // Blue Zone Menu
  blue_zone_menu.set_menu_text(blue_zone_menu_txt, text_size);
  text_size = sizeof(red_scale_menu_txt)/sizeof(red_scale_menu_txt[0]);             // Red Scale Menu
  red_scale_menu.set_menu_text(red_scale_menu_txt, text_size); 
  text_size = sizeof(red_root_menu_txt)/sizeof(red_root_menu_txt[0]);               // Red Root Menu
  red_root_menu.set_menu_text(red_root_menu_txt, text_size);
  text_size = sizeof(green_scale_menu_txt)/sizeof(green_scale_menu_txt[0]);         // Green Scale Menu
  green_scale_menu.set_menu_text(green_scale_menu_txt, text_size);
  text_size = sizeof(green_root_menu_txt)/sizeof(green_root_menu_txt[0]);           // Green Root Menu
  green_root_menu.set_menu_text(green_root_menu_txt, text_size);
  text_size = sizeof(blue_scale_menu_txt)/sizeof(blue_scale_menu_txt[0]);           // Blue Scale Menu
  blue_scale_menu.set_menu_text(blue_scale_menu_txt, text_size);
  text_size = sizeof(blue_root_menu_txt)/sizeof(blue_root_menu_txt[0]);             // Blue Root Menu
  blue_root_menu.set_menu_text(blue_root_menu_txt, text_size);
  text_size = sizeof(guitar_level_menu_txt)/sizeof(guitar_level_menu_txt[0]);       // Guitar Level Menu
  guitar_level_menu.set_menu_text(guitar_level_menu_txt, text_size);
  text_size = sizeof(backing_level_menu_txt)/sizeof(backing_level_menu_txt[0]);     // Guitar Backing Menu
  backing_level_menu.set_menu_text(backing_level_menu_txt, text_size);
  text_size = sizeof(master_level_menu_txt)/sizeof(master_level_menu_txt[0]);       // Master Level Menu
  master_level_menu.set_menu_text(master_level_menu_txt, text_size);
  text_size = sizeof(reconnect_menu_txt)/sizeof(reconnect_menu_txt[0]);             // Reconnect Menu
  reconnect_menu.set_menu_text(reconnect_menu_txt, text_size);
  text_size = sizeof(connection_fail_menu_txt)/sizeof(connection_fail_menu_txt[0]); // Connection Fail Menu
  connection_fail_menu.set_menu_text(connection_fail_menu_txt, text_size);
  text_size = sizeof(song_fail_menu_txt)/sizeof(song_fail_menu_txt[0]); // Connection Fail Menu
  song_fail_menu.set_menu_text(song_fail_menu_txt, text_size);

  // *** Assign parameter text to Menu_Page(s) ***
  text_size = sizeof(scales_param_txt)/sizeof(scales_param_txt[0]);                 // Scales parameter text
  red_scale_menu.set_and_send_parameter_text(scales_param_txt, text_size);
  green_scale_menu.set_and_send_parameter_text(scales_param_txt, text_size);
  blue_scale_menu.set_and_send_parameter_text(scales_param_txt, text_size);
  text_size = sizeof(root_param_txt)/sizeof(root_param_txt[0]);                     // Root parameter text
  red_root_menu.set_and_send_parameter_text(root_param_txt, text_size);
  green_root_menu.set_and_send_parameter_text(root_param_txt, text_size);
  blue_root_menu.set_and_send_parameter_text(root_param_txt, text_size);
  text_size = sizeof(mix_levels_param_txt)/sizeof(mix_levels_param_txt[0]);         // Mix Levels parameter text
  guitar_level_menu.set_and_send_parameter_text(mix_levels_param_txt, text_size);
  backing_level_menu.set_and_send_parameter_text(mix_levels_param_txt, text_size);
  master_level_menu.set_and_send_parameter_text(mix_levels_param_txt, text_size);

  // *** Assign parameter structs to Menu_Page(s) ***
  main_menu.set_and_send_parameter_struct(&parameter_container.m_song);
  guitar_menu.set_and_send_parameter_struct(&parameter_container.m_guitar);
  red_scale_menu.set_and_send_parameter_struct(&parameter_container.m_red_scale);
  red_root_menu.set_and_send_parameter_struct(&parameter_container.m_red_root);
  green_scale_menu.set_and_send_parameter_struct(&parameter_container.m_green_scale);
  green_root_menu.set_and_send_parameter_struct(&parameter_container.m_green_root);
  blue_scale_menu.set_and_send_parameter_struct(&parameter_container.m_blue_scale);
  blue_root_menu.set_and_send_parameter_struct(&parameter_container.m_blue_root);
  guitar_level_menu.set_and_send_parameter_struct(&parameter_container.m_guitar_level);
  backing_level_menu.set_and_send_parameter_struct(&parameter_container.m_backing_level);
  master_level_menu.set_and_send_parameter_struct(&parameter_container.m_master_level);

  menu_controller.set_currently_selected_menu(&main_menu);                // Setting the main_menu as the Menu_Page currently selected
 
  // *** Build the menu system ***
  main_menu.set_sub_menus(p_main_sub_menus);                              // Add sub Menu_Page(s) to Menu_Page(s)
  zone_menu.set_sub_menus(p_zone_sub_menus);
  red_zone_menu.set_sub_menus(p_red_zone_sub_menus);
  green_zone_menu.set_sub_menus(p_green_zone_sub_menus);
  blue_zone_menu.set_sub_menus(p_blue_zone_sub_menus);
  mix_levels_menu.set_sub_menus(p_mix_levels_sub_menus);
  
  guitar_menu.set_previous_menu(&main_menu);                              // Make the Menu_Page(s) aware of their parents. 
  zone_menu.set_previous_menu(&main_menu);                                
  mix_levels_menu.set_previous_menu(&main_menu);
  red_zone_menu.set_previous_menu(&zone_menu);                                                    
  green_zone_menu.set_previous_menu(&zone_menu);  
  blue_zone_menu.set_previous_menu(&zone_menu);  
  guitar_level_menu.set_previous_menu(&mix_levels_menu);
  backing_level_menu.set_previous_menu(&mix_levels_menu);
  master_level_menu.set_previous_menu(&mix_levels_menu);
  red_scale_menu.set_previous_menu(&red_zone_menu);
  red_root_menu.set_previous_menu(&red_zone_menu);
  green_scale_menu.set_previous_menu(&green_zone_menu);
  green_root_menu.set_previous_menu(&green_zone_menu);
  blue_scale_menu.set_previous_menu(&blue_zone_menu);
  blue_root_menu.set_previous_menu(&blue_zone_menu);
  
  // *** Configure the LEDs ***
  play_led.set_pinout(PL_LED_G);
  wristband_leds.set_pinout(wristband_led_pins);
  zone_leds.set_pinout(zone_led_pins);

  // *** Configure the buttons ***
  play_button.set_led(&play_led);
  stop_button.set_led(&play_led);
  song_1_button.set_led(&play_led);
  song_2_button.set_led(&play_led);
  song_3_button.set_led(&play_led);
  song_4_button.set_led(&play_led);
  
  access_switch.set_led(&zone_leds);
  
  enter_button.set_callback_func(enter_pressed);                                                // Set button callback functions
  back_button.set_callback_func(back_pressed);
  play_button.set_callback_func(play_pressed);
  stop_button.set_callback_func(stop_pressed);
  song_1_button.set_callback_func(song_pressed);
  song_2_button.set_callback_func(song_pressed);
  song_3_button.set_callback_func(song_pressed);
  song_4_button.set_callback_func(song_pressed);
  access_switch.set_callback_func(access_switch_pressed);

  song_1_button.m_redraw_display = true;
  song_2_button.m_redraw_display = true;
  song_3_button.m_redraw_display = true;
  song_4_button.m_redraw_display = true;

  // *** Configure the encoder ***
  selection_encoder.initialise(ENCODER_PIN_A, ENCODER_PIN_B, DEBOUNCE_TIME, &menu_controller);

  // *** Configure the shift register ***
  pinMode(SHIFT_REG_LATCH, OUTPUT); 
  pinMode(SHIFT_REG_CLOCK, OUTPUT);
  pinMode(SHIFT_REG_DATA, INPUT);
  
  // *** Configure the display ***
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);   
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.display();

  ////// display splash here....

  display.setTextSize(4);
  display.setCursor(5,11);
  display.println("i1");
  display.setTextSize(1);
  display.setCursor(6,47);
  display.println("Virtual Guitar");
  display.display();   

  Serial1.begin(9600);                          // Begin serial1
  Serial.begin(9600);                           // Serial for debugging.
  delay(500);                                   // Wait for the serial stream to get going.

  bool data_received_flag = false;
  
  while(data_received_flag == false){           // Pause until serial data is available.
    Serial1.println("r00");                     // Send a message to RPi to indicate that it's ready to recieve song data

    if(Serial1.available() > 0){                // If there is serial data...
      serial_parser();                          // ...call the serial parser
      data_received_flag = true;                // Set the data recieved flag to exit the loop
    }
    delay(500);                                 // Delay to avoid spamming the serial stream
  }

  // *** Set LEDs based on serial data.
  switch(parameter_container.m_zone.value){
    case 0:
      zone_leds.set_colour(RGB_RED);
      break;
    case 1:
      zone_leds.set_colour(RGB_GREEN);
      break;
    case 2:
      zone_leds.set_colour(RGB_BLUE);
      break;
  }

  // Wristband has a default state of not connected until told otherwise by RPi.
  wristband_leds.set_colour(RG_RED);
  wristband_leds.set_flashing(true);

  delay(1000);                                  // Pause again before we get going.
}

void loop() {
  
  if(splash_page_loaded){
    current_time_ms = millis();

    if((current_time_ms - time_splash_loaded_ms) > SPLASH_DISPLAY_TIME){
      // This method will also set the redraw display flag.
      menu_controller.set_currently_selected_menu(p_wristband_return_page);
      splash_page_loaded = false;
    }
  }
  
  if(menu_controller.get_redraw_display_flag() == true){
    // Assign pointer to the currently selected menu.
    p_current_menu_page = (Menu_Page*)menu_controller.get_currently_selected_menu();
    
    if(p_current_menu_page != p_previous_menu_page){
      menu_controller.set_cursor_position(0);             // The menu page has changed so reset the cursor position. 
    }
    
    p_current_menu_page->draw(display);
    menu_controller.set_redraw_display_flag(false);

    p_previous_menu_page = p_current_menu_page;           // Update the previous menu page.
  }
  
  // Set the latch pin to 1 to collect parallel data.
  digitalWrite(SHIFT_REG_LATCH, 1);
  delayMicroseconds(1);
  // Set the latch pin to to 0 to transmit data serially.
  digitalWrite(SHIFT_REG_LATCH, 0);
  // Collect the register as a byte.
  shift_reg_byte = shift_in(SHIFT_REG_DATA, SHIFT_REG_CLOCK);

  // Process the shift register buttons
  reconnect_button.check_button_pressed(shift_reg_byte);
  power_button.check_button_pressed(shift_reg_byte);
  song_1_button.check_button_pressed(shift_reg_byte);
  song_2_button.check_button_pressed(shift_reg_byte);
  song_3_button.check_button_pressed(shift_reg_byte);
  song_4_button.check_button_pressed(shift_reg_byte);
  
  // Process the buttons connected directly to the microcontroller
  access_switch.check_button_pressed();
  play_button.check_button_pressed();
  stop_button.check_button_pressed();
  back_button.check_button_pressed();
  enter_button.check_button_pressed();
  
  // Process the leds that need to flash.
  play_led.update_flashing();
  wristband_leds.update_flashing();
 
  if(Serial1.available() > 0){              // If serial data is available...
    serial_parser();                        // ...call the parser.
  }

  // Get the current encoder position
  current_encoder_position = selection_encoder.track_position();

  // Has the value changed?
  if(current_encoder_position != last_encoder_position){
    // Print the value.
    Serial1.println(current_encoder_position);
    // Update the last known value.
    last_encoder_position = current_encoder_position;
  }
}

byte shift_in(int incoming_data_pin, int incoming_clock_pin){
  int i;
  int temp = 0;
  //int pin_state;
  byte data_in = 0;

  // Set the clock pin to high in preparation for
  // reading the first value.
  digitalWrite(incoming_clock_pin, 1);

  for(i = 7; i >= 0; i--){
    // The data pin changes value to the next pin of the
    // shift register on every transition from HIGH to LOW
    digitalWrite(incoming_clock_pin, 0);
    delayMicroseconds(0.2);
    temp = digitalRead(incoming_data_pin);
    // If the temp value is HIGH then...
    if(temp){
      // ...set the corresponding bit to 1.
      // The OR operation allows us to maintain the state of the other bits.
      data_in = data_in | (1 << i);
    }
    // Set the clock pin to high in preparation for
    // reading the next value.
    digitalWrite(incoming_clock_pin, 1);
  }
  return data_in;
}

void print_bits(byte incoming_byte){
  for(int i = 7; i >= 0; i--){
    Serial1.print(bitRead(incoming_byte,i));
  }
  Serial1.println();
}

void enter_pressed(Menu_Controller* p_menu_controller){                                // Function expects a pointer to a Menu_Controller.
  Menu_Page *m_menu = (Menu_Page*)p_menu_controller->m_currently_selected_menu;        // Create local pointer to the currently selected Menu_Page, via the Menu_Controller pointer.
  
  if(m_menu->m_enter_enabled){                                                         // if the currently selected menu has an on_enter() function...
    m_menu->on_enter();                                                                // ... call it.
  }                                                                   
}

void back_pressed(Menu_Controller* p_menu_controller){                                 // Function expects a pointer to a Menu_Controller.
  Menu_Page *m_menu = (Menu_Page*)p_menu_controller->m_currently_selected_menu;        // Create local pointer to the currently selected Menu_Page, via the Menu_Controller pointer.
  
  if(m_menu->m_back_enabled){                                                          // if the currently selected menu has an on_enter() function...
    m_menu->on_back();                                                                 // ... call it.
  }                                                      
}

void play_pressed(Single_Led *led, Parameter_Container *parameter_container, Parameter *parameter_struct, int parameter_value){
  // check to see if song is loaded before acting on play button events
  if(parameter_container->m_song_loaded.value == 1){
    switch(parameter_struct->value){
      case 0:
        parameter_container->set_and_send_parameter(parameter_struct, 1);         // Playback is currently stopped, so start it.
        led->set_on(true);                                                        // Update the led
        break;
      case 1:
        parameter_container->set_and_send_parameter(parameter_struct, 2);         // Song is playing already, so pause it.
        led->set_flashing(true);                                                  // Update the led
        break;
      case 2:
        parameter_container->set_and_send_parameter(parameter_struct, 1);         // Song is paused, so commence playback.
        led->set_on(true);                                                        // Update the led
        break;
    }
  }
}

void stop_pressed(Single_Led *led, Parameter_Container *parameter_container, Parameter *parameter_struct, int parameter_value){
  // check to see if song is loaded before acting on play button events
  if(parameter_container->m_song_loaded.value == 1){
    if(parameter_struct->value){
      parameter_container->set_and_send_parameter(parameter_struct, 0);          // Currently playing the song, so stop it.
      led->set_on(false);                                               
    }
  }
}

void song_pressed(Single_Led *led, Parameter_Container *parameter_container, Parameter *parameter_struct, int parameter_value){
  led->set_on(false);                                                                       // switch off the LED, which in this case is the play LED.
  parameter_container->set_and_send_parameter(&parameter_container->m_play, 0);             // set the play state to off.
  parameter_container->set_and_send_parameter(parameter_struct, parameter_value);           // update the currently selected song
}

void access_switch_pressed(Single_Led *led, Parameter_Container *parameter_container, Parameter *parameter_struct, int parameter_value){
  Rgb_Led *rgb_led = (Rgb_Led*)led;        // Create local pointer to the currently selected Menu_Page, via the Menu_Controller pointer.

  switch(parameter_struct->value){
    case 0:
      parameter_container->set_and_send_parameter(parameter_struct, 1);         // Increment parameter value
      rgb_led->set_colour(RGB_GREEN);                                           // Update the led
      break;
    case 1:
      parameter_container->set_and_send_parameter(parameter_struct, 2);         // Increment parameter value
      rgb_led->set_colour(RGB_BLUE);                                            // Update the led
      break;
    case 2:
      parameter_container->set_and_send_parameter(parameter_struct, 0);         // Reset parameter value to 0
      rgb_led->set_colour(RGB_RED);                                             // Update the led
      break;
  }
}

void serial_parser(){
  incoming_byte_1 = Serial1.read();
  delay(5);
  incoming_byte_2 = Serial1.read();
  delay(5);
  incoming_byte_3 = Serial1.read();
  delay(5);
  Serial.print("I received: ");
  Serial.print(incoming_byte_1);
  Serial.print(",");
  Serial.print(incoming_byte_2);
  Serial.print(",");
  Serial.println(incoming_byte_3);

  if(incoming_byte_1 == 97){
    // a character received
    parameter_container.m_master_level.value = (incoming_byte_2 + incoming_byte_3) - 96;
    
    // if associated page is the current menu page, redraw the display
    if(menu_controller.get_currently_selected_menu() == &master_level_menu){
      menu_controller.set_redraw_display_flag(true);
    }
    
  } else if(incoming_byte_1 == 98){ 
    // b character received
    // *** m_song parameter **
    parameter_container.m_song.value = (incoming_byte_2 + incoming_byte_3) - 96;
    
    // if associated page is the current menu page, redraw the display
    if(menu_controller.get_currently_selected_menu() == &main_menu){
      menu_controller.set_redraw_display_flag(true);
    }
    
  } else if(incoming_byte_1 == 99){
    // c character received
    // *** m_guitar parameter ***
  
    parameter_container.m_guitar.value = (incoming_byte_2 + incoming_byte_3) - 96;
    // if associated page is the current menu page, redraw the display
    if(menu_controller.get_currently_selected_menu() == &guitar_menu){
      menu_controller.set_redraw_display_flag(true);
    }
  } else if(incoming_byte_1 == 100){
    // d character received
    // *** m_guitar_level parameter ***
    parameter_container.m_guitar_level.value = (incoming_byte_2 + incoming_byte_3) - 96;
    
    // if associated page is the current menu page, redraw the display
    if(menu_controller.get_currently_selected_menu() == &guitar_level_menu){
      menu_controller.set_redraw_display_flag(true);
    }
  } else if(incoming_byte_1 == 101){
    // e character received
    // *** m_backing_level parameter ***
    parameter_container.m_backing_level.value = (incoming_byte_2 + incoming_byte_3) - 96;
    
    // if associated page is the current menu page, redraw the display
    if(menu_controller.get_currently_selected_menu() == &backing_level_menu){
      menu_controller.set_redraw_display_flag(true);
    }
  } else if(incoming_byte_1 == 102){
    // f character received
    // *** m_red_scale parameter ***
    parameter_container.m_red_scale.value = (incoming_byte_2 + incoming_byte_3) - 96;
    
    // if associated page is the current menu page, redraw the display
    if(menu_controller.get_currently_selected_menu() == &red_scale_menu){
      menu_controller.set_redraw_display_flag(true);
    }
  } else if(incoming_byte_1 == 103){
    // g character received
    // *** m_green_scale parameter ***
    parameter_container.m_green_scale.value = (incoming_byte_2 + incoming_byte_3) - 96;
    
    // if associated page is the current menu page, redraw the display
    if(menu_controller.get_currently_selected_menu() == &green_scale_menu){
      menu_controller.set_redraw_display_flag(true);
    }
  } else if(incoming_byte_1 == 104){
    // h character received
    // *** m_blue_scale parameter ***
    parameter_container.m_blue_scale.value = (incoming_byte_2 + incoming_byte_3) - 96;
    
    // if associated page is the current menu page, redraw the display
    if(menu_controller.get_currently_selected_menu() == &blue_scale_menu){
      menu_controller.set_redraw_display_flag(true);
    }
  } else if(incoming_byte_1 == 105){
    // i character received
    // *** m_red_root parameter ***
    parameter_container.m_red_root.value = (incoming_byte_2 + incoming_byte_3) - 96;
    
    // if associated page is the current menu page, redraw the display
    if(menu_controller.get_currently_selected_menu() == &red_root_menu){
      menu_controller.set_redraw_display_flag(true);
    }
  } else if(incoming_byte_1 == 106){
    // j character received
    // *** m_green_root parameter ***
    parameter_container.m_green_root.value = (incoming_byte_2 + incoming_byte_3) - 96;
    
    // if associated page is the current menu page, redraw the display
    if(menu_controller.get_currently_selected_menu() == &green_root_menu){
      menu_controller.set_redraw_display_flag(true);
    }
  } else if(incoming_byte_1 == 107){
    // k character received
    // *** m_blue_root parameter ***
    parameter_container.m_blue_root.value = (incoming_byte_2 + incoming_byte_3) - 96;
    
    // if associated page is the current menu page, redraw the display
    if(menu_controller.get_currently_selected_menu() == &blue_root_menu){
      menu_controller.set_redraw_display_flag(true);
    }
  } else if(incoming_byte_1 == 108){
    // l character received
    // *** m_zone parameter ***
    parameter_container.m_zone.value = (incoming_byte_2 + incoming_byte_3) - 96;

    switch(parameter_container.m_zone.value){
      case 0:
        zone_leds.set_colour(RGB_RED);
        break;
      case 1:
        zone_leds.set_colour(RGB_GREEN);
        break;
      case 2:
        zone_leds.set_colour(RGB_BLUE);
        break;
    }
  } else if(incoming_byte_1 == 111){
    // o character received
    // *** m_play parameter ***
    play_led.set_flashing(false);
    play_led.set_on(false);
    parameter_container.m_play.value = 0;
    
  }
    else if(incoming_byte_1 == 112){
    // Attempting to connect to wristband
    menu_controller.set_currently_selected_menu(&reconnect_menu);                             // Switch to splash menu.
     
  } else if(incoming_byte_1 == 113){
    // Successfully connected to wristband.
    menu_controller.set_currently_selected_menu(p_wristband_return_page);
    wristband_leds.set_flashing(false);
    wristband_leds.set_colour(RG_GREEN);
    
  } else if(incoming_byte_1 == 114){
     menu_controller.set_currently_selected_menu(&connection_fail_menu);
     splash_page_loaded = true;
     time_splash_loaded_ms = millis();
     
  } else if(incoming_byte_1 == 116){
    // song_loaded parameter received (i.e. 't')
    parameter_container.m_song_loaded.value = (int)incoming_byte_3 - 48;

    // display notification onscreen if song couldn't be loaded
    if(parameter_container.m_song_loaded.value == 0){
      menu_controller.set_currently_selected_menu(&song_fail_menu);
      splash_page_loaded = true;
      time_splash_loaded_ms = millis();
    }
     
  } else if(incoming_byte_1 == 121){
    // Convert ascii characters to int note value.
    int note_number = (int)((incoming_byte_2 - 48) * 10) + (int)(incoming_byte_3 - 48);
    usbMIDI.sendNoteOn(note_number, NOTE_VELOCITY, 0);
    
  }  else if(incoming_byte_1 == 122){
    // Convert ascii characters to int note value.
    int note_number = (int)((incoming_byte_2 - 48) * 10) + (int)(incoming_byte_3 - 48);
    usbMIDI.sendNoteOff(note_number, NOTE_VELOCITY, 0);
  } 
}
