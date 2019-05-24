// Variable shared beetween multiple modules

// global
uint32_t state;

//Oscilators:
uint32_t increment_0,  increment_1, increment_2, increment_3, increment_4, increment_5, increment_6;
uint32_t sin1_phase, sin2_phase, sin3_phase, sin4_phase;
uint32_t VCO1_phase, VCO2_phase;
uint32_t VCO_H1_phase, VCO_H2_phase, VCO_H3_phase;

// VCF:
int32_t in1_oldL,in2_oldL, in3_oldL, in4_oldL, in5_oldL;
int32_t in1_oldR,in2_oldR, in3_oldR, in4_oldR, in5_oldR;
uint32_t reso, reso2;
int32_t G1, G2, G3, G5;
int32_t G12, G22, G32, G52;
uint32_t freq_global, freq_global2;

// ADSR
uint32_t ADSR1_gate, ADSR1_status, ADSR1_goal, ADSR1_filter, ADSR1_out;
uint32_t ADSR2_gate, ADSR2_status, ADSR2_goal, ADSR2_filter, ADSR2_out;
// status : 0 = attack, 1 = decay, 2 = release
bool gate;

// LFO
uint32_t LFO1_phase, LFO2_phase, LFO3_phase, LFO4_phase;
int32_t LFO1_increment, LFO2_increment;
uint32_t symetrie_1, distortion_1, distortion2_1, gain_1, offset_gain_1;
int32_t offset_signed_1;
uint32_t symetrie_2, distortion_2, distortion2_2, gain_2, offset_gain_2;
int32_t offset_signed_2;
uint32_t actualise_LFO1, actualise_LFO2;
volatile uint32_t hold, hold2;
uint32_t reset1, reset2;

// effect
int32_t dry1_goal, dry2_goal;
int32_t dry1_save, dry2_save, bt1_save, bt2_save;

// delay
uint32_t delay_time_global, FB_global, gain_global, bypass_global;
int32_t filter_audio_out, filter_audio_in;
uint32_t delay_time_save, FB_save, gain_save, bypass_save;
uint32_t filter_LOP_global, filter_HIP_global;
uint32_t filter_LOP_save, filter_HIP_save;

#define Max_Delay 0x7FFF // must be 2^n - 1 (attention, ne pas changer)
union delay_data
{
  int16_t S16[Max_Delay+1];
  uint16_t U16[Max_Delay+1];
  int32_t S32[(Max_Delay+1)>>1];
  uint32_t U32[(Max_Delay+1)>>1];
} delay_line;
uint32_t index_ecriture;

// Gran
volatile uint32_t grain1_pos, grain1_pos_lecture, grain1_offset, grain1_offset2, grain1_size;
volatile uint32_t grain2_pos, grain2_pos_lecture, grain2_offset, grain2_offset2, grain2_size;
volatile int32_t grain1_speed, grain2_speed, grain1_speed2, grain2_speed2;
volatile bool start_new_grain, start_new_grain_old, grain1_used, grain2_used;

//Chaos
uint32_t thomas[3][64]; // 64 thomas chaotic attractor

//clock 
uint32_t last_clock_;
uint32_t nb_tick;
int32_t clock_diviseur, clock_multiplieur;
const uint32_t tab_diviseur[] = {16, 8, 4, 2, 1, 1, 1, 1, 1};
const uint32_t tab_multiplieur[] = {1, 1, 1, 1, 1, 2, 4, 8, 16};
const uint32_t tab_multiplieur2[] = {2, 3, 4, 1, 5, 4, 3};
const uint32_t tab_diviseur2[]    = {3, 4, 5, 1, 4, 3, 2};

//rnd loop
const uint32_t possible_step[] = { 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 24, 32};
uint32_t nb_step, current_step, last_step;
int32_t last_interpol_valueX, current_interpol_valueX, current_interpol_valueX2, previous_interpol_valueX; // 4 value for cubic interpolation
int32_t last_interpol_valueY, current_interpol_valueY, current_interpol_valueY2, previous_interpol_valueY;
uint32_t rnd_all_save;
uint32_t loop_index;

// interpolation 
uint32_t interpol_pos;

// various
uint32_t potA_save, potB_save, potC_save, potD_save;
uint32_t last_CV1;

// filter
uint32_t low_gain, medium_gain, high_gain;

// calibration
uint32_t filtre_audioL, filtre_audioR;

//syncro 
int32_t clock1_diviseur, clock1_multiplieur;
int32_t clock2_diviseur, clock2_multiplieur;
uint32_t last_clock_1, last_clock_2;
uint32_t nb_tick1, nb_tick2;

