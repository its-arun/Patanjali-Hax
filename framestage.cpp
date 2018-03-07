#include "main.hpp"
#include "skinchanger.h"

bool bOnce = false;


void hkFrameStage(void* thisptr, ClientFrameStage_t curStage) {
    skinchanger->FrameStageNotify(curStage);

    client_vmt->GetOriginalMethod<tFrameStage>(36)(thisptr, curStage);
}
