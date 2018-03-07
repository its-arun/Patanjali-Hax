#include "main.hpp"

ISurface* pSurface = nullptr;
IPanel* pPanel = nullptr;
ICvar* pCvar = nullptr;
IGameEventManager2* pGameEventManager = nullptr;
IBaseClientDLL* pClient = nullptr;
IClientEntityList* pEntList = nullptr;
IVDebugOverlay* pOverlay = nullptr;
IEngineClient* pEngine = nullptr;
IVModelInfo* pModelInfo = nullptr;
IEngineTrace* pEngineTrace = nullptr;
IClientMode* pClientMode = nullptr;
RecvVarProxyFn g_pSequence  = nullptr;

VMT* game_event_vmt = nullptr;
VMT* testvmt = nullptr;
VMT* client_vmt = nullptr;
VMT* input_vmt = nullptr;
