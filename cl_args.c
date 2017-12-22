/*

  ChiLLink - portable OMF-to-MZ linker
  Copyright (c) DonVasyl (i INNE GWIAZDY), 2017 - 2018

*/

#include "include/cl_common.h"
#include "include/cl_errors.h"
#include "include/cl_main.h"

static cl_args_switch_t *cl_args_switches = NULL;
static cl_args_command_t *cl_args_commands = NULL, *cl_args_commandCurrent = NULL;
static cl_args_file_t *cl_args_files = NULL;

static cl_args_switch_t *CL_ARGS_getNewCmdLineSwitch(void) {
  if(cl_args_switches == NULL) {
    cl_args_switches = malloc(sizeof(cl_args_switch_t));
    if(cl_args_switches == NULL) {
      CL_ERRORS_Error(cl_main_eError_eInternal,
		      "ARGS: Memory allocation error (switch structure, root)!");
      return NULL;
    }
    memset(cl_args_switches, 0x00, sizeof(cl_args_switch_t));
    return cl_args_switches;
  }

  cl_args_switch_t *lSwitch = cl_args_switches;
  while(lSwitch->next != NULL) {
    lSwitch = lSwitch->next;
  }

  lSwitch->next = malloc(sizeof(cl_args_switch_t));
  if(lSwitch->next == NULL) {
    CL_ERRORS_Error(cl_main_eError_eInternal,
		    "ARGS: Memory allocation error (switch structure)!");
    return NULL;
  }
  memset(lSwitch->next, 0x00, sizeof(cl_args_switch_t));
  return lSwitch->next;
}

static cl_args_switch_t *CL_ARGS_getSwitchByName(char *pName) {
  cl_args_switch_t *lSwitch = cl_args_switches;
  while(lSwitch != NULL) {
    if(strcmp(lSwitch->name, pName) == 0x00) {
      return lSwitch;
    }
    lSwitch = lSwitch->next;
  }
  return NULL;
}

static cl_args_command_t *CL_ARGS_getNewCmdLineCommand(void) {
  if(cl_args_commands == NULL) {
    cl_args_commands = malloc(sizeof(cl_args_command_t));
    if(cl_args_commands == NULL) {
      CL_ERRORS_Error(cl_main_eError_eInternal,
		      "ARGS: Memory allocation error (command structure, root)!");
      return NULL;
    }
    memset(cl_args_commands, 0x00, sizeof(cl_args_command_t));
    return cl_args_commands;
  }

  cl_args_command_t *lCommands = cl_args_commands;
  while(lCommands->next != NULL) {
    lCommands = lCommands->next;
  }

  lCommands->next = malloc(sizeof(cl_args_command_t));
  if(lCommands->next == NULL) {
    CL_ERRORS_Error(cl_main_eError_eInternal,
		    "ARGS: Memory allocation error (command structure)!");
    return NULL;
  }
  memset(lCommands->next, 0x00, sizeof(cl_args_command_t));

  return lCommands->next;
}

static cl_args_command_t *CL_ARGS_getCommandByName(char *pName) {
  cl_args_command_t *lCommand = cl_args_commands;
  while(lCommand != NULL) {
    if(strcmp(lCommand->name, pName) == 0x00) {
      return lCommand;
    }
    lCommand = lCommand->next;
  }
  return NULL;
}

static cl_args_file_t *CL_ARGS_getNewFile(void) {
  if(cl_args_files == NULL) {
    cl_args_files = malloc(sizeof(cl_args_file_t));
    if(cl_args_files == NULL) {
      CL_ERRORS_Error(cl_main_eError_eInternal,
		      "ARGS: Memory allocation error (file structure, root)!");
      return NULL;
    }
    memset(cl_args_files, 0x00, sizeof(cl_args_file_t));
    return cl_args_files;
  }

  cl_args_file_t *lFiles = cl_args_files;
  while(lFiles->next != NULL) {
    lFiles = lFiles->next;
  }

  lFiles->next = malloc(sizeof(cl_args_file_t));
  if(lFiles->next == NULL) {
    CL_ERRORS_Error(cl_main_eError_eInternal,
		    "ARGS: Memory allocation error (file structure)!");
    return NULL;
  }
  memset(lFiles->next, 0x00, sizeof(cl_args_file_t));

  return lFiles->next; 
}

void CL_ARGS_RegisterCmdLineSwitch(char *pName, char *pHelp, void (*pHandler)(char * pValue), void (*pMissing)(void)) {

  if(pName == NULL) {
    CL_ERRORS_Error(cl_main_eError_eInternal,
		    "ARGS: Attempt to register command line switch with NULL name!");
    return;
  }

  size_t lTextLen = strlen(pName);
  if(lTextLen < 0x01) {
    CL_ERRORS_Error(cl_main_eError_eInternal,
		    "ARGS: Attempt to register command line switch with empty name!");
    return;	
  }

  cl_args_switch_t *lNewSwitch = CL_ARGS_getNewCmdLineSwitch();

  lNewSwitch->state = cl_args_sState_eMissing;

  if(lTextLen > CL_ARGS_SWITCHNAMELEN_MAX) lTextLen = CL_ARGS_SWITCHNAMELEN_MAX;
  strncpy(lNewSwitch->name, pName, lTextLen);

  if(pHelp != NULL) {
    lTextLen = strlen(pHelp);
    if(lTextLen > CL_ARGS_SWITCHHELPLEN_MAX) lTextLen = CL_ARGS_SWITCHHELPLEN_MAX;
    strncpy(lNewSwitch->help, pHelp, lTextLen);    
  }

  lNewSwitch->handler = pHandler;
  lNewSwitch->missing = pMissing;
  lNewSwitch->next = NULL;
} 

