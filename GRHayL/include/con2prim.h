#ifndef CON2PRIM_H_
#define CON2PRIM_H_

#include "GRHayL.h"

//------------- Con2Prim struct --------------------
/*
   The struct con2prim_diagnostics contains variables for error-checking and
   diagnostic feedback. The struct elements are detailed below:

 --TODO

TODO: consider changing failure_checker to be bitwise; failure modes are currently
      1: atmosphere reset when rho_star < 0
      10: reseting P when P<P_min in enforce_...
      100: reseting P when P>P_max in enforce_...
      1k: Limiting velocity u~ after C2P/Font Fix or v in enforce_...
      10k: Font Fix was applied
      100k: Both C2P and Font Fix failed
      1M: tau~ was reset in apply_inequality_fixes
      10M: S~ was reset in apply_inequality_fixes via the first case
      100M: S~ was reset in apply_inequality_fixes via the second case
For bitwise, would become 1, 2, 4, 8, 16, 32. 64, 128, and 256
https://www.tutorialspoint.com/cprogramming/c_bitwise_operators.htm
*/

typedef struct con2prim_diagnostics {
  bool c2p_failed;
  int failures;
  int failure_checker;
  int font_fix;
  int speed_limited;
  int which_routine;
  int backup[3];
  int nan_found;
  int c2p_fail_flag;
  double error_int_numer;
  double error_int_denom;
  int n_iter;
} con2prim_diagnostics;

typedef struct palenzuela_quantities {
  double q, r, s, t, D, Y_e, S;
} palenzuela_quantities;

//--------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

void reset_prims_to_atmosphere(
      const eos_parameters *restrict eos,
      primitive_quantities *restrict prims);

//--------- Initialization routines ----------------

void initialize_diagnostics(con2prim_diagnostics *restrict diagnostics);

//----------- Pre/Post-C2P routines ----------------

void apply_inequality_fixes(
      const GRHayL_parameters *restrict params,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric,
      const primitive_quantities *restrict prims,
      conservative_quantities *restrict cons,
      con2prim_diagnostics *restrict diagnostics);

void undensitize_conservatives(
      const metric_quantities *restrict metric,
      const conservative_quantities *restrict cons,
      conservative_quantities *restrict cons_undens);

void guess_primitives(
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric,
      const conservative_quantities *restrict cons,
      primitive_quantities *restrict prims);

void enforce_primitive_limits_and_compute_u0(
      const GRHayL_parameters *restrict params,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric,
      primitive_quantities *restrict prims,
      int *restrict speed_limit);

void compute_conservs_and_Tmunu(
      const GRHayL_parameters *restrict params,
      const metric_quantities *restrict metric,
      const primitive_quantities *restrict prims,
      conservative_quantities *restrict cons,
      stress_energy *restrict Tmunu);

//--------------------------------------------------

//---------- Primary Con2Prim routines -------------

int Hybrid_Multi_Method(
      const GRHayL_parameters *restrict params,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric,
      const conservative_quantities *restrict cons,
      primitive_quantities *restrict prims,
      con2prim_diagnostics *restrict diagnostics);

int Tabulated_Multi_Method(
      const GRHayL_parameters *restrict params,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric,
      const conservative_quantities *restrict cons,
      primitive_quantities *restrict prim,
      con2prim_diagnostics *restrict diagnostics);

int Hybrid_Noble2D(
      const GRHayL_parameters *restrict params,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric,
      const conservative_quantities *restrict cons,
      primitive_quantities *restrict prim,
      con2prim_diagnostics *restrict diagnostics);

int Tabulated_Palenzuela1D_energy(
      const GRHayL_parameters *restrict params,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric,
      const conservative_quantities *restrict cons,
      primitive_quantities *restrict prim,
      con2prim_diagnostics *restrict diagnostics);

int Tabulated_Palenzuela1D_entropy(
      const GRHayL_parameters *restrict params,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric,
      const conservative_quantities *restrict cons,
      primitive_quantities *restrict prim,
      con2prim_diagnostics *restrict diagnostics);

int Tabulated_Newman1D_energy(
      const GRHayL_parameters *restrict params,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric,
      const conservative_quantities *restrict cons,
      primitive_quantities *restrict prim,
      con2prim_diagnostics *restrict diagnostics);

int Tabulated_Newman1D_entropy(
      const GRHayL_parameters *restrict params,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric,
      const conservative_quantities *restrict cons,
      primitive_quantities *restrict prim,
      con2prim_diagnostics *restrict diagnostics);

//--------------------------------------------------

//-------------- Font Fix routines -----------------

int font_fix(
      const GRHayL_parameters *restrict params,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric,
      const conservative_quantities *restrict cons_undens,
      primitive_quantities *restrict prims,
      con2prim_diagnostics *restrict diagnostics);

int font_fix_hybrid_EOS(
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric,
      const conservative_quantities *restrict cons_undens,
      const primitive_quantities *restrict prims,
      double *restrict u_x_ptr, double *restrict u_y_ptr,
      double *restrict u_z_ptr );

//TODO: this was formerly inline. Include inside font_fix_hybrid_EOS directly?
int font_fix_rhob_loop(
      const eos_parameters *restrict eos,
      const int maxits, const double tol, const double W_in,
      const double Sf2_in, const double Psim6, const double sdots,
      const double BbardotS2, const double B2bar,
      const conservative_quantities *restrict cons,
      const double rhob_in, double *restrict rhob_out_ptr );

//--------------------------------------------------

//------------ Auxiliary Functions -----------------

void limit_utilde_and_compute_v(
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric,
      double *restrict utcon1_ptr,
      double *restrict utcon2_ptr,
      double *restrict utcon3_ptr,
      primitive_quantities *restrict prims,
      int *restrict speed_limit);

#ifdef __cplusplus
}
#endif

double compute_Bsq_from_Bup(
      const metric_quantities *restrict metric,
      const double *restrict Bup );

#endif // CON2PRIM_H
