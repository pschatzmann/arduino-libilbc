#pragma once

#include <stdint.h>

extern "C" {
  #include "ilbc_src/iLBC_decode.h"
  #include "ilbc_src/iLBC_define.h"
  #include "ilbc_src/iLBC_encode.h"
}

enum EnumLBCFrameSize { ms20 = 20, ms30 = 30 };
enum EnumLBCDecodeMode { MODE_BAD = 0, MODE_NORMAL = 1 };

/**
 * @brief Internet Low Bitrate Codec (iLBC) Encoder which is expecting int16_t samples
 * @author Phil Schatzmann
 */
class iLBCEncode {
public:
  iLBCEncode(EnumLBCFrameSize frameSize = ms30) {
    this->frameSize = frameSize;
    initEncode(&g_enc_inst, frameSize);
  }
  ~iLBCEncode(){
    if (block!=nullptr) delete block;
  }

  /// Number of decoded samples
  int getSamples() { return g_enc_inst.blockl; }
  /// number of encoded bytes
  int getEncodedBytes() { return g_enc_inst.no_of_bytes; }

  /// Encodes framesize of int16_t samples 
  int encode(int16_t *samples, uint8_t *data) {
    if (block==nullptr){
      block = new float[getSamples()];
    }
    // Convert to float representaion of voice signal. Returns number of bytes
    for (int i = 0; i < getSamples(); i++) {
      block[i] = samples[i];
    }

    iLBC_encode(data, block, &g_enc_inst);
    return g_enc_inst.no_of_bytes;
  }

  /// Encodes framesize of float samples 
  int encode(float *samples, uint8_t *data) {
    iLBC_encode(data, samples, &g_enc_inst);
    return g_enc_inst.no_of_bytes;
  }

protected:
  iLBC_Enc_Inst_t g_enc_inst;
  EnumLBCFrameSize frameSize;
  float *block=nullptr;
};

/**
 * @brief Internet Low Bitrate Codec (iLBC) Decoder which is expecting int16_t samples
 * @author Phil Schatzmann
 */
class iLBCDecode {
public:
  iLBCDecode(EnumLBCFrameSize frameSize = ms30, bool useEnhancer = true) {
    this->frameSize = frameSize;
    initDecode(&g_dec_inst, frameSize, useEnhancer);
  }

  ~iLBCDecode(){
    if (block!=nullptr) delete block;
  }

  /// Number of decoded samples
  int getSamples() { return g_dec_inst.blockl; }
  /// number of encoded bytes
  int getEncodedBytes() { return g_dec_inst.no_of_bytes; }


  /// Decodes framesize of int16_t samples - returns number of bytes
  int decode(uint8_t *data, int16_t *samples, EnumLBCDecodeMode mode=MODE_NORMAL) {
    if (block==nullptr){
      block = new float[getSamples()];
    }

    iLBC_decode(block, data, &g_dec_inst, (int)mode);

    // convert to PCM16
    float point = 0.0;
    for (int i = 0; i < getSamples(); i++) {
      point = block[i];
      if (point < MIN_SAMPLE) {
        point = MIN_SAMPLE;
      } else if (point > MAX_SAMPLE) {
        point = MAX_SAMPLE;
      }
      samples[i] = point;
    }

    return g_dec_inst.blockl * sizeof(int16_t);
  }

  /// Decodes framesize of float samples
  int decode(uint8_t *data,float *samples, EnumLBCDecodeMode mode=MODE_NORMAL) {
    iLBC_decode(samples, data, &g_dec_inst, (int)mode);
    return g_dec_inst.blockl * sizeof(float);
  }

protected:
  iLBC_Dec_Inst_t g_dec_inst;
  EnumLBCFrameSize frameSize;
  float *block=nullptr;
};
