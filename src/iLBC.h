#pragma once

#include <math.h>
#include <string.h>

extern "C" {
  #include "ilbc_src/iLBC_decode.h"
  #include "ilbc_src/iLBC_define.h"
  #include "ilbc_src/iLBC_encode.h"
}

enum EnumLBCFrameSize { ms20 = 20, ms30 = 30 };

/**
 * @brief Internet Low Bitrate Codec (iLBC)
 *
 */
class iLBC {
public:
  iLBC(EnumLBCFrameSize frameSize = ms30, bool useEnhancer = true) {
    this->frameSize = frameSize;
    block = new float[frameSize * 80];
    initEncode(&g_enc_inst, frameSize);
    initDecode(&g_dec_inst, frameSize, useEnhancer);
  }

  /// Number of decoded samples
  int getSamples() { return this->frameSize * 80; }
  /// number of encoded bytes
  int getEncodedBytes() { return g_enc_inst.no_of_bytes; }

  /// Encodes framesize of samples
  int encode(int16_t *samples, unsigned char *data) {
    int i;

    // Convert to float representaion of voice signal.
    for (i = 0; i < g_enc_inst.blockl; i++) {
      block[i] = samples[i];
    }

    iLBC_encode(data, block, &g_enc_inst);

    return g_enc_inst.no_of_bytes;
  }

  /// Decodes framesize of samples
  int decode(unsigned char *data, int16_t *samples, int mode) {
    int i;
    float block[BLOCKL_MAX];

    // Validate Mode
    if (mode != 0 && mode != 1) {
      return -1;
    }

    iLBC_decode(block, data, &g_dec_inst, mode);

    // Validate PCM16
    for (i = 0; i < g_dec_inst.blockl; i++) {
      float point;

      point = block[i];
      if (point < MIN_SAMPLE) {
        point = MIN_SAMPLE;
      } else if (point > MAX_SAMPLE) {
        point = MAX_SAMPLE;
      }

      samples[i] = point;
    }

    return g_dec_inst.blockl * 2;
  }

protected:
  iLBC_Enc_Inst_t g_enc_inst;
  iLBC_Dec_Inst_t g_dec_inst;
  EnumLBCFrameSize frameSize;
  float *block=nullptr;
};