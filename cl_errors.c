/*

  ChiLLink - portable OMF-to-MZ linker
  Copyright (c) DonVasyl (i INNE GWIAZDY), 2017 - 2018

*/

#include "include/cl_common.h"

static cl_errors_error_t *cl_errors_errors = NULL, *cl_errors_currentError = NULL;
static char *cl_errors_errorFormat = CL_ERRORS_ERRORFORMAT;

static cl_errors_error_t *CL_ERRORS_getNewError(void) {
  if(cl_errors_errors == NULL) {
    cl_errors_errors = malloc(sizeof(cl_errors_error_t));
    if(cl_errors_errors == NULL) {
      printf("FATAL_ERROR: ERRORS: Memory allocation error (error structure, root)!\n");
      exit(EXIT_FAILURE);
    }
    memset(cl_errors_errors, 0x00, sizeof(cl_errors_error_t));
    return cl_errors_errors;
  }

  cl_errors_error_t *lError = cl_errors_errors;
  while(lError->next != NULL) {
    lError = lError->next;
  }

  lError->next = malloc(sizeof(cl_errors_error_t));
  if(lError->next == NULL) {
    printf("FATAL_ERROR: ERRORS: Memory allocation error (errors structure)!\n");
    exit(EXIT_FAILURE);
  }
  memset(lError->next, 0x00, sizeof(cl_errors_error_t));

  return lError->next;
}

static cl_errors_error_t *CL_ERRORS_getErrorByName(char *pName) {
  cl_errors_error_t *lError = cl_errors_errors;
  while(lError != NULL) {
    if(strcmp(lError->name, pName) == 0x00) {
      return lError;
    }
    lError = lError->next;
  }
  return NULL;
}

static cl_errors_error_t *CL_ERRORS_getErrorByEnum(int pEnum) {
  cl_errors_error_t *lError = cl_errors_errors;
  while(lError != NULL) {
    if(lError->intEnum == pEnum) {
      return lError;
    }
    lError = lError->next;
  }
  return NULL;
}

void CL_ERRORS_RegisterError(int pEnum, char *pName, char *pHelp,
			     void (*pHandler)(void)) {

  if(pName == NULL) {
    printf("FATAL_ERROR: ERRORS: Attempt to register an error with NULL name!\n");
    exit(EXIT_FAILURE);
  }

  size_t lTextLen = strlen(pName);
  if(lTextLen < 0x01) {
    printf("FATAL_ERROR: ERRORS: Attempt to register an error with empty name!\n");
    exit(EXIT_FAILURE);		
  }
  
  cl_errors_error_t *lNewError = CL_ERRORS_getNewError();

  if(lTextLen > CL_ARGS_COMMNAMELEN_MAX) lTextLen = CL_ARGS_COMMNAMELEN_MAX;
  strncpy(lNewError->name, pName, lTextLen);

  if(pHelp != NULL) {
    lTextLen = strlen(pHelp);
    if(lTextLen > CL_ARGS_COMMHELPLEN_MAX) lTextLen = CL_ARGS_COMMHELPLEN_MAX;
    strncpy(lNewError->help, pHelp, lTextLen);
  }

  lNewError->intEnum = pEnum;
  lNewError->handler = pHandler;
  lNewError->next = NULL;
}

void CL_ERRORS_PrintErrorHelps(void) {
  cl_errors_error_t *lError = cl_errors_errors;
  
  while(lError != NULL) {
    printf(" ERROR_%s - %s\n", lError->name, lError->help);
    lError = lError->next;
  }
}

void CL_ERRORS_Error(int pEnum, char *pMessage, ...) {
  cl_errors_error_t *lError = CL_ERRORS_getErrorByEnum(pEnum);

  if(pMessage != NULL) {
    va_list lAp;

    char *lMessage = malloc(strlen(pMessage) + strlen(cl_errors_errorFormat));
    sprintf(lMessage, cl_errors_errorFormat, pEnum, pMessage);
    
    va_start(lAp, pMessage);
    vfprintf(stdout, lMessage, lAp);
    va_end(lAp);

    free(lMessage);
  }
  
  if(lError != NULL) {
    cl_errors_currentError = lError;
  } else {
    printf("FATAL_ERROR: ERRORS: unregistered error %i\n", pEnum);
    exit(EXIT_FAILURE);
  }
}

int CL_ERRORS_GetCurrentErrorEnum(void) {
  if(cl_errors_currentError != NULL) {
    return cl_errors_currentError->intEnum;
  } else return -1;
}

void CL_ERRORS_ProcessErrors(void) {
  if(cl_errors_currentError != NULL) {
    printf("\nERROR_%s\n", cl_errors_currentError->name);
    if(cl_errors_currentError->handler != NULL) {
      (cl_errors_currentError->handler)();
    }
  }
}
