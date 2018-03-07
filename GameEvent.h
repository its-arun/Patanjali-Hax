//
//  GameEvent.h
//  Patanjali
//
//  Created by Arun Chaudhary on 07/03/18.
//  Copyright © 2018 felli0t. All rights reserved.
//

#include "main.hpp"

#ifndef PATANJALI_GAMEEVENT_H_H
#define PATANJALI_GAMEEVENT_H_H

class IGameEvent
{
public:
    const char* GetName( )
    {
        return getvfunc<const char*( * )( void* )>( this, 1 )( this );
    }
    
    int GetInt( const char* szKeyName, int nDefault = 0 )
    {
        return getvfunc<int( * )( void*, const char*, int )>( this, 6 )( this, szKeyName, nDefault );
    }
    
    const char* GetString( const char* szKeyName )
    {
        return getvfunc<const char*( * )( void*, const char*, int )>( this, 9 )( this, szKeyName, 0 );
    }
    
    void SetString( const char* szKeyName, const char* szValue )
    {
        getvfunc<void( * )( void*, const char*, const char* )>( this, 16 )( this, szKeyName, szValue );
    }
};

class IGameEventManager2
{
public:
    bool FireEventClientSide( IGameEvent* pEvent )
    {
        return getvfunc<bool( * )( void*, IGameEvent* )>( this, 9 )( this, pEvent );
    }
};

#endif
