// NOZORI
// based on arduino duo
//
// main file
// --------------------------------------------------------------------------
// This file is part of the NOZORI firmware.
//
//    NOZORI firmware is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    NOZORI firmware is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with NOZORI firmware. If not, see <http://www.gnu.org/licenses/>.
// --------------------------------------------------------------------------

#include <Arduino.h>
#pragma GCC optimize ("-O3")


// Change this option befor compilling for your hardware

#define NOZORI_68
//#define NOZORI_84

//#define NOZORI_VERSION 100
//#define NOZORI_VERSION 102 // 1.2 : demo version
//#define NOZORI_VERSION 103 // 1.3 : test version
#define NOZORI_VERSION 104 // 1.4, 1.5, 1.6 // this is the commercial version

////////////////////////////////////////////////////////////////////
//#define force_module_type 161
///////////////////////////////////////////////////////////////////

// control timing for debug purpose
//#define syncro_out

#include "a_tables.h"
#include "b_nozori.h"
#include "c_macro.h"

uint32_t module_type, toggle_global;
volatile uint32_t audio_inR,  audio_inL;
volatile uint32_t audio_outR, audio_outL;

volatile uint32_t sample[10][4]; //(3 sample + index) : sample[n][3] = index du dernier sample enregistré
volatile uint32_t ADC_IN[10];  // valeur apres filtrage

volatile uint32_t CV1_connect, CV2_connect, CV3_connect, CV4_connect, IN1_connect, IN2_connect; // is connected or not
volatile uint32_t connect_timer;
volatile uint32_t connect_test_value;

uint32_t CV1_0V, CV2_0V, CV3_0V, CV4_0V, IN1_0V, IN2_0V, IN1_1V_value, IN2_1V_value;
uint32_t CV1_1V, CV2_1V, CV3_1V, CV4_1V, IN1_1V, IN2_1V;
uint32_t OUT1_0V, OUT1_1V, OUT2_0V, OUT2_1V;
int32_t CV1_1Vminus0V, CV2_1Vminus0V, CV3_1Vminus0V, CV4_1Vminus0V;

#define filter_order 6 // nb de sample a utiliser ds le filtre des CV
uint32_t CV_filter16[12], CV_filter16_out[12], filter_index[12];
int32_t CV_filter_moins[12][filter_order], CV_filter_plus[12][filter_order];

void (*pointeurSurFonction_init)(void);
void (*pointeurSurFonction_loop)(void);
void (*pointeurSurFonction_audio)(void);

