/*

  ChiLLink - portable OMF-to-MZ linker
  Copyright (c) DonVasyl (i INNE GWIAZDY), 2017 - 2018

*/

extern cl_walkers_child_t *CL_WALKERS_AddChild(cl_walkers_child_t **pStart,
					       size_t pDelta);
extern cl_walkers_child_t *CL_WALKERS_Init(cl_walkers_child_t *pStart);
extern void CL_WALKERS_Step(cl_walkers_child_t **pPtr);
extern void CL_WALKERS_Kill(cl_walkers_child_t **pPtr);

