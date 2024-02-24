#include "detour.h"

PVOID SetDetour(PVOID& target, PVOID detour)
{
    LONG result = DetourTransactionBegin();
    if (result != NO_ERROR)
    {
        return NULL;
    }

    result = DetourUpdateThread(GetCurrentThread());
    if (result != NO_ERROR)
    {
        DetourTransactionAbort();
        return NULL;
    }

    result = DetourAttach(&target, detour);
    if (result != NO_ERROR)
    {
        DetourTransactionAbort();
        return NULL;
    }

    result = DetourTransactionCommit();
    if (result != NO_ERROR)
    {
        DetourTransactionAbort();
        return NULL;
    }

    return target;
}
BOOL RemoveDetour( PVOID* ppTarget, PVOID Handler )
{
	if ( DetourTransactionBegin( ) != NO_ERROR )
		return FALSE;

	if ( DetourUpdateThread( GetCurrentThread( ) ) != NO_ERROR )
	{
		DetourTransactionCommit( );
		return FALSE;
	}

	if ( DetourDetach( ppTarget, Handler ) != NO_ERROR )
	{
		DetourTransactionCommit( );
		return FALSE;
	}

	if ( DetourTransactionCommit( ) != NO_ERROR )
	{
		DetourTransactionAbort( );
		return FALSE;
	}

	return TRUE;
}
