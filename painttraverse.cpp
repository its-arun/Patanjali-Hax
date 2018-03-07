#include "main.hpp"

/* 
 
 To mention, since we don't have SDL2 added.. YET, we are simply
 gonna do a boolean for keypress.
 
 */

bool bKeys[256];
bool bKeyPressed(int _keyCode) {
    return bKeys[_keyCode];
}

#define GetAsyncKeyState(_x) bKeyPressed(_x);

/* Some test drawings */
void DrawBox(int x, int y, int w, int h, Color color) {
    pSurface->DrawSetColor(color);
    pSurface->DrawOutlinedRect(x, y, x + w, y + h);
}

void DrawBoxOutline(int x, int y, int w, int h, Color color) {
    Drawings->DrawBox(x + 1, y + 1, w - 2, h - 2, Color(0, 0, 0, 120));
    Drawings->DrawBox(x - 1, y - 1, w + 2, h + 2, Color(0, 0, 0, 120));
    Drawings->DrawBox(x, y, w, h, color);
}
/* end */

/* test esp shit */
enum Teams {
    Terrorist = 2,
    CounterTerrorist = 3
};

bool ScreenTransform( const Vector& point, Vector& screen )
{
    auto& worldToScreen = pEngine->GetWorldToScreenMatrix( );

    auto tempX = 0.f, tempY = 0.f, w = 0.f;

    for ( auto i = 0; i < 3; i++ )
    {
        tempX += worldToScreen[ 0 ][ i ] * point[ i ];
        tempY += worldToScreen[ 1 ][ i ] * point[ i ];
        w += worldToScreen[ 3 ][ i ] * point[ i ];
    }

    screen.x = tempX + worldToScreen[ 0 ][ 3 ];
    screen.y = tempY + worldToScreen[ 1 ][ 3 ];
    screen.z = 0.0f;
    w += worldToScreen[ 3 ][ 3 ];

    auto behind = w < 0.001f;

    if ( behind )
    {
        screen *= 100000;
    }
    else
    {
        auto invw = 1.0f / w;
        screen *= invw;
    }

    return behind;
}



bool WorldToScreen(Vector& vFrom, Vector& vTo) {

    return (pOverlay->ScreenPosition(vFrom, vTo) != 1);
    /*
    int ScreenWidth, ScreenHeight;
    pEngine->GetScreenSize(ScreenWidth, ScreenHeight);

    if ( !ScreenTransform( vFrom, vTo ) )
    {
        float x = ScreenWidth >> 1;
        float y = ScreenHeight >> 1;

        x += 0.5 * vTo.x * ScreenWidth + 0.5;
        y -= 0.5 * vTo.y * ScreenHeight + 0.5;

        vTo.x = x;
        vTo.y = y;

        return true;
    }

    return false;
     */
}



auto TestTrace(C_BaseEntity* pEntity, C_BaseEntity* pLocal) -> bool { /* Just a simple visible check :^) */
    Ray_t ray;
    trace_t trace;
    CTraceFilter filter;
    filter.pSkip = pLocal;
    
    ray.Init(pLocal->GetEyePosition(), pEntity->GetEyePosition());
    pEngineTrace->TraceRay(ray, 0x4600400B, &filter, &trace);
    
    return (trace.m_pEnt == pEntity || trace.fraction > 0.99f);
}

void DrawSkeleton(C_BaseEntity* pEntity, Color color) {
    studiohdr_t* pStudioModel = pModelInfo->GetStudioModel( pEntity->GetModel() );
    if ( pStudioModel ) {
        static matrix3x4_t pBoneToWorldOut[128];
        if ( pEntity->SetupBones( pBoneToWorldOut, 128, 256, 0) )
        {
            for ( int i = 0; i < pStudioModel->numbones; i++ )
            {
                mstudiobone_t* pBone = pStudioModel->pBone( i );
                if ( !pBone || !( pBone->flags & 256 ) || pBone->parent == -1 )
                    continue;
                
                
                Vector vBone1 = pEntity->GetBonePosition(i);
                Vector vBoneOut1;
                
                Vector vBone2 = pEntity->GetBonePosition(pBone->parent);
                Vector vBoneOut2;
                
                if(WorldToScreen(vBone1, vBoneOut1) && WorldToScreen(vBone2, vBoneOut2)) {
                    Drawings->DrawLine(vBoneOut1.x, vBoneOut1.y, vBoneOut2.x, vBoneOut2.y, color);
                }
            }
        }
    }
}

