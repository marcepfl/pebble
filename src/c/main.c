
/*---------------------------------------------------------------------------
TP based on template of the course "System Engineering" 2016, EPFL

Authors: Matthieu Boubat Marc Dumont Ivan Pantelic
Version: 1.0
Date: 07.11.2016
---------------------------------------------------------------------------*/


// Include Pebble library
#include <pebble.h>
#include <math.h>


static void accel_data_handler(AccelData *data, uint32_t num_samples);

static int countSteps=0;
static bool ono=1;
static int StepPerHour=0;
static int count=0;

// Declare the main window and multiple text layers
Window *main_window;
TextLayer *background_layer;
TextLayer *gait_count_layer_title;
TextLayer *gait_count_layer;
TextLayer *gait_speed_layer_title;
TextLayer *gait_speed_layer;
ActionBarLayer *action_bar;

static GBitmap *pause_icon;
static GBitmap *reset_icon;

// Click handler
void up_single_click_handler(ClickRecognizerRef recognizer, void *context){
    /*/////////////////////////////////////*/

  if(ono)  ono=0;
  else  ono=1;

    
      
    /*/////////////////////////////////////*/
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    countSteps = 0 ;
}

// single click config:
void click_config_provider(Window *window) {
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
}

// Init function called when app is launched
static void init_window(void) {
  
    // Create main Window element and assign to pointer
    main_window = window_create();
    Layer *window_layer = window_get_root_layer(main_window);  
  
    // Create background Layer
    background_layer = text_layer_create(GRect( 0, 0, 144, 168));
  
    // Setup background layer color (black)
    text_layer_set_background_color(background_layer, GColorBlack);
  
    // Create text Layer gait count TITLE
    gait_count_layer_title = text_layer_create(GRect( 10, 20, 100, 25));
    text_layer_set_background_color(gait_count_layer_title, GColorClear);
    text_layer_set_text_color(gait_count_layer_title, GColorWhite);	
    text_layer_set_font(gait_count_layer_title, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(gait_count_layer_title, GTextAlignmentLeft);
  
    // Title display
    static char title_1[60]= "Count [gait]";
    text_layer_set_text(gait_count_layer_title, title_1);
  
    // Create text Layer gait count
    gait_count_layer = text_layer_create(GRect( 0, 40, 100, 40));
    text_layer_set_background_color(gait_count_layer, GColorClear);
    text_layer_set_text_color(gait_count_layer, GColorWhite);	
    text_layer_set_font(gait_count_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(gait_count_layer, GTextAlignmentCenter);
  
    // Create text Layer gait speed TITLE
    gait_speed_layer_title = text_layer_create(GRect( 10, 100, 100, 25));
    text_layer_set_background_color(gait_speed_layer_title, GColorClear);
    text_layer_set_text_color(gait_speed_layer_title, GColorWhite);	
    text_layer_set_font(gait_speed_layer_title, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(gait_speed_layer_title, GTextAlignmentLeft);
  
    // Title display
    static char title_2[60]= "Speed [g/h]";
    text_layer_set_text(gait_speed_layer_title, title_2);
  
    // Create text Layer gait speed
    gait_speed_layer = text_layer_create(GRect( 0, 120, 100, 40));
    text_layer_set_background_color(gait_speed_layer, GColorClear);
    text_layer_set_text_color(gait_speed_layer, GColorWhite);	
    text_layer_set_font(gait_speed_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(gait_speed_layer, GTextAlignmentCenter);
  
    // Create action bar on the right
    action_bar = action_bar_layer_create();
    action_bar_layer_set_background_color(action_bar, GColorWhite);
    action_bar_layer_set_click_config_provider(action_bar, (ClickConfigProvider) click_config_provider);
  
    // Set the icons:
    pause_icon = gbitmap_create_with_resource(RESOURCE_ID_PAUSE_ICON);
    reset_icon = gbitmap_create_with_resource(RESOURCE_ID_RESET_ICON);
    action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_UP, pause_icon, true);
    action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_DOWN, reset_icon, true);
  
  
    // Add layers as childs layers to the Window's root layer
    layer_add_child(window_layer, text_layer_get_layer(background_layer));
    layer_add_child(window_layer, text_layer_get_layer(gait_count_layer_title));    
    layer_add_child(window_layer, text_layer_get_layer(gait_count_layer));
    layer_add_child(window_layer, text_layer_get_layer(gait_speed_layer_title));
    layer_add_child(window_layer, text_layer_get_layer(gait_speed_layer));
    action_bar_layer_add_to_window(action_bar, main_window);
  
    // Show the window on the watch, with animated = true
    window_stack_push(main_window, true);
  
}

static void init_acc(void) {
  // Define number of samples 25 hz=1sec
  uint32_t num_samples = 25;
  
  // Allow Accelerometer Event
  accel_data_service_subscribe(num_samples, accel_data_handler);
  
  // Define accelerometer sampling rate
  accel_service_set_sampling_rate(ACCEL_SAMPLING_25HZ);
  
}

// deinit function init_acc
static void deinit_acc(void) {
    // stop acc
    accel_data_service_unsubscribe();
}

//Function accel_data_handler
static void accel_data_handler(AccelData *data, uint32_t num_samples)
{
    
  int16_t xx=data[1].x;
  int16_t yy=data[1].y;
  int16_t zz=data[1].z;

  int16_t px=data[0].x;
  int16_t py=data[0].y;
  int16_t pz=data[0].z;



  double dot[num_samples];


  double a=1;
  double b=1;


  int test=0;
  int i=0;
  
  int countPerLoop=0;
  
  
  //Read samples x,y,z
  for(i=0; i<(int)num_samples;i++)
  {
    xx = data[i].x;
    yy = data[i].y;
    zz = data[i].z;
    
    

    px = data[i+1].x;
    py = data[i+1].x;
    pz = data[i+1].x;
  


    dot[i] = fabs(((px * xx) + (py * yy) + (pz * zz))*((px * xx) + (py * yy) + (pz * zz)));
    a = fabs(px * px + py * py + pz * pz);
    b = fabs(xx * xx + yy * yy + zz * zz);
    dot[i] /= (a * b);

    if(dot[i]<0.035 && ono)
    {
      
      test++;

      if(test>=15)
      {

        countSteps++;
        countPerLoop++;
        test=0;
        
      }  
    }
  }
    count++;
    StepPerHour=(StepPerHour*(count-1)+countPerLoop*3600)/count;
    
    
  
    // Tab of char to print gait result on window
    // Print result Watch
    static char count_table[60];
    snprintf(count_table, 60, "%d", countSteps);
    text_layer_set_text(gait_count_layer, count_table);
  
    static char gait_speed_table[60];
    snprintf(gait_speed_table, 60, "%d", StepPerHour);
    text_layer_set_text(gait_speed_layer, count_table);
}

// deinit function called when the app is closed
static void deinit_window(void) {
    // Destroy layers and main window
    text_layer_destroy(background_layer);
    text_layer_destroy(gait_count_layer_title);
  	text_layer_destroy(gait_count_layer);
  	text_layer_destroy(gait_speed_layer_title);
  	text_layer_destroy(gait_speed_layer);
    action_bar_layer_destroy(action_bar);
    window_destroy(main_window);
}

int main(void) {
    init_window();
    init_acc();
    app_event_loop();
    deinit_acc();
    deinit_window();
}
