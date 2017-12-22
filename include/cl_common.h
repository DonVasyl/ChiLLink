/*

  ChiLLink - portable OMF-to-MZ linker
  Copyright (c) DonVasyl (i INNE GWIAZDY), 2017 - 2018

*/

#ifndef CL_COMMON_H_INCLUDED
#define CL_COMMON_H_INCLUDED

// STD headers

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

// constants

// in items number
#define CL_MAIN_ARGC 0x02

// in bytes
#define CL_ERRORS_ERRORNAMELEN_MAX 32
#define CL_ERRORS_ERRORHELPLEN_MAX 80
#define CL_ARGS_SWITCHNAMELEN_MAX 32
#define CL_ARGS_SWITCHVALUELEN_MAX 128
#define CL_ARGS_SWITCHHELPLEN_MAX 80
#define CL_ARGS_COMMNAMELEN_MAX 32
#define CL_ARGS_COMMHELPLEN_MAX 80

// in strings
#define CL_ERRORS_ERRORFORMAT "Error 0x%04x occured: %s\n"
#define CL_MAIN_DEFAULTOUTPUT "./A.OUT"

// enumerators

typedef enum {
  cl_main_eError_eOK = 0x00,
  cl_main_eError_eInternal,
  cl_main_eError_eARGC,
  cl_main_eError_eCommand,
  cl_main_eError_eSwitch,
  cl_main_eError_eRead,
  cl_main_eError_eWrite,
  cl_main_eError_eFormatOMF,
  cl_main_eError_eSymbol,
  cl_main_eError_eMAX,
} cl_main_eError_e;

typedef enum {
  cl_args_sState_eMissing = 0x00,
  cl_args_sState_eHandled,
  cl_args_sState_eMAX
} cl_args_sState_e;

// custom data types

typedef struct cl_errors_error {
  char name[CL_ERRORS_ERRORNAMELEN_MAX];
  char help[CL_ERRORS_ERRORHELPLEN_MAX];
  int intEnum;
  void (*handler)(void);
  struct cl_errors_error *next;
} cl_errors_error_t;

typedef struct cl_args_switch {
  cl_args_sState_e state;
  char name[CL_ARGS_SWITCHNAMELEN_MAX];
  char value[CL_ARGS_SWITCHVALUELEN_MAX];
  void (*handler)(char * pValue);
  void (*missing)(void);
  char help[CL_ARGS_SWITCHHELPLEN_MAX];
  struct cl_args_switch *next;
} cl_args_switch_t;

typedef struct cl_args_command {
  char name[CL_ARGS_COMMNAMELEN_MAX];
  char help[CL_ARGS_COMMHELPLEN_MAX];
  void (*handler)(void);
  struct cl_args_command *next;
} cl_args_command_t;

typedef struct cl_args_file {
  char *path;
  struct cl_args_file *next;
} cl_args_file_t;

#endif /* CL_COMMON_H_INCLUDED */