void setup() {
  uint32_t i;
  uint32_t test;
  
  REG_PMC_PCER0 = 1 << 11; // enable la clock du port PIO A pour les entree
  REG_PMC_PCER0 = 1 << 12; // enable la clock du port PIO B pour les entree
  REG_PMC_PCER0 = 1 << 13; // enable la clock du port PIO C pour les entree
  REG_PMC_PCER0 = 1 << 14; // enable la clock du port PIO D pour les entree

  REG_SUPC_SMMR = 0x0000110B; // suply monitor reset at 3V
  REG_SUPC_MR = 0xA5005A00;

  init_led();
  init_analog_in();  
  init_toggle(); 
  init_switch();
  init_analog_out();
  init_random();
  init_dac();
  init_serial(); // serial out is not tested
  init_test(); 
  init_save();
  init_calibration();
  init_test_connect_loop();

  // ADC init when using a firmware that is not for the correct nozori board
  CV_filter16_out[6]  = 0;      // pot7
  CV_filter16_out[7]  = 0;      // pot8
  CV_filter16_out[8]  = 0x7FFF; // CV1
  CV_filter16_out[9]  = 0x7FFF; // CV2
  CV_filter16_out[10] = 0x7FFF; // CV3
  CV_filter16_out[11] = 0x7FFF; // CV4

  module_type = get_switch();
  #ifdef force_module_type
    module_type = force_module_type;
  #endif

/*
  // initialize CV and CV_connect
//  #if NOZORI_VERSION > 103
    for (i=0; i<511; i++) { // delay to initialize all voltages
      led2(i);
      delay(10);
      if( (i>100) && (i<120) ) set_led4(255); else set_led4(0);
    }
//  #endif
*/

  if ( rom_read_int32(rom_address_IN1_0V) == 0xFFFFFFFF) { // flash because no calibration was recorded
    for (i=0; i<4; i++) {
      delay(150);
      set_led2(255);
      set_led4(255);
      delay(150);
      set_led2(0);
      set_led4(0);
    }
  }
  
  // get pointer to module function
  switch (module_type) { // init function
    switch_module_type(init)
  }
  switch (module_type) { // data loop function
    switch_module_type(loop)
  }
  switch (module_type) { // audio loop function
    switch_module_type(audio)
  }

   set_led2(20);
   set_led4(20);

 for (i=0; i<100; i++) { // initialize all digital filter value
    delay(1);
    filter16_nozori
  }

  test = test_mode();
  if (test == 7) { // pin 7 et 6 connecté ensemble
    test_1_init_();
    pointeurSurFonction_audio = test_1_audio_;
    start_dac();
    while (true) {
      test_1_loop_();
    }
  }
  if (test == 6) { // pin 6 et 5 connecté ensemble
    test_2_init_();
    pointeurSurFonction_audio = test_2_audio_;
    start_dac();
    while (true) {
      test_2_loop_();
    }
  }
  if (test == 5) { // pin 5 et 4 connecté ensemble
    calibrate_serial_init_();
    pointeurSurFonction_audio = calibrate_serial_audio_;
    start_dac();
    while (true) {
      calibrate_serial_loop_();
    }
  }
  if (test == 4) { // pin 4 et 3 connecté ensemble
    calibrate_pd_init_();
    pointeurSurFonction_audio = calibrate_pd_audio_;
    start_dac();
    while (true) {
      calibrate_pd_loop_();
    }
  }
  
  if (test == 2) { // pin 2 et 1 connecté ensemble
    calibrate_manual_2_init_();
    pointeurSurFonction_audio = calibrate_manual_2_audio_;
    start_dac();
    while (true) {
      calibrate_manual_2_loop_();
    }
  }
  if (test == 1) { // pin 1 et 0 connecté ensemble
    toggle_global = get_toggle(); 
    pointeurSurFonction_audio = calibrate_manual_1_audio_;
    calibrate_manual_1_init_();
    start_dac();
    while (true) {
      calibrate_manual_1_loop_();
    }
  }

  // init
  //////////////////////////////////
  toggle_global = get_toggle(); 
  (*pointeurSurFonction_init)();
  start_dac();

  // loop
  /////////////////////////////////
  while (true) {
    // Start fct loop
    (*pointeurSurFonction_loop)();  

    #ifndef force_module_type
      if (get_switch() != module_type) { // le N° du module a changé, il faut reset le pross et le dac
        REG_RSTC_CR = 0xA5000001; // reboot
      }
    #endif
    
    #ifdef syncro_out
  	  test1_toggle();
  	#endif
  }
}
 
void loop() {
  //not used
}

void SSC_Handler(void){
uint32_t status_value;

  #ifdef syncro_out
    test2_on();
  #endif

  if (!(REG_SSC_SR & SSC_IER_TXSYN)) {
    REG_SSC_THR = REG_SSC_RHR; // just to initialise properlly (not to invert R and L)
  }
  else {
    #if NOZORI_VERSION < 103
      audio_inR = 0x7FFFFFFF-REG_SSC_RHR;
    #else
      audio_inR = REG_SSC_RHR^0x80000000;           // 0 -> -7V ; 0xFFFFFFFF -> 7V
    #endif

    REG_SSC_THR = (-(audio_outR+0x80000001)>>8);    // idem
    connect_timer++;
   
    (*pointeurSurFonction_audio)();  

    while(!(REG_SSC_SR & (1<<4))) {} // wait for the next sample to be ready 
    NVIC_ClearPendingIRQ(SSC_IRQn);
    REG_SSC_THR = (-(audio_outL+0x80000001)>>8); 
    #if NOZORI_VERSION < 103
      audio_inL = 0x7FFFFFFF-REG_SSC_RHR;
    #else
      audio_inL = REG_SSC_RHR^0x80000000;           // 0 -> -7V ; 0xFFFFFFFF -> 7V
    #endif
  }
  
  #ifdef syncro_out
    test2_off();
  #endif
}

