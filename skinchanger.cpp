//
//  skinchanger.cpp
//  Patanjali
//
//  Created by Arun Chaudhary on 07/03/18.
//  Copyright © 2018 felli0t. All rights reserved.
//

#include "skinchanger.h"

int KnifeCT = WEAPON_KNIFE_KARAMBIT; //Change knife model if you make changes below
int KnifeT = WEAPON_KNIFE_BUTTERFLY;

std::unordered_map<int, cSkin> cSkinchanger::SkinList = std::unordered_map<int, cSkin>( {
    /*To add more skins add std::make_pair( WEAPON_NAME, cSkin(PaintkitID, SEED, ItemDefinition, StatTrack, CustomName, Float))
     Seed = PaintkitID or 0 ; Use -1 as ItemDefinition for anything other than Knife ; For knife put ItemDefinition as WEAPON_KNIFE_KNIFETYPE*/
    std::make_pair( WEAPON_AK47, cSkin( 675, 675, -1, -1, ( char* )( "Allahu Akbar" ), 0.0001f ) ),
    std::make_pair( WEAPON_KNIFE, cSkin( 419, 419, WEAPON_KNIFE_KARAMBIT, -1, ( char* )( "Karma" ), 0.0001f )),
    std::make_pair( WEAPON_AWP, cSkin(344, 344, -1, -1, nullptr, 0.0001f)),
    std::make_pair( WEAPON_M4A1, cSkin(309, 309, -1, -1, nullptr, 0.0001f)),
    std::make_pair( WEAPON_GLOCK, cSkin(586, 586, -1, -1, nullptr, 0.0001f)),
    std::make_pair( WEAPON_FIVESEVEN, cSkin(427, 427, -1, -1, nullptr, 0.0001f)),
    std::make_pair( WEAPON_USP_SILENCER, cSkin(504, 504, -1, -1, nullptr, 0.0001f)),
    std::make_pair( WEAPON_P90, cSkin(156, 156, -1, -1, nullptr, 0.0001f)),
    std::make_pair( WEAPON_UMP45, cSkin(556, 556, -1, -1, nullptr, 0.0001f)),
    std::make_pair( WEAPON_DEAGLE, cSkin(185, 185, -1, -1, nullptr, 0.0001f)),
    std::make_pair( WEAPON_P250, cSkin(551, 551, -1, -1, nullptr, 0.0001f)),
    std::make_pair( WEAPON_MAC10, cSkin(433, 433, -1, -1, nullptr, 0.0001f)),
    std::make_pair( WEAPON_KNIFE_T, cSkin(619, 619, WEAPON_KNIFE_BUTTERFLY, -1, nullptr, 0.0001f))
} );

std::unordered_map<int, const char*> cSkinchanger::ModelList;

cSkinchanger* skinchanger = new cSkinchanger;

void cSkinchanger::FrameStageNotify(ClientFrameStage_t stage) {
    if(stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START){
        pLocalPlayer = (C_BaseEntity*)(pEntList->GetClientEntity(pEngine->GetLocalPlayer()));
        
        if(pLocalPlayer && pLocalPlayer->GetHealth() > 0){
            if(!bInit){
                Init();
                bInit = true;
            }
            
            ForceSkins();
        }
    }
}

