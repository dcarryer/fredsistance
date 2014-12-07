/*
 * Project: Fredsistance
 * ---------------------
 *    Author: David Carryer, david@carryer.me (http://www.carryer.me)
 *    Created: December 4, 2014
 *    Copyright 2014 David Carryer
 *    
 *    This file is part of the Fredsistance Pebble Watchface.
 * 
 *    Foobar is free software: you can redistribute it and/or modify it under 
 *    the terms of the GNU General Public License as published by the Free 
 *    Software Foundation, either version 3 of the License, or (at your option) 
 *    any later version.
 *
 *    Foobar is distributed in the hope that it will be useful, but WITHOUT 
 *    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 *    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
 *    more details.
 *  
 *    You should have received a copy of the GNU General Public License along 
 *    with the Fredsistance Pebble Watchface source code. If not, 
 *    see http://www.gnu.org/licenses/.
 */

// Main directive to use the Pebble SDK
#include <pebble.h>
  
// Declare a static pointer to a Window variable, so that we can 
// access it wherever we need to.  
static Window *s_main_window;

// Declare a static pointer to a TextLayer variable, so that we can
// show time text on the screen.
static TextLayer *s_time_layer;

// Declare a static pointer to a TextLayer variable, so that we can
// show date text on the screen.
static TextLayer *s_date_layer;

// Declare a static pointer to a BitmapLayer variable, so that we can
// show a bitmap on the screen.
static BitmapLayer *s_background_layer;

// Declare a static point that contains the background image.
static GBitmap *s_background_bitmap;

// Declare static pointers for Coda (Ingress Inspired) font.
static GFont s_custom_font_coda_regular_20;
static GFont s_custom_font_coda_caption_heavy_24;

/*
 * Function: update_time()
 * ------------------------------------------
 *    Contains the logic to update the time in the TextLayer.   
 */
static void update_time() {
  
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // TIME
  
  // Create a long-lived buffer
  static char tbuffer[] = "00:00 XX";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(tbuffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(tbuffer, sizeof("00:00 XX"), "%I:%M %p", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, tbuffer);
  
  // DATE
  
  // Create a long-lived buffer
  static char dbuffer[] = "XXX, XX/XX/XX";

  // Write the current hours and minutes into the buffer
  strftime(dbuffer, sizeof("XXX, XX/XX/XX"), "%a, %D", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_date_layer, dbuffer); 
}


/*
 * Function: main_window_load(Window *window)
 * ------------------------------------------
 *    Window *window: Pointer to window.
 *
 *    Handler functions that provides an additional layer of abstraction 
 *    to manage the subsequent creation of the Window's sub-elements. 
 */
static void main_window_load(Window *window) {
  // Create GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHFACE_BG);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 5, 144, 35));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite); 
  s_custom_font_coda_caption_heavy_24 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_CODA_HEAVY_24));
  text_layer_set_font(s_time_layer, s_custom_font_coda_caption_heavy_24);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
   // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));  
  
  // Create temperature Layer
  s_date_layer = text_layer_create(GRect(0, 133, 144, 25));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  s_custom_font_coda_regular_20 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_CODA_REGULAR_20));
  text_layer_set_font(s_date_layer, s_custom_font_coda_regular_20);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer)); 
}


/*
 * Function: main_window_unload(Window *window)
 * ------------------------------------------
 *    Window *window: Pointer to window.
 *
 *    Handler functions that provides an additional layer of abstraction 
 *    to manage the subsequent destruction of the Window's sub-elements.  
 */
static void main_window_unload(Window *window) {
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);
  
  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);  
  
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  
  // Destroy date element
  text_layer_destroy(s_date_layer);  
}


/*
 * Function: tick_handler(struct tm *tick_time, TimeUnits units_changed)
 * ------------------------------------------
 *    struct tm *tick_time: Data structure containing current time.
 *    TimeUnits units_changed: Constant tells us which value changed.
 * 
 *    This function is called whenever the time changes, based on
 *    settings set with tick_timer_service_subscribe.
 */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  // Call logic for updating the time
  update_time();
}


/*
 * Function: init()
 * ------------------------------------------
 *    Used to organize the creation of all the Pebble SDK elements.  
 */
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);  
}


/*
 * Function: deinit()
 * ------------------------------------------
 *    Used to organize the destruction of all the Pebble SDK elements.  
 */
static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}


/*
 * Function: main(void)
 * ------------------------------------------
 *    The first function to be called when the program starts execution.   
 */
int main(void) {
  // Initiate Pebble SDK elements.
  init();

  // Lets the watchapp wait for system events until it exits.
  app_event_loop();

  // Destroy Pebble SDK elements.
  deinit();
} 



  