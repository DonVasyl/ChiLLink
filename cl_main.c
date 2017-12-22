/*

  ChiLLink - portable OMF-to-MZ linker
  Copyright (c) DonVasyl (i INNE GWIAZDY), 2017 - 201

*/

#include "include/cl_common.h"
#include "include/cl_errors.h"
#include "include/cl_handlers.h"
#include "include/cl_args.h"

static void CL_MAIN_PrintAbout(void) {
  printf("Sneaky Chinchillas' ChiLLink, version 0.0.%04u%02u%02u\n", \
	 CT_BDATE_Y, CT_BDATE_M, CT_BDATE_D);
  printf("a highly portable OMF(.OBJ)-to-MZ(.EXE) linker for x86/DOS targets\n");
  printf("Copyright (c) DonVasyl (i INNE GWIAZDY), 2017 - 2018\n\n");
}

void CL_MAIN_PrintUsage(void) {
  printf("\nUsage: chillink --[SWITCH<=VALUE>] ... COMMAND [FILE] ...\n");

  printf("\nAvailable switches:\n");
  CL_ARGS_PrintSwitchHelps();

  printf("\nAvailable commands:\n");
  CL_ARGS_PrintCommandHelps(); 

  printf("\nPossible outputs:\n");
  CL_ERRORS_PrintErrorHelps();
}

static void CL_MAIN_fatalErrorHandler(void) {
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
  setbuf(stdout, NULL);

  // ORDER OF REGISTRATION MATTERS!
  // (helps printed and handlers/missings called by registration order)

  // ERRORS
  CL_ERRORS_RegisterError(cl_main_eError_eInternal, "INTERNAL",
			  "error in ChiLLink code",
			  CL_MAIN_fatalErrorHandler);
  CL_ERRORS_RegisterError(cl_main_eError_eARGC, "ARGC",
			  "too few command line arguments",
			  CL_MAIN_fatalErrorHandler);
  CL_ERRORS_RegisterError(cl_main_eError_eCommand, "COMMAND",
			  "no known command in command line",
			  CL_MAIN_fatalErrorHandler);
  CL_ERRORS_RegisterError(cl_main_eError_eSwitch, "SWITCH",
			  "unkown command line switch",
			  CL_MAIN_fatalErrorHandler);
  CL_ERRORS_RegisterError(cl_main_eError_eRead, "READ",
			  "can't read input file",
			  CL_MAIN_fatalErrorHandler);
  CL_ERRORS_RegisterError(cl_main_eError_eWrite, "WRITE",
			  "can't write ouput file",
			  CL_MAIN_fatalErrorHandler);
  CL_ERRORS_RegisterError(cl_main_eError_eFormatOMF, "OMF",
			  "object file format not compatible",
			  CL_MAIN_fatalErrorHandler);
  CL_ERRORS_RegisterError(cl_main_eError_eSymbol, "SYMBOL",
			  "symbol dependency not met",
			  CL_MAIN_fatalErrorHandler);
  CL_ERRORS_RegisterError(cl_main_eError_eOK, "OK",
			  "no error",
			  NULL);

  // SWITCHES
  CL_ARGS_RegisterCmdLineSwitch("output", "output file path (default: "CL_MAIN_DEFAULTOUTPUT")",
				CL_HANDLERS_OutputSwitchHandler,
				CL_HANDLERS_OutputSwitchMissing);

  // COMMANDS
  CL_ARGS_RegisterCmdLineCommand("help", "guess, what?",
				 CL_HANDLERS_HelpCommandHandler);
  CL_ARGS_RegisterCmdLineCommand("greet", "some greetings and kisses",
				 CL_HANDLERS_GreetCommandHandler);
  CL_ARGS_RegisterCmdLineCommand("link", "link OMF(.OBJ) file(s) into single MZ(.EXE)",
				 CL_HANDLERS_LinkCommandHandler);
  CL_ARGS_RegisterCmdLineCommand("check", "check OMF(.OBJ) or MZ(.EXE) file(s)",
				 NULL);

  // ACTION!
  CL_ERRORS_Error(cl_main_eError_eOK, NULL);

  CL_MAIN_PrintAbout();
  
  if(argc < CL_MAIN_ARGC) {
    CL_ERRORS_Error(cl_main_eError_eARGC, "No enough command line arguments.");
    CL_MAIN_PrintUsage();
  }

  if(CL_ERRORS_GetCurrentErrorEnum() == cl_main_eError_eOK) {
    CL_ARGS_ProcessArguments(argc, &argv);
  }

  CL_ERRORS_ProcessErrors();
  exit(EXIT_SUCCESS);
}