/*
*/

void makeshittyesp() {
    C_BaseEntity *pLocalPlayer = (C_BaseEntity *) pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    Vector vecTop, vecPos;

    for (int i = 0; i < pEntList->GetHighestEntityIndex(); i++) {
        C_BaseEntity *pEntity = (C_BaseEntity *) pEntList->GetClientEntity(i);

        if (!pEntity)
            continue;

        if (pEntity->GetHealth() < 1)
            continue;

        if (pEntity->GetDormant())
            continue;

        if (pEntity == pLocalPlayer)
            continue;

        if(pEntity->GetTeam() == pLocalPlayer->GetTeam())
            continue;

        auto isVisible = TestTrace(pEntity, pLocalPlayer);

        IEngineClient::player_info_t pInfo;
        pEngine->GetPlayerInfo(i, &pInfo);

        auto vecTop3D = pEntity->GetVecOrigin();
        auto vecMax = pEntity->GetCollideable()->OBBMaxs();
        auto vecPos3D = pEntity->GetVecOrigin();

        vecTop3D += Vector(0, 0, vecMax.z);

        if (WorldToScreen(vecTop3D, vecTop) && WorldToScreen(vecPos3D, vecPos)) {

            auto iMiddle = vecPos.y - vecTop.y;
            auto iWidth = iMiddle / 4;

            Color clr = [&]() -> Color {
                if (pEntity->GetTeam() == Terrorist) {
                    return isVisible ? Color::Red() : Color::Yellow();
                }

                if (pEntity->GetTeam() == CounterTerrorist) {
                    return isVisible ? Color::Green() : Color::Blue();
                }
                return Color(0, 0, 0);
            }();

            DrawBox(vecTop.x - iWidth, vecTop.y, iWidth * 2, iMiddle, clr);
            DrawBox(vecTop.x - iWidth - 1, vecTop.y - 1, (iWidth * 2) + 2, iMiddle + 2, Color(0, 0, 0, 255));
            DrawBox(vecTop.x - iWidth + 1, vecTop.y + 1, (iWidth * 2) - 2, iMiddle - 2, Color(0, 0, 0, 255));
            Drawings->FillRGBA( vecTop.x - iWidth, vecTop.y + iMiddle + 3, ( iWidth *2 ) + 2, 5, Color( 0, 0, 0, 255 ) );
            Drawings->FillRGBA( vecTop.x - iWidth + 1, vecTop.y + iMiddle + 4, (( iWidth * 2 ) / 100.f) * pEntity->GetHealth(), 3, Color( 0, 255, 0, 255 ) );

            Drawings->DrawString(vecTop.x, vecTop.y - 12/* 12 = the font size */, Color::White(),
                                 testfont, true, pInfo.name);
            DrawSkeleton(pEntity, Color::White());
        }
    }
}
/* end */



void hkPaintTraverse(void* thisptr, VPANEL vguiPanel, bool forceRepaint, bool allowForce) {
    testvmt->GetOriginalMethod<tPaintTraverse>(42)(thisptr, vguiPanel, forceRepaint, allowForce);
    
    static VPANEL currentPanel = 0;
    
    if(!currentPanel) {
        
        if(strstr(pPanel->GetName(vguiPanel), "FocusOverlayPanel")) {
            
            testfont = pSurface->CreateFont();
            pSurface->SetFontGlyphSet(testfont, "Tahoma", 12, 250, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
            
            currentPanel = vguiPanel;
            
        }
    }
    
    if(vguiPanel == currentPanel) {
    
        /* Draw your stuff here. */
        makeshittyesp();
       // DrawBoxOutline(10, 20, 30, 40, Color::Black());
        
        /* Test with FontAwesome */
        //Drawings->DrawString(10, 20, Color::White(), testfont, "Patanjali Hax");

    }
    
}
