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

// you do what you want to test anything
// Or use this module as a template to create an other module.
// rename the 3 functions and declare it in the nozori.h module list

// look at other module for usefull function

// Pot 1 : 
// Pot 2 : 
// Pot 3 : 
// Pot 4 : 
// Pot 5 : 
// Pot 6 : 
// CV 1 : 
// CV 2 : 
// CV 3 : 
// CV 4 : 
// IN 1 : 
// IN 2 : 
// Selecteur3 :
// OUT 1 : 
// OUT 2 : 


// put here global variable that should be shared between data function and audio function

inline void template_init_() {
  // this function is used only once, for the module initialisation
  set_led1(0);
  set_led2(0);
  set_led3(0);
  set_led4(0);
}

inline void template_loop_() {
  // this fuction run in loop, as fast as possible. it should run faster than 15KHz.
  // only control frequency data should be computed here.
  
  uint32_t tmp;
  uint32_t addresse;
  filter16_nozori_68  // macro that get CV and potentiometter value
  test_connect_loop_68(); // macro that test if a jack is plugged or not

  //led1(audio_inL>>24);
  //if (CV1_connect < 60) led1(255); else led1(0);
  //if (CV2_connect < 60) led2(255); else led2(0);
  //if (CV3_connect < 60) led3(255); else led3(0);
  //if (CV4_connect < 60) led4(255); else led4(0);

  if (IN1_connect < 60) led2(255); else led2(0);
  if (IN2_connect < 60) led4(255); else led4(0);

  //led4(ADC->ADC_CDR[table_adc[0]]>>3);
  //led4(REG_pot1>>3);

  //led1(REG_pot1>>3);
  //led2(REG_pot2>>3);
  //led3(REG_pot3>>3);
  //led4(REG_pot4>>3);

  //led1(REG_CV1>>3);
  //led2(REG_CV2>>3);
  //led3(REG_CV3>>3);
  //led3(CV_filter16_out[index_filter_pot1]>>7);
  //led4(REG_CV4>>3);
  //if(REG_pot1 < (1<<11)) default1_0V(); else default1_5V();
  //if(REG_pot2 < (1<<11)) default2_0V(); else default2_5V();
  //if(REG_pot3 < (1<<11)) default3_0V(); else default3_5V();
  //if(REG_pot4 < (1<<11)) default4_0V(); else default4_5V();
  
  // led1(get_switch*255);
  // set_led1(get_switch());
  // analog_out_1(REG_pot1<<20);
  // analog_out_2(REG_pot2<<20);
  //analog_out_1(random32());
  //serial_out(55);
  //analog_out_1(0x7FFFFFFF);
  //analog_out_2(fast_sin(VCO1_phase));
  
}

inline void template_audio_() {
  // this function is run for EVERY sample
  // at 96KHz, this function should run in less then 3µs!
  // input are audio_inL, audio_inR, and output are audio_outL and audio_outR : they all are uint32_t
  
  uint32_t i;
  uint32_t tmp;
  
  //chaos(5);
  VCO1_phase += 0x1000000;
  
  //audio_outR = audio_inR;
  //audio_outL = phase;
  // audio_outL = audio_inL;

  //audio_outR = 0xFFFFFFFF;
  //audio_outR = 0x80000000;
  //audio_outR = 0x80000000 + (phase>>1);
  //audio_outR = 0x7FFFFFFF;
  //audio_outR = 0xC0000000+(phase>>1);
  //audio_outL = 0x7F800000 + (fast_sin(VCO1_phase)>>2);
  //audio_outR = OUT1_0V;
  audio_outR = audio_inR;
  //audio_outL = fast_sin(VCO1_phase);
  //audio_outL = audio_inL;
  audio_outL = audio_inL;

  analog_out_1(audio_inR);
  analog_out_2(audio_inL);
  
  //analog_out_1(phase);
  //analog_out_2(fast_sin(phase));

  //tmp =  chaos_X^0x80000000;
  //audio_outR = (chaos_X<<16)^0x80000000;
  //tmp =  chaos_Y^0x80000000;
  //audio_outL = (chaos_Y<<16)^0x80000000;
  //audio_outR = chaos_dx^0x80000000;
  //audio_outL = chaos_dy^0x80000000;
}

