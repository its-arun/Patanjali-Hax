//
//  firegameevent.cpp
//  Patanjali
//
//  Created by Arun Chaudhary on 07/03/18.
//  Copyright © 2018 felli0t. All rights reserved.
//

#include "main.hpp"
#include "skinchanger.h"

bool hkFireEventClientSide(void* thisptr, IGameEvent* event){
    
    if(event){
        skinchanger->FireEventClientSide(event);
    }
    
    return game_event_vmt->GetOriginalMethod< tFireGameEvent >(11)(thisptr, event);
}
