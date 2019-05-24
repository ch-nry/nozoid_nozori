// macro definition 

#define _min(X, Y) (((X) < (Y)) ? (X) : (Y))
#define _max(X, Y) (((X) > (Y)) ? (X) : (Y))

#define macro_fq_in_tlg                                     \
int32_t freq;                                               \
  switch (get_toggle()) {                                   \
  case 0 : /* High : full audio range*/                     \
    freq = (CV_filter16_out[index_filter_pot1])<<11;        \
    freq += 0x07C00000;                                     \
  break;                                                    \
  case 1 : /* medium (for external control using 1V/Oct) */ \
    freq = (CV_filter16_out[index_filter_pot1])<<9;         \
    freq += 0x09000000;                                     \
  break;                                                    \
  case 2 :  /* low (LFO) */                                 \
    freq = (CV_filter16_out[index_filter_pot1])<<11;        \
    freq += 0x03000000;                                     \
  break;                                                    \
  }

#define macro_fq_in_tlg_pot(pot)                            \
  switch (get_toggle()) {                                   \
  case 0 : /* High : full audio range*/                     \
    freq = (CV_filter16_out[index_filter_ ## pot])<<11;     \
    freq += 0x07C00000;                                     \
  break;                                                    \
  case 1 : /* medium (for external control using 1V/Oct) */ \
    freq = (CV_filter16_out[index_filter_ ## pot])<<9;      \
    freq += 0x09000000;                                     \
  break;                                                    \
  case 2 :  /* low (LFO) */                                 \
    freq = (CV_filter16_out[index_filter_ ## pot])<<11;     \
    freq += 0x03000000;                                     \
  break;                                                    \
  }

#define macro_fq_in                                         \
int32_t freq;                                               \
  freq = (CV_filter16_out[index_filter_pot1])<<11;          \
  freq += 0x06000000;

#define macro_fq_in_novar                                   \
  freq = (CV_filter16_out[index_filter_pot1])<<11;          \
  freq += 0x06000000;

#define macro_1VOct_CV1                                     \
  if (CV1_connect < 60) {                                   \
    tmpS = CV_filter16_out[index_filter_cv1];               \
    tmpS -= CV1_0V;                                         \
    tmpS *= CV1_1V;                                         \
    freq += tmpS;                                           \
  }

#define macro_1VOct_CV1_Q                                   \
  if (CV1_connect < 60) {                                   \
    tmpS = CV_filter16_out[index_filter_cv1];               \
    tmpS -= CV1_0V;                                         \
    tmpS *= CV1_1V;                                         \
    tmpS &= 0xFFFFF000;                                      \
    freq += tmpS;                                           \
  }
 
#define macro_1VOct_IN1                                     \
  if (IN1_connect < 60) {                                   \
    tmpS = audio_inL>>16;                                   \
    tmpS -= IN1_0V>>16;                                     \
    tmpS *= IN1_1V;                                         \
    freq += tmpS;                                           \
  }

#define macro_FqMod_fine_simple(pot)                        \
    freq += CV_filter16_out[index_filter_ ## pot]*(48<<2);  \

#define macro_FqMod_fine(pot,CV)                            \
  if (CV ## _connect < 60) {                                \
    tmpS = CV_filter16_out[index_filter_ ## CV];            \
    tmpS -= CV ## _0V;                                      \
    tmpS *= min(4086<<4,CV_filter16_out[index_filter_ ## pot]);\
    tmpS /= (4086<<4);                                      \
    tmpS *= CV ## _1V;                                      \
    freq += tmpS;                                           \
  }                                                         \
  else { /* fine tune */                                    \
    freq += CV_filter16_out[index_filter_ ## pot]*(48<<2);  \
  }

#define macro_FqMod_fine_IN1(pot)                           \
  if (IN1_connect < 60) {                                   \
    tmpS = audio_inL>>17;                                   \
    tmpS -= IN1_0V>>17;                                     \
    tmpS *= min(4086<<4, CV_filter16_out[index_filter_ ## pot]); \
    tmpS /= (4086<<3);                                      \
    tmpS *= IN1_1V;                                         \
    freq += tmpS;                                           \
  }                                                         \
  else { /* fine tune */                                    \
    freq += CV_filter16_out[index_filter_ ## pot]*(48<<2);  \
  }

#define macro_FqMod_fine_IN2(pot)                           \
  if (IN2_connect < 60) {                                   \
    tmpS = audio_inR>>17;                                   \
    tmpS -= IN2_0V>>17;                                     \
    tmpS *= min(4086<<4, CV_filter16_out[index_filter_ ## pot]); \
    tmpS /= (4086<<3);                                      \
    tmpS *= IN2_1V;                                         \
    freq += tmpS;                                           \
  }                                                         \
  else { /* fine tune */                                    \
    freq += CV_filter16_out[index_filter_ ## pot]*(48<<2);  \
  }

/*
#define macro_FqMod_IN1(pot)                                \
  if (IN1_connect < 60) {                                   \
    tmpS = audio_inL>>17;                                   \
    tmpS -= IN1_0V>>17;                                     \
    tmpS *= min(4086<<4, CV_filter16_out[index_filter_ ## pot]); \
    tmpS /= (4086<<3);                                      \
    tmpS *= IN1_1V;                                         \
    freq += tmpS;                                           \
  }

#define macro_FqMod_IN2(pot)                                \
  if (IN2_connect < 60) {                                   \
    tmpS = audio_inR>>17;                                   \
    tmpS -= IN2_0V>>17;                                     \
    tmpS *= min(4086<<4, CV_filter16_out[index_filter_ ## pot]); \
    tmpS /= (4086<<3);                                      \
    tmpS *= IN2_1V;                                         \
    freq += tmpS;                                           \
  }
*/

// overflow possible si CV-CV_0V n'est pas centré
#define macro_FqMod(pot,CV)                                 \
  if (CV ## _connect < 60) {                                \
    tmpS = CV_filter16_out[index_filter_ ## CV];            \
    tmpS -= CV ## _0V;                                      \
    tmpS *= CV_filter16_out[index_filter_ ## pot];          \
    tmpS >>= 16;                                            \
    tmpS *= CV ## _1V;                                      \
    freq += tmpS;                                           \
  }

#define macro_FqMod_value(pot, CV, value)                   \
  tmpS = value;                                             \
  tmpS *= CV_filter16_out[index_filter_ ## pot];            \
  tmpS >>= 16;                                              \
  tmpS *= CV ## _1V;                                        \
  freq += tmpS;

    
# define macro_fq2increment                                 \
uint32_t freq_MSB, freq_LSB;                                \
uint32_t increment1, increment2;                            \
  freq = _min(0xFA00000, freq); /*20KHz max*/               \
  freq = _max(0, freq);                                     \
  freq_MSB = freq >> 18; /* keep the 1st 10 bits */         \
  freq_LSB = freq & 0x3FFFF; /* other 18  bits */           \
  increment1 = table_CV2increment[freq_MSB];                \
  increment2 = table_CV2increment[freq_MSB+1];              \
  increment2 -= increment1;                                 \
  increment1 += ((increment2>>8)*(freq_LSB>>2))>>8;         

# define macro_fq2increment_novar                           \
  freq = _min(0xFA00000, freq); /*20KHz max*/               \
  freq = _max(0, freq);                                     \
  freq_MSB = freq >> 18; /* keep the 1st 10 bits */         \
  freq_LSB = freq & 0x3FFFF; /* other 18  bits */           \
  increment1 = table_CV2increment[freq_MSB];                \
  increment2 = table_CV2increment[freq_MSB+1];              \
  increment2 -= increment1;                                 \
  increment1 += ((increment2>>8)*(freq_LSB>>2))>>8;         

# define macro_fq2cutoff                                    \
uint32_t freq_MSB, freq_LSB;                                \
uint32_t increment1, increment2;                            \
  freq = _min(0xFA00000, freq); /*20KHz max*/               \
  freq = _max(0, freq);                                     \
  freq_MSB = freq >> 18; /* keep the 1st 10 bits */         \
  freq_LSB = freq & 0x3FFFF; /* other 18  bits */           \
  increment1 = table_CV2cutof[freq_MSB];                    \
  increment2 = table_CV2cutof[freq_MSB+1];                  \
  increment2 -= increment1;                                 \
  increment1 += ((increment2>>8)*(freq_LSB>>2))>>8;         

# define macro_fq2cutoff_novar                              \
  freq = _min(0xFA00000, freq); /*20KHz max*/               \
  freq = _max(0, freq);                                     \
  freq_MSB = freq >> 18; /* keep the 1st 10 bits */         \
  freq_LSB = freq & 0x3FFFF; /* other 18  bits */           \
  increment1 = table_CV2cutof[freq_MSB];                    \
  increment2 = table_CV2cutof[freq_MSB+1];                  \
  increment2 -= increment1;                                 \
  increment1 += ((increment2>>8)*(freq_LSB>>2))>>8;         

// ---------------------------------------------------------------------

# define macro_out_gain_pan                                           \
uint32_t macro_panR, macro_panL, macro_outL, macro_outR;              \
int32_t macro_outS, macro_tmp, macro_tmp2, macro_tmp3;                \
  if (IN1_connect < 60) {                                             \
    macro_outS = (audio_inL>>1) - (IN1_0V>>1);                        \
    macro_outS = _min(0x2AAAAAAA,macro_outS);                         \
    macro_outS += macro_outS>>1;                                      \
     macro_outS = _max(0,macro_outS);                                 \
    _m_u32xu32_u32(macro_outS, macro_outS, macro_outS, macro_tmp2);   \
    _m_s32xs32_s32(out^0x80000000, macro_outS, out, macro_tmp2);      \
    out <<= 4;                                                        \
    out ^= 0x80000000;                                                \
  }                                                                   \
  if (IN2_connect < 60) {                                             \
    out ^= 0x80000000;                                                \
    macro_panR = audio_inR;                                           \
    macro_panR = max(macro_panR, 0x3FFFFFC0);                         \
    macro_panR = min(macro_panR - 0x3FFFFFC0, 0x7FFFFF80);            \
    macro_panR <<= 1;                                                 \
    macro_panL = macro_panR;                                          \
    macro_panL = fast_sin((macro_panL>>2) + 0x40000000);              \
    macro_panL -= 0x7FFFFFFF;                                         \
    macro_panR = fast_sin(macro_panR>>2);                             \
    macro_panR -= 0x7FFFFFFF;                                         \
    _m_s32xs32_s32(macro_panL, out, macro_outL, macro_tmp);           \
    _m_s32xs32_s32(macro_panR, out, macro_outR, macro_tmp);           \
    audio_outR = (macro_outR<<1)^0x80000000;                          \
    audio_outL = (macro_outL<<1)^0x80000000;                          \
  }                                                                   \
  else {                                                              \
    macro_out                                                         \
  }

# define macro_out_gain                                               \
uint32_t macro_panR, macro_panL, macro_outL, macro_outR;              \
int32_t macro_outS, macro_tmp, macro_tmp2, macro_tmp3;                \
  if (IN1_connect < 60) {                                             \
    macro_outS = (audio_inL>>1) - (IN1_0V>>1);                        \
    macro_outS = _min(0x2AAAAAAA,macro_outS);                         \
    macro_outS += macro_outS>>1;                                      \
     macro_outS = _max(0,macro_outS);                                 \
    _m_u32xu32_u32(macro_outS, macro_outS, macro_outS, macro_tmp2);   \
    _m_s32xs32_s32(out^0x80000000, macro_outS, out, macro_tmp2);      \
    out <<= 4;                                                        \
    out ^= 0x80000000;                                                \
  }                                                                   \
  macro_out                                                         


  # define macro_out_gainL_stereo                                      \
uint32_t macro_panR, macro_panL, macro_outL, macro_outR;              \
int32_t macro_outS, macro_tmp, macro_tmp2, macro_tmp3;                \
  if (IN1_connect < 60) {                                             \
    macro_outS = (audio_inL>>1) - (IN1_0V>>1);                        \
    macro_outS = _min(0x2AAAAAAA,macro_outS);                         \
    macro_outS += macro_outS>>1;                                      \
    macro_outS = _max(0,macro_outS);                                  \
    _m_u32xu32_u32(macro_outS, macro_outS, macro_outS, macro_tmp2);   \
    _m_s32xs32_s32(out^0x80000000, macro_outS, out, macro_tmp2);      \
    out <<= 4;                                                        \
    out ^= 0x80000000;                                                \
    _m_s32xs32_s32(out2^0x80000000, macro_outS, out2, macro_tmp2);    \
    out2 <<= 4;                                                       \
    out2 ^= 0x80000000;                                               \
  }                                                                   \
  macro_out_stereo

  # define macro_out_gainR_stereo                                      \
uint32_t macro_panR, macro_panL, macro_outL, macro_outR;              \
int32_t macro_outS, macro_tmp, macro_tmp2, macro_tmp3;                \
  if (IN2_connect < 60) {                                             \
    macro_outS = (audio_inR>>1) - (IN2_0V>>1);                        \
    macro_outS = _min(0x2AAAAAAA,macro_outS);                         \
    macro_outS += macro_outS>>1;                                      \
    macro_outS = _max(0,macro_outS);                                  \
    _m_u32xu32_u32(macro_outS, macro_outS, macro_outS, macro_tmp2);   \
    _m_s32xs32_s32(out^0x80000000, macro_outS, out, macro_tmp2);      \
    out <<= 4;                                                        \
    out ^= 0x80000000;                                                \
    _m_s32xs32_s32(out2^0x80000000, macro_outS, out2, macro_tmp2);    \
    out2 <<= 4;                                                       \
    out2 ^= 0x80000000;                                               \
  }                                                                   \
  macro_out_stereo

  
# define macro_out_gain_pan_stereo                                    \
uint32_t macro_panR, macro_panL, macro_outL, macro_outR;              \
int32_t macro_outS, macro_tmp, macro_tmp2, macro_tmp3;                \
  if (IN1_connect < 60) {                                             \
    macro_outS = (audio_inL>>1) - (IN1_0V>>1);                        \
    macro_outS = _min(0x2AAAAAAA,macro_outS);                         \
    macro_outS += macro_outS>>1;                                      \
    macro_outS = _max(0,macro_outS);                                  \
    _m_u32xu32_u32(macro_outS, macro_outS, macro_outS, macro_tmp2);   \
    _m_s32xs32_s32(out^0x80000000, macro_outS, out, macro_tmp2);      \
    out <<= 4;                                                        \
    out ^= 0x80000000;                                                \
    _m_s32xs32_s32(out2^0x80000000, macro_outS, out2, macro_tmp2);    \
    out2 <<= 4;                                                       \
    out2 ^= 0x80000000;                                               \
  }                                                                   \
  if (IN2_connect < 60) {                                             \
    out ^= 0x80000000;                                                \
    macro_panR = audio_inR;                                           \
    macro_panR = max(macro_panR, 0x3FFFFFC0);                         \
    macro_panR = min(macro_panR - 0x3FFFFFC0, 0x7FFFFF80);            \
    macro_panR <<= 1;                                                 \
    macro_panL = macro_panR;                                          \
    macro_panL = fast_sin((macro_panL>>2) + 0x40000000);              \
    macro_panL -= 0x7FFFFFFF;                                         \
    macro_panR = fast_sin(macro_panR>>2);                             \
    macro_panR -= 0x7FFFFFFF;                                         \
    _m_s32xs32_s32(macro_panL, out, macro_outL, macro_tmp);           \
    _m_s32xs32_s32(macro_panR, out, macro_outR, macro_tmp);           \
    audio_outR = (macro_outR<<1)^0x80000000;                          \
    audio_outL = (macro_outL<<1)^0x80000000;                          \
  }                                                                   \
  else {                                                              \
    macro_out_stereo                                                  \
  }

# define macro_out_gain_pan_stereo2                                    \
uint32_t macro_panR, macro_panL, macro_outL, macro_outR;              \
int32_t macro_outS, macro_tmp, macro_tmp2, macro_tmp3;                \
  if (IN1_connect < 60) {                                             \
    macro_outS = (audio_inL>>1) - (IN1_0V>>1);                        \
    macro_outS = _min(0x2AAAAAAA,macro_outS);                         \
    macro_outS += macro_outS>>1;                                      \
    macro_outS = _max(0,macro_outS);                                  \
    _m_u32xu32_u32(macro_outS, macro_outS, macro_outS, macro_tmp2);   \
    _m_s32xs32_s32(out^0x80000000, macro_outS, out, macro_tmp2);      \
    out <<= 4;                                                        \
    out ^= 0x80000000;                                                \
    _m_s32xs32_s32(out2^0x80000000, macro_outS, out2, macro_tmp2);    \
    out2 <<= 4;                                                       \
    out2 ^= 0x80000000;                                               \
  }                                                                   \
  if (IN2_connect < 60) {                                             \
    out ^= 0x80000000;                                                \
    out2 ^= 0x80000000;                                               \
    macro_panR = audio_inR;                                           \
    macro_panR = max(macro_panR, 0x3FFFFFC0);                         \
    macro_panR = min(macro_panR - 0x3FFFFFC0, 0x7FFFFF80);            \
    macro_panR <<= 1;                                                 \
    macro_panL = macro_panR;                                          \
    macro_panL = fast_sin((macro_panL>>2) + 0x40000000);              \
    macro_panL -= 0x7FFFFFFF;                                         \
    macro_panR = fast_sin(macro_panR>>2);                             \
    macro_panR -= 0x7FFFFFFF;                                         \
    _m_s32xs32_s32(macro_panL, out, macro_outL, macro_tmp);           \
    _m_s32xs32_s32(macro_panR, out2, macro_outR, macro_tmp);           \ 
    audio_outR = (macro_outR<<1)^0x80000000;                          \
    audio_outL = (macro_outL<<1)^0x80000000;                          \
  }                                                                   \
  else {                                                              \
    macro_out_stereo                                                  \
  }

#define macro_out_pan                                                 \  
uint32_t macro_panR, macro_panL, macro_outL, macro_outR, macro_tmp;   \
  if (IN2_connect < 60) {                                             \
    out ^= 0x80000000;                                                \
    macro_panR = audio_inR;                                           \
    macro_panR = max(macro_panR, 0x3FFFFFC0);                         \
    macro_panR = min(macro_panR - 0x3FFFFFC0, 0x7FFFFF80);            \
    macro_panR <<= 1;                                                 \
    macro_panL = macro_panR;                                          \
    macro_panL = fast_sin((macro_panL>>2) + 0x40000000);              \
    macro_panL -= 0x7FFFFFFF;                                         \
    macro_panR = fast_sin(macro_panR>>2);                             \
    macro_panR -= 0x7FFFFFFF;                                         \
    _m_s32xs32_s32(macro_panL, out, macro_outL, macro_tmp);           \
    _m_s32xs32_s32(macro_panR, out, macro_outR, macro_tmp);           \
    audio_outR = (macro_outR<<1)^0x80000000;                          \
    audio_outL = (macro_outL<<1)^0x80000000;                          \
  }                                                                   \
  else {                                                              \
    macro_out                                                         \
  }

#define macro_out_pan_stereo                                          \  
uint32_t macro_panR, macro_panL, macro_outL, macro_outR, macro_tmp;   \
  if (IN2_connect < 60) {                                             \
    out ^= 0x80000000;                                                \
    macro_panR = audio_inR;                                           \
    macro_panR = max(macro_panR, 0x3FFFFFC0);                         \
    macro_panR = min(macro_panR - 0x3FFFFFC0, 0x7FFFFF80);            \
    macro_panR <<= 1;                                                 \
    macro_panL = macro_panR;                                          \
    macro_panL = fast_sin((macro_panL>>2) + 0x40000000);              \
    macro_panL -= 0x7FFFFFFF;                                         \
    macro_panR = fast_sin(macro_panR>>2);                             \
    macro_panR -= 0x7FFFFFFF;                                         \
    _m_s32xs32_s32(macro_panL, out, macro_outL, macro_tmp);           \
    _m_s32xs32_s32(macro_panR, out, macro_outR, macro_tmp);           \
    audio_outR = (macro_outR<<1)^0x80000000;                          \
    audio_outL = (macro_outL<<1)^0x80000000;                          \
  }                                                                   \
  else {                                                              \
    macro_out_stereo                                                  \
  }


# define macro_out_pan_novar                                          \
  if (IN2_connect < 60) {                                             \
    out ^= 0x80000000;                                                \
    macro_panR = audio_inR;                                           \
    macro_panR = max(macro_panR, 0x3FFFFFC0);                         \
    macro_panR = min(macro_panR - 0x3FFFFFC0, 0x7FFFFF80);            \
    macro_panR <<= 1;                                                 \
    macro_panL = macro_panR;                                          \
    macro_panL = fast_sin((macro_panL>>2) + 0x40000000);              \
    macro_panL -= 0x7FFFFFFF;                                         \
    macro_panR = fast_sin(macro_panR>>2);                             \
    macro_panR -= 0x7FFFFFFF;                                         \
    _m_s32xs32_s32(macro_panL, out, macro_outL, macro_tmp);           \
    _m_s32xs32_s32(macro_panR, out, macro_outR, macro_tmp);           \
    audio_outR = (macro_outR<<1)^0x80000000;                          \
    audio_outL = (macro_outL<<1)^0x80000000;                          \
  }                                                                   \
  else {                                                              \
    macro_out                                                         \
  }


# define macro_out                                                    \
    audio_outL = out;                                                 \
    audio_outR = out;

# define macro_out_stereo                                             \
    audio_outL = out;                                                 \
    audio_outR = out2;

#define filter16_var                  \
  uint32_t input, out, j, index;      \
  int32_t value_min, value_max, diff;

/*
//#define filter_in_16b(i,REG_pot, filter)                  \
  if (REG_ADC_ISR & (1<<table_adc[i])) {                    \
    index = filter_index[i];                                \
    index = (index >= filter_order-1)? 0:index+1;           \
    filter_index[i] = index;                                \
    input = REG_pot<<4;                                     \
    out = CV_filter16_out[i];                               \
    diff = input - out;                                     \
    CV_filter_moins[i][index] = _min(0, diff);              \
    CV_filter_plus[i][index]  = _max(0, diff);              \
    value_min = CV_filter_moins[i][0];                      \
    value_max = CV_filter_plus[i][0];                       \
    for (j=1; j < filter_order; j++) {                      \
      value_min = _max(value_min, CV_filter_moins[i][j]);   \
      value_max = _min(value_max, CV_filter_plus[i][j]);    \
    }                                                       \
    diff = (value_max > -value_min)? value_max: value_min;  \
    out += diff>>filter;                                    \
    CV_filter16_out[i] = out;                               \
  }
*/

#define filter_in_16b(i,REG_pot)                            \
  if (REG_ADC_ISR & (1<<table_adc[i])) {                    \
    index = filter_index[i];                                \
    index = (index >= filter_order-1)? 0:index+1;           \
    filter_index[i] = index;                                \
    input = REG_pot<<4;                                     \
    out = CV_filter16[i];                                   \
    diff = input - out;                                     \
    CV_filter_moins[i][index] = _min(0, diff);              \
    CV_filter_plus[i][index]  = _max(0, diff);              \
    value_min = CV_filter_moins[i][0];                      \
    value_max = CV_filter_plus[i][0];                       \
    for (j=1; j < filter_order; j++) {                      \
      value_min = _max(value_min, CV_filter_moins[i][j]);   \
      value_max = _min(value_max, CV_filter_plus[i][j]);    \
    }                                                       \
    if (value_max > -value_min) {                           \
      diff = (value_max>=64)? value_max-60:value_max>>4;    \
    } else {                                                \
      diff = (value_min<=-64)? value_min+60:value_min>>4;   \
    }                                                       \
    out += diff;                                            \
    CV_filter16[i] = out;                                   \
    CV_filter16_out[i] = max(out, 100) - 100;               \
  } 

#define filter16_pot1() filter_in_16b(0, REG_pot1)
#define filter16_pot2() filter_in_16b(1, REG_pot2)
#define filter16_pot3() filter_in_16b(2, REG_pot3)
#define filter16_pot4() filter_in_16b(3, REG_pot4)
#define filter16_pot5() filter_in_16b(4, REG_pot5)
#define filter16_pot6() filter_in_16b(5, REG_pot6)
#define filter16_pot7() filter_in_16b(6, REG_pot7)
#define filter16_pot8() filter_in_16b(7, REG_pot8)
#define filter16_cv1()  filter_in_16b(8, REG_CV1)
#define filter16_cv2()  filter_in_16b(9, REG_CV2)
#define filter16_cv3()  filter_in_16b(10,REG_CV3)
#define filter16_cv4()  filter_in_16b(11,REG_CV4)

#define filter16_nozori_68            \
  uint32_t input, out, j, index;      \
  int32_t value_min, value_max, diff; \
  filter16_pot1();                    \
  filter16_pot2();                    \
  filter16_pot3();                    \
  filter16_pot4();                    \
  filter16_pot5();                    \
  filter16_pot6();                    \
  filter16_cv1();                     \
  filter16_cv2();                     \
  filter16_cv3();                     \
  filter16_cv4();

#define filter16_nozori_84            \
  uint32_t input, out, j, index;      \
  int32_t value_min, value_max, diff; \
  filter16_pot1();                    \
  filter16_pot2();                    \
  filter16_pot3();                    \
  filter16_pot4();                    \
  filter16_pot5();                    \
  filter16_pot6();                    \
  filter16_pot7();                    \
  filter16_pot8();                    \

#define filter16_nozori               \
  uint32_t input, out, j, index;      \
  int32_t value_min, value_max, diff; \
  filter16_pot1();                    \
  filter16_pot2();                    \
  filter16_pot3();                    \
  filter16_pot4();                    \
  filter16_pot5();                    \
  filter16_pot6();                    \
  filter16_pot7();                    \
  filter16_pot8();                    \
  filter16_cv1();                     \
  filter16_cv2();                     \
  filter16_cv3();                     \
  filter16_cv4();



#define test_connect_loop_start                                                                   \
  uint32_t test_loop_value, test_loop_next_value;                                                 \
  if (connect_timer > 0x30) {                                                                     \
    connect_timer = 0;                                                                            \
    test_loop_value = connect_test_value;                                                         \
    test_loop_next_value = random32() & 1;

#define test_connect_loop_test_cv1                                                                \
    if ((REG_CV1 > 0b101000000000) == (test_loop_value > 0)) CV1_connect++; else CV1_connect = 0; \
    CV1_connect = _min(CV1_connect,0xFFFFFFF0);                                                   \
    default1(test_loop_next_value);

#define test_connect_loop_test_cv2                                                                \
    if ((REG_CV2 > 0b101000000000) == (test_loop_value > 0)) CV2_connect++; else CV2_connect = 0; \
    CV2_connect = _min(CV2_connect,0xFFFFFFF0);                                                   \
    default2(test_loop_next_value);

#define test_connect_loop_test_cv3                                                                \
    if ((REG_CV3 > 0b101000000000) == (test_loop_value > 0)) CV3_connect++; else CV3_connect = 0; \
    CV3_connect = _min(CV3_connect,0xFFFFFFF0);                                                   \
    default3(test_loop_next_value);

#define test_connect_loop_test_cv4                                                                \
    if ((REG_CV4 > 0b101000000000) == (test_loop_value > 0)) CV4_connect++; else CV4_connect = 0; \
    CV4_connect = _min(CV4_connect,0xFFFFFFF0);                                                   \
    default4(test_loop_next_value);

#define test_connect_loop_test_in1                                                                \
    if ((audio_inL > 0x98000000) == (test_loop_value > 0)) IN1_connect++; else IN1_connect = 0;   \
    IN1_connect = _min(IN1_connect,0xFFFFFFF0);                                                   \
    default5(test_loop_next_value);

#define test_connect_loop_test_in2                                                                \
    if ((audio_inR > 0x98000000) == (test_loop_value > 0)) IN2_connect++; else IN2_connect = 0;   \
    IN2_connect = _min(IN2_connect,0xFFFFFFF0);                                                   \
    default6(test_loop_next_value);

#define test_connect_loop_test_end                                                                \
    connect_test_value = test_loop_next_value;                                                    \
  }

