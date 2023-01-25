#include "unit_tests.h"

/*
  In the function validate_conservatives, the conservatives are validated
  individually. In the case of failure, the code errors out and lists all
  the components that failed to be within the error bars of the perturbed
  version of the code.
*/

void validate_conservatives(
                     const bool evolve_entropy,
                     const conservative_quantities *restrict cons,
                     const conservative_quantities *restrict cons_trusted,
                     const conservative_quantities *restrict cons_pert) {

  char fail_msg[100] = "Test has failed!\n The conservative variable(s) which failed are ";
  int test_fail = 0;
  if( relative_error(cons_trusted->rho, cons->rho) > tolerance*relative_error(cons_trusted->rho, cons_pert->rho) ) {
    sprintf(fail_msg, "%.90s rho_star", fail_msg);
    test_fail = 1;
  }

  if( relative_error(cons_trusted->tau, cons->tau) > tolerance*relative_error(cons_trusted->tau, cons_pert->tau) ) {
    sprintf(fail_msg, "%.90s tau", fail_msg);
    test_fail = 1;
  }

  if( relative_error(cons_trusted->S_x, cons->S_x) > tolerance*relative_error(cons_trusted->S_x, cons_pert->S_x) ) {
    sprintf(fail_msg, "%.90s S_x", fail_msg);
    test_fail = 1;
  }

  if( relative_error(cons_trusted->S_y, cons->S_y) > tolerance*relative_error(cons_trusted->S_y, cons_pert->S_y) ) {
    sprintf(fail_msg, "%.90s S_y", fail_msg);
    test_fail = 1;
  }

  if( relative_error(cons_trusted->S_z, cons->S_z) > tolerance*relative_error(cons_trusted->S_z, cons_pert->S_z) ) {
    sprintf(fail_msg, "%.90s S_z", fail_msg);
    test_fail = 1;
  }

  if(evolve_entropy)
    if( relative_error(cons_trusted->entropy, cons->entropy) > tolerance*relative_error(cons_trusted->entropy, cons_pert->entropy) ) {
      sprintf(fail_msg, "%.90s entropy", fail_msg);
      test_fail = 1;
    }

  if(test_fail) {
    grhayl_error("%.100s\n", fail_msg);
  }
  return;
}