void cSkinchanger::FindModels() {
    ModelList[pModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl")] = KnifeToModelMatrix[KnifeCT].c_str();
    ModelList[pModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl")] = KnifeToModelMatrix[KnifeT].c_str();
}

void cSkinchanger::ForceSkins() {
    IEngineClient::player_info_t player_info;
    
    if(pEngine->GetPlayerInfo(pLocalPlayer->GetId(), &player_info)){
        
        int* pWeapons = pLocalPlayer->GetWeapons();
        
        C_BaseViewModel* LocalPlayerViewModel = (C_BaseViewModel*)pEntList->GetClientEntityFromHandle(pLocalPlayer->GetViewModel());
        
        C_BaseAttributableItem* WeaponViewModel = nullptr;
        
        if(LocalPlayerViewModel)
            WeaponViewModel = (C_BaseAttributableItem*)pEntList->GetClientEntityFromHandle(LocalPlayerViewModel->GetWeapon());
        
        C_BaseCombatWeapon* localWeapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(pLocalPlayer->GetActiveWeapon());
        
        if(pWeapons){
            for(int i = 0; pWeapons[i]; i++){
                C_BaseAttributableItem* attributableItem = (C_BaseAttributableItem*)pEntList->GetClientEntityFromHandle(pWeapons[i]);
                
                if(attributableItem){
                    int* Definition = attributableItem->GetItemDefinitionIndex();
                    
                    //std::unordered_map<int, cSkin>::iterator SkinIter = (*Definition == WEAPON_KNIFE_T ? SkinList.find(WEAPON_KNIFE) : SkinList.find(*Definition));
                    unordered_map<int, cSkin>::iterator SkinIter = (*Definition == WEAPON_KNIFE ? (*Definition == WEAPON_KNIFE ? SkinList.find(WEAPON_KNIFE) : SkinList.find(WEAPON_KNIFE_T)) : SkinList.find(*Definition));
                    
                    if(SkinIter != SkinList.end()) {
                        if(*attributableItem->GetOriginalOwnerXuidLow() == player_info.xuidlow && *attributableItem->GetOriginalOwnerXuidHigh() == player_info.xuidhigh){
                            
                            int* model_index = attributableItem->GetModelIndex();
                            
                            std::unordered_map<int, const char*>::iterator model_iter = ModelList.find(*model_index);
                            
                            if(model_iter != ModelList.end()){
                                *model_index = pModelInfo->GetModelIndex(model_iter->second);
                            }
                            
                            cSkin skin = std::move(SkinIter->second);
                            
                            if(KnifeCT && (*Definition == WEAPON_KNIFE))
                                *attributableItem->GetItemDefinitionIndex() = KnifeCT;
                            else if(KnifeT && (*Definition == WEAPON_KNIFE_T))
                                *attributableItem->GetItemDefinitionIndex() = KnifeT;
                            
                            
                            if(skin.name){
                                sprintf(attributableItem->GetCustomName(), skin.name);
                            }
                            
                            *attributableItem->GetItemIDHigh() = -1;
                            *attributableItem->GetFallbackPaintKit() = skin.Paintkit;
                            *attributableItem->GetFallbackStatTrak() = skin.StatTrack;
                            *attributableItem->GetEntityQuality() = 4;
                            *attributableItem->GetFallbackSeed() = skin.Seed;
                            *attributableItem->GetFallbackWear() = skin.Wear;
                            *attributableItem->GetAccountID() = player_info.xuidlow;
                        }
                    }
                    
                    if (WeaponViewModel && WeaponViewModel == attributableItem)
                    {
                        int* model_index = ((C_BaseEntity*)LocalPlayerViewModel)->GetModelIndex();
                        
                        std::unordered_map<int, const char*>::iterator model_iter = ModelList.find(*model_index);
                        
                        if (model_iter != ModelList.end())
                        {
                            *model_index = pModelInfo->GetModelIndex(model_iter->second);
                        }
                    }
                    
                }
            }
            
            if(LocalPlayerViewModel && localWeapon){
                
                int* model_index = ((C_BaseEntity*)LocalPlayerViewModel)->GetModelIndex();
                
                std::unordered_map<int, const char*>::iterator model_iter = ModelList.find(*((C_BaseEntity*)localWeapon)->GetModelIndex());
                
                if(model_iter != ModelList.end()){
                    *model_index = pModelInfo->GetModelIndex(model_iter->second);
                }
            }
        }
    }
}

void cSkinchanger::Init() {
    ModelList.clear();
    FindModels();
}

void cSkinchanger::FireEventClientSide(IGameEvent *event) {
    
    if (!strcmp(event->GetName(), "game_newmap"))
    {
        Init();
    }
}

// Fix Animations

inline const int RandomSequence(int low, int high) {
    return (rand() % (high - low + 1) + low);
}

void HSequenceProxyFn(const CRecvProxyData *pDataConst, void *pStruct, void *pOut) {
    
    CRecvProxyData* pData = const_cast<CRecvProxyData*>(pDataConst);
    C_BaseViewModel* pViewModel = (C_BaseViewModel*)pStruct;
    
    if(!pViewModel)
        return g_pSequence(pDataConst, pStruct, pOut);
    
    C_BaseEntity* pOwner = (C_BaseEntity*)pEntList->GetClientEntityFromHandle(pViewModel->GetOwner());
    
    if (pViewModel && pOwner) {
        if (pOwner->GetIndex() == pEngine->GetLocalPlayer()) {
            
            const model_t* knife_model = pModelInfo->GetModel(*pViewModel->GetModelIndex());
            const char* model_filename = pModelInfo->GetModelName(knife_model);
            
            int m_nSequence = (int)pData->m_Value.m_Int;
            
            if (!strcmp(model_filename, "models/weapons/v_knife_butterfly.mdl")) {
                
                switch (m_nSequence) {
                    case SEQUENCE_DEFAULT_DRAW:
                        m_nSequence = RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
                        break;
                    case SEQUENCE_DEFAULT_LOOKAT01:
                        m_nSequence = RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
                        break;
                    default:
                        m_nSequence++;
                }
                
            } else if (!strcmp(model_filename, "models/weapons/v_knife_falchion_advanced.mdl")) {
                
                switch (m_nSequence) {
                    case SEQUENCE_DEFAULT_IDLE2:
                        m_nSequence = SEQUENCE_FALCHION_IDLE1;
                        break;
                    case SEQUENCE_DEFAULT_HEAVY_MISS1:
                        m_nSequence = RandomSequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
                        break;
                    case SEQUENCE_DEFAULT_LOOKAT01:
                        m_nSequence = RandomSequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
                        break;
                    case SEQUENCE_DEFAULT_DRAW:
                    case SEQUENCE_DEFAULT_IDLE1:
                        break;
                    default:
                        m_nSequence--;
                }
                
            } else if (!strcmp(model_filename, "models/weapons/v_knife_push.mdl")) {
                
                switch (m_nSequence) {
                    case SEQUENCE_DEFAULT_IDLE2:
                        m_nSequence = SEQUENCE_DAGGERS_IDLE1;
                        break;
                    case SEQUENCE_DEFAULT_LIGHT_MISS1:
                    case SEQUENCE_DEFAULT_LIGHT_MISS2:
                        m_nSequence = RandomSequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
                        break;
                    case SEQUENCE_DEFAULT_HEAVY_MISS1:
                        m_nSequence = RandomSequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
                        break;
                    case SEQUENCE_DEFAULT_HEAVY_HIT1:
                    case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
                    case SEQUENCE_DEFAULT_LOOKAT01:
                        m_nSequence += 3;
                        break;
                    case SEQUENCE_DEFAULT_DRAW:
                    case SEQUENCE_DEFAULT_IDLE1:
                        break;
                    default:
                        m_nSequence += 2;
                }
                
            } else if (!strcmp(model_filename, "models/weapons/v_knife_survival_bowie.mdl")) {
                
                switch (m_nSequence) {
                    case SEQUENCE_DEFAULT_DRAW:
                    case SEQUENCE_DEFAULT_IDLE1:
                        break;
                    case SEQUENCE_DEFAULT_IDLE2:
                        m_nSequence = SEQUENCE_BOWIE_IDLE1;
                        break;
                    default:
                        m_nSequence--;
                }
                
            }
            
            pData->m_Value.m_Int = m_nSequence;
        }
    }
    
    return g_pSequence(pData, pStruct, pOut);
}


