/*

  ChiLLink - portable OMF-to-MZ linker
  Copyright (c) DonVasyl (i INNE GWIAZDY), 2017 - 2018

*/

#ifndef CL_ERRORS_H_INCLUDED
#define CL_ERRORS_H_INCLUDED

extern void CL_ERRORS_RegisterError(int pEnum, char *pName, char *pHelp,
				    void (*pHandler)(void));

extern void CL_ERRORS_PrintErrorHelps(void);

extern void CL_ERRORS_Error(int pEnum, char *pMessage, ...);

extern int CL_ERRORS_GetCurrentErrorEnum(void);
extern void CL_ERRORS_ProcessErrors(void);

#endif /* CL_ERRORS_H_INCLUDED */

