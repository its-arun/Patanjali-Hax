typedef void(*tFrameStage)(void*, ClientFrameStage_t);
typedef bool(*tFireGameEvent)(void*, IGameEvent*);

extern void hkFrameStage(void* thisptr, ClientFrameStage_t curStage);
extern bool hkFireEventClientSide(void* thisptr, IGameEvent* event);

