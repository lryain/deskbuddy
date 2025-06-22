#include "aec_msu_pub.h"

void AecMsuSetDefaultConfig(AecMsuConfig_t* pConfig,
                            int32 blockSize,
                            int32 lenChanModel,
                            int32 sampleRate_Hz)
{
    // stub: do nothing or set default values
    if (pConfig) {
        pConfig->blockSize = blockSize;
        pConfig->lenChanModel = lenChanModel;
        pConfig->sampleRate_Hz = sampleRate_Hz;
        // 其它字段可设为0或默认值
    }
}