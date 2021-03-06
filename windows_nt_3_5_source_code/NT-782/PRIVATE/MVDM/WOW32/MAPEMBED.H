/*++

Copyright (c) 1992  Microsoft Corporation

Module Name:

    mapembed.c

Abstract:

    This module contains the the prototypes of the functions that
    perform the mapping between the "embedding" section of win.ini,
    and the subkeys of HKEY_CLASSES_ROOT.

    This mapping is a hack implemented on Win3.1, that must also
    exist on NT.
    It is implemnted in the WOW layer, since only some win16 apps
    that read or write to the "embedding" section ( Excel and
    MsMail) depend on it.



Author:


    Jaime F. Sasson (jaimes) 25-Nov-1992



--*/

#if !defined( _MAP_EMBEDDING_SECTION_ )

#define _MAP_EMBEDDING_SECTION_


    BOOLEAN
    IsEmbeddingSection(
        IN  LPSTR   SectionName
        );

    BOOLEAN
    IsFileWinIni(
        IN  LPSTR   FileName
        );

    VOID
    UpdateEmbeddingAllKeys(
        );

    PSTR
    GetWinIniFullPath(
        );

    VOID
    SetLastTimeUpdated(
        );

    VOID
    UpdateEmbeddingKey(
        IN  LPSTR   KeyName
        );

    VOID
    UpdateClassesRootSubKey(
        IN  LPSTR   KeyName,
        IN  LPSTR   Value
        );

    BOOLEAN
    WasSectionRecentlyUpdated(
        );

#endif
