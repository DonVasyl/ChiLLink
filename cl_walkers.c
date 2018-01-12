/*

  ChiLLink - portable OMF-to-MZ linker
  Copyright (c) DonVasyl (i INNE GWIAZDY), 2017 - 2018

*/

#include "include/cl_common.h"
#include "include/cl_errors.h"
#include "include/cl_main.h"

static cl_walkers_child_t *CL_WALKERS_getNewChild(cl_walkers_child_t **pStart,
						  size_t pSize) {
  if(*pStart == NULL) {
    *pStart = malloc(pSize);
    if(*pStart == NULL) {
      CL_ERRORS_Error(cl_main_eError_eMemory,
		      "WALKERS: Memory allocation error (walker structure, root)!");
      return NULL;
    }
    memset(*pStart, 0x00, pSize);
    return *pStart;
  }

  cl_walkers_child_t *lChild = *pStart;
  while(lChild->walker.next != NULL) {
    lChild = (cl_walkers_child_t *)lChild->walker.next;
  }

  lChild->walker.next = malloc(sizeof(cl_omfreader_name_t));
  if(lChild->walker.next == NULL) {
    CL_ERRORS_Error(cl_main_eError_eMemory,
		    "WALKERS: Memory allocation error (walker structure)!");
    return NULL;
  }
  memset(lChild->walker.next, 0x00, pSize);
  return (cl_walkers_child_t *)lChild->walker.next;
}

cl_walkers_child_t *CL_WALKERS_AddChild(cl_walkers_child_t **pStart,
					size_t pDelta) {
  cl_walkers_child_t *lNewChild = CL_WALKERS_getNewChild(pStart, pDelta);
  lNewChild->walker.next = NULL;

  return lNewChild;
}

cl_walkers_child_t *CL_WALKERS_Init(cl_walkers_child_t *pStart) {
  cl_walkers_child_t **lPtr = malloc(sizeof(cl_walkers_child_t *));
  if(lPtr == NULL) {
      CL_ERRORS_Error(cl_main_eError_eMemory,
		      "WALKERS: Memory allocation error (Init)!");
      return NULL;
  }
  *lPtr = pStart;
  return *lPtr;
}

void CL_WALKERS_Step(cl_walkers_child_t **pPtr) {
  *pPtr = (cl_walkers_child_t *)(*pPtr)->walker.next;
}

void CL_WALKERS_Kill(cl_walkers_child_t **pPtr) {
  free(pPtr);
}
