/*

  ChiLLink - portable OMF-to-MZ linker
  Copyright (c) DonVasyl (i INNE GWIAZDY), 2017 - 201

*/

#include "include/cl_common.h"
#include "include/cl_main.h"
#include "include/cl_args.h"

// SWITCH HANDLERS

void CL_HANDLERS_OutputSwitchHandler(char *pValue) {
  printf("OUTPUT_FILE: %s\n", pValue);
}

void CL_HANDLERS_OutputSwitchMissing(void) {
  printf("OUTPUT_FILE: %s\n", CL_MAIN_DEFAULTOUTPUT);
}

// COMMAND HANDLERS

void CL_HANDLERS_HelpCommandHandler(void) {
  printf("WE HELP:\n");
  CL_MAIN_PrintUsage();
}

void CL_HANDLERS_GreetCommandHandler(void) {
  printf("WE GREET:\n");
}

void CL_HANDLERS_LinkCommandHandler(void) {
  printf("WE LINK:\n");

  cl_args_file_t *lFiles = CL_ARGS_GetFiles();
  while(lFiles->next != NULL) {
    printf("%s\n",  lFiles->path);
    lFiles = lFiles->next;
  }
  printf("%s\n", lFiles->path);  
}

