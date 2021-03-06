/*
 *	Global functions generated by FORMPP.  Intended to be
 *	included in another source file.
 *	
 */

/* prototype */
FLD * PfldCreate( int );

_public
FLD *
PfldCreate( int ifld )
{
	FLD		*pfld;

	AssertSz(ifld, "ifld == 0");

	switch (ifld)
	{
	default:
		pfld= NULL;
		break;
	case 1:
		pfld= new FLDEDIT();
		break;
	case 2:
		pfld= new FLDPSHB();
		break;
	case 3:
		pfld= new FLDCHKB();
		break;
	case 4:
		pfld= new FLDRADB();
		break;
	case 5:
		pfld= new FLDRADG();
		break;
	case 6:
		pfld= new FLDLABEL();
		break;
	case 7:
		pfld= new FLDRECT();
		break;
	case 8:
		pfld= new FLDPASSWORD();
		break;
	}

	return pfld;
}


/* prototype */
FIN * PfinCreate( int );

_public
FIN *
PfinCreate( int ifinMap )
{
	FIN *	pfin;

	AssertSz(ifinMap, "ifinMap == 0");

	switch (ifinMap)
	{
	default:
		pfin= NULL;
		break;
	case 1:
		pfin= new FINPUSHIT();
		break;
	}

	return pfin;
}











					
