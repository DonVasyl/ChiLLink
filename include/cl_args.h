/*

  ChiLLink - portable OMF-to-MZ linker
  Copyright (c) DonVasyl (i INNE GWIAZDY), 2017 - 2018

*/

#ifndef CL_ARGS_H_INCLUDED
#define CL_ARGS_H_INCLUDED

extern void CL_ARGS_RegisterCmdLineSwitch(char *pName, char *pHelp, \
					  void (*pHandler)(char * pValue), \
					  void (*pMissing)(void));
extern void CL_ARGS_RegisterCmdLineCommand(char *pName, char *pHelp, \
					   void (*pHandler)(void));

extern void CL_ARGS_PrintSwitchHelps(void);
extern void CL_ARGS_PrintCommandHelps(void);

extern void CL_ARGS_ProcessArguments(int pArgc, char *(*pArgv[]));
extern cl_args_file_t *CL_ARGS_GetFiles(void);

extern char *CL_ARGS_GetCommandName(void); // one command per session only

#endif /* CL_ARGS_H_INCLUDED */
