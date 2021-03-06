/**************************************************************************/
/*** SCICALC Scientific Calculator for Windows 3.00.12                  ***/
/*** By Kraig Brockschmidt, Microsoft Co-op, Contractor, 1988-1989      ***/
/*** (c)1989 Microsoft Corporation.  All Rights Reserved.               ***/
/***                                                                    ***/
/*** sciset.c                                                           ***/
/***                                                                    ***/
/*** Functions contained:                                               ***/
/***    SetRadix--Changes the number base and the radiobuttons.         ***/
/***    SetBox--Handles the checkboxes for inv/hyp.                     ***/
/***                                                                    ***/
/*** Functions called:                                                  ***/
/***    none                                                            ***/
/***                                                                    ***/
/*** Last modification Thu  31-Aug-1989                                 ***/
/**************************************************************************/

#include "scicalc.h"

extern HWND        hgWnd;
extern TCHAR        szBlank[6];
extern SHORT       nRadix, nTrig, nDecMode, nHexMode;
extern DWORD       dwChop;
extern TCHAR       *rgpsz[CSTRINGS];
#ifdef JAPAN
extern RECT        rcDeg[6];
#endif

/* SetRadix sets the display mode according to the selected button.       */

VOID NEAR SetRadix (WORD wRadix)
{
    register SHORT nx, n;
    static SHORT   nRadish[4]={2,8,10,16}; /* Number bases.               */
#ifdef JAPAN //KKBUGFIX
        extern  BOOL    bPaint; // Used in DisplayNum() to decide whether have to
                                                        // increase nNumZeros [yutakan]
        extern  BOOL    bFirstPaint;
#endif

    nx=1;
    if (wRadix!=DEC)
        {
        nx=0;
        nTrig=nHexMode;
        }
    else
        nTrig=nDecMode;

    CheckRadioButton(hgWnd, BIN, HEX, wRadix); /* Check the selection.    */
    CheckRadioButton(hgWnd, DEG, GRAD,nTrig+DEG);

    for (n=0; n<3; n++)
    {
#ifdef JAPAN
        MoveWindow(GetDlgItem(hgWnd, DEG+n),
                   rcDeg[n+(nx*3)].left,
                   rcDeg[n+(nx*3)].top,
                   rcDeg[n+(nx*3)].right,
                   rcDeg[n+(nx*3)].bottom,
                   TRUE);
#endif
        SetDlgItemText(hgWnd, DEG+n, rgpsz[IDS_MODES+n+(nx*3)]);
    }

    nRadix=nRadish[wRadix-BIN]; /* Set the radix.  Note the dependency on */
                                /* the numerical order BIN/OCT/DEC/HEX.   */
#ifdef JAPAN //KKBUGFIX
        /*       We don't want to increase number of padding '0' in DisplayNum().
        **      Set a flag in order to prevent this.    [yutakan:09/04/91]
        */
        if(bFirstPaint)
                bPaint = TRUE;
#endif

    DisplayNum ();
#ifdef JAPAN //KKBUGFIX
        if(bPaint)
                bPaint = FALSE; // Reset the flag.
#endif
}


/* Check/uncheck the visible inverse/hyperbolic                           */
VOID NEAR SetBox (int id, BOOL bOnOff)
    {
    CheckDlgButton(hgWnd, id, (WORD) bOnOff);
    return;
    }
