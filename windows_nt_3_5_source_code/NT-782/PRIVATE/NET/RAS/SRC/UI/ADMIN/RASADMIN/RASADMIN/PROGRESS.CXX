/**********************************************************************/
/**                       Microsoft Windows/NT                       **/
/**                Copyright(c) Microsoft Corp., 1991                **/
/**********************************************************************/

/*
    progress.cxx
    This file contains the class definition for the PROGRESS_CONTROL
    class.

    The PROGRESS_CONTROL class is a display object derived from the
    ICON_CONTROL class.  PROGRESS_CONTROL adds a new method Advance()
    for cycling through a set of icons.


    FILE HISTORY:
        EamC        28-Jun-1993 Adopted for RasAdmin
        KeithMo     06-Oct-1991 Win32 Conversion.
        KeithMo     03-Oct-1991 Created.

*/
#if 0
#define INCL_WINDOWS
#define INCL_WINDOWS_GDI
#define INCL_DOSERRORS
#define INCL_NETCONS
#define INCL_NETERRORS
#include <lmui.hxx>

#define INCL_BLT_WINDOW
#define INCL_BLT_DIALOG
#define INCL_BLT_CONTROL
#define INCL_BLT_MISC
#define INCL_BLT_CLIENT
#include <blt.hxx>

#if defined(DEBUG)
static const CHAR szFileName[] = __FILE__;
#define _FILENAME_DEFINED_ONCE szFileName
#endif  // DEBUG

#include <uiassert.hxx>

#include <dbgstr.hxx>

#include <progress.hxx>
#endif

#include "precomp.hxx"

//
//  PROGRESS_INDICATOR methods.
//

/*******************************************************************

    NAME:       PROGRESS_CONTROL :: PROGRESS_CONTROL

    SYNOPSIS:   PROGRESS_CONTROL class constructor.

    ENTRY:      powner                  - Owning window.

                cid                     - Cid for this control

                idFirstIcon             - Resource ID for the first
                                          icon in the icon list.

                cIcons                  - The number of icons to cycle.

    HISTORY:
        KeithMo     03-Oct-1991 Created.

********************************************************************/
PROGRESS_CONTROL :: PROGRESS_CONTROL( OWNER_WINDOW * powner,
                                      CID            cid,
                                      UINT           idFirstIcon,
                                      UINT           cIcons )
  : ICON_CONTROL( powner, cid ),
    _cIcons( cIcons ),
    _idFirstIcon( idFirstIcon ),
    _usCurrent( 0 )
{
    CtAux();


}   // PROGRESS_CONTROL :: PROGRESS_CONTROL


/*******************************************************************

    NAME:       PROGRESS_CONTROL :: ~PROGRESS_CONTROL

    SYNOPSIS:   PROGRESS_CONTROL class destructor.

    HISTORY:
        KeithMo     03-Oct-1991 Created.

********************************************************************/
PROGRESS_CONTROL :: ~PROGRESS_CONTROL()
{
    //
    //  This space intentionally left blank.
    //

}   // PROGRESS_CONTROL :: ~PROGRESS_CONTROL


/*******************************************************************

    NAME:       PROGRESS_CONTROL :: CtAux

    SYNOPSIS:   Auxilliary constructor.

    HISTORY:
        KeithMo     03-Oct-1991 Created.

********************************************************************/
VOID PROGRESS_CONTROL :: CtAux( VOID )
{
    //
    //  Ensure we constructed properly.
    //

    if( QueryError() != NERR_Success )
    {
    	::OutputDebugString((LPCTSTR)(L"Progress died\n"));
        return;
    }

    //
    //  Display the initial icon.
    //

    Advance();

}   // PROGRESS_CONTROL :: CtAux


/*******************************************************************

    NAME:       PROGRESS_CONTROL :: Advance

    SYNOPSIS:   Advance the progress indicator to the next icon.

    ENTRY:      nStep                   - The number of "steps" to
                                          increment the icon by.
                                          Default = 1.

    HISTORY:
        KeithMo     03-Oct-1991 Created.

********************************************************************/
VOID PROGRESS_CONTROL :: Advance( INT nStep )
{
    //
    //  Display the current icon.
    //

    SetIcon( MAKEINTRESOURCE( _idFirstIcon + _usCurrent ) );

    //
    //  We need this Invalidate() call here to get around a bug
    //  in Win/NT which causes icon controls to not be repainted
    //  after a STM_SETICON message.  Remove this bulls**t when
    //  NT has been fixed!
    //

    Invalidate( TRUE );

    //
    //  Advance to the next icon in the cycle.
    //

    _usCurrent = ( _usCurrent + nStep ) % _cIcons;

}   // PROGRESS_CONTROL :: Advance