void CL_ARGS_RegisterCmdLineCommand(char *pName, char *pHelp, void (*pHandler)(void)) {

  if(pName == NULL) {
    CL_ERRORS_Error(cl_main_eError_eInternal,
		    "ARGS: Attempt to register command line command with NULL name!");
    return;
  }

  size_t lTextLen = strlen(pName);
  if(lTextLen < 0x01) {
    CL_ERRORS_Error(cl_main_eError_eInternal,
		    "ARGS: Attempt to register command line command with empty name!");
    return;
  }

  cl_args_command_t *lNewCommand = CL_ARGS_getNewCmdLineCommand();

  if(lTextLen > CL_ARGS_COMMNAMELEN_MAX) lTextLen = CL_ARGS_COMMNAMELEN_MAX;
  strncpy(lNewCommand->name, pName, lTextLen);

  if(pHelp != NULL) {
    lTextLen = strlen(pHelp);
    if(lTextLen > CL_ARGS_COMMHELPLEN_MAX) lTextLen = CL_ARGS_COMMHELPLEN_MAX;
    strncpy(lNewCommand->help, pHelp, lTextLen);    
  }

  lNewCommand->handler = pHandler;
  lNewCommand->next = NULL;
}

void CL_ARGS_PrintSwitchHelps(void) {
  cl_args_switch_t *lSwitch = cl_args_switches;
  
  while(lSwitch != NULL) {
    printf(" --%s - %s\n", lSwitch->name, lSwitch->help);
    lSwitch = lSwitch->next;
  }
}

void CL_ARGS_PrintCommandHelps(void) {
  cl_args_command_t *lCommand = cl_args_commands;

  while(lCommand != NULL) {
    printf(" %s - %s\n", lCommand->name, lCommand->help);
    lCommand = lCommand->next;
  }
}

void CL_ARGS_ProcessArguments(int pArgc, char *(*pArgv[])) {
  cl_args_commandCurrent = NULL;
  
  for(int lN = 0x01; lN < pArgc; lN++) {
    if(strncmp((*pArgv)[lN], "--", 0x02) == 0x00) {
      cl_args_switch_t *lSwitch = CL_ARGS_getSwitchByName(strtok((*pArgv)[lN], "-="));
      if(lSwitch != NULL) {
	lSwitch->state = cl_args_sState_eHandled;
	char *lValue = strtok(NULL, "-=");
	if(lValue != NULL) {
	  strncpy(lSwitch->value, lValue, CL_ARGS_SWITCHVALUELEN_MAX);
	}
      } else {
	CL_ERRORS_Error(cl_main_eError_eSwitch, "Unknown switch: %s", (*pArgv)[lN]);
	CL_MAIN_PrintUsage();
	return;
      }
    } else {
      cl_args_command_t *lCommand = CL_ARGS_getCommandByName((*pArgv)[lN]);
      if(lCommand != NULL) {
	cl_args_commandCurrent = lCommand;
      } else {
	cl_args_file_t *lFile = CL_ARGS_getNewFile();
	size_t lPathSize = strlen((*pArgv)[lN]) + 1;
	lFile->path = malloc(lPathSize);
	memset(lFile->path, 0x00, lPathSize);
	strcpy(lFile->path, (*pArgv)[lN]);
      }
    }
  }

  if (cl_args_commandCurrent == NULL) {
    CL_ERRORS_Error(cl_main_eError_eCommand, "No known command in command line.");
    CL_MAIN_PrintUsage();
    return;
  }
  
  // missing missy
  cl_args_switch_t *lSwitch = cl_args_switches;
  while(lSwitch != NULL) {
    switch(lSwitch->state){
    case cl_args_sState_eHandled:
      if(lSwitch->handler != NULL) lSwitch->handler(lSwitch->value);
      break;
    case cl_args_sState_eMissing:
    default:
      if(lSwitch->missing != NULL) lSwitch->missing();
      break;
    }

    lSwitch = lSwitch->next;
  }

  if((cl_args_commandCurrent != NULL) &&
     (cl_args_commandCurrent->handler != NULL)) {
    cl_args_commandCurrent->handler();
  }
}

cl_args_file_t *CL_ARGS_GetFiles(void) {
  return cl_args_files;
}

char *CL_ARGS_GetCommandName(void) {
  return cl_args_commandCurrent->name;
}

