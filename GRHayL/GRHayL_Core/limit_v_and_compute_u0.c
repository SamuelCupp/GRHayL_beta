#include "con2prim.h"

/* Function    : limit_v_and_compute_u0()
 * Description : Applies speed limit to v^i and computes u^0
 *
 * Inputs      : eos            - eos_parameters struct with data for the
 *                                EOS of the simulation
 *             : metric         - metric_quantities struct with data for
 *                                the gridpoint of interest
 *             : prims          - primitive_quantities struct to be speed limited
 *
 * Outputs     : prims          - returns velocity-limited prims->v^i and t
 *                                component of 4-velocity prims->u0
 *             : speed_limit    - tracks if the velocity was limited
 *
 */

void limit_v_and_compute_u0(
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric,
      primitive_quantities *restrict prims,
      int *restrict speed_limit) {

  // Derivation of first equation:
  // \gamma_{ij} (v^i + \beta^i)(v^j + \beta^j)/(\alpha)^2
  //   = \gamma_{ij} 1/(u^0)^2 ( \gamma^{ik} u_k \gamma^{jl} u_l /(\alpha)^2 <- Using Eq. 53 of arXiv:astro-ph/0503420
  //   = 1/(u^0 \alpha)^2 u_j u_l \gamma^{jl}  <- Since \gamma_{ij} \gamma^{ik} = \delta^k_j
  //   = 1/(u^0 \alpha)^2 ( (u^0 \alpha)^2 - 1 ) <- Using Eq. 56 of arXiv:astro-ph/0503420
  //   = 1 - 1/(u^0 \alpha)^2 <= 1
  double one_minus_one_over_alpha_u0_squared = (metric->adm_gxx * SQR(prims->vx + metric->betax) +
                                                2.0*metric->adm_gxy*(prims->vx + metric->betax)*(prims->vy + metric->betay) +
                                                2.0*metric->adm_gxz*(prims->vx + metric->betax)*(prims->vz + metric->betaz) +
                                                metric->adm_gyy * SQR(prims->vy + metric->betay) +
                                                2.0*metric->adm_gyz*(prims->vy + metric->betay)*(prims->vz + metric->betaz) +
                                                metric->adm_gzz * SQR(prims->vz + metric->betaz) )*metric->lapseinv2;

  /*** Limit velocity to GAMMA_SPEED_LIMIT ***/
  const double one_minus_one_over_W_max_squared = 1.0-1.0/SQR(eos->W_max); // 1 - W_max^{-2}
  if(one_minus_one_over_alpha_u0_squared > one_minus_one_over_W_max_squared) {
    const double correction_fac = sqrt(one_minus_one_over_W_max_squared/one_minus_one_over_alpha_u0_squared);
    prims->vx = (prims->vx + metric->betax)*correction_fac - metric->betax;
    prims->vy = (prims->vy + metric->betay)*correction_fac - metric->betay;
    prims->vz = (prims->vz + metric->betaz)*correction_fac - metric->betaz;
    one_minus_one_over_alpha_u0_squared = one_minus_one_over_W_max_squared;
    (*speed_limit)++;
  }

  // A = 1.0-one_minus_one_over_alpha_u0_squared = 1-(1-1/(al u0)^2) = 1/(al u0)^2
  // 1/sqrt(A) = al u0
  //double alpha_u0_minus_one = 1.0/sqrt(1.0-one_minus_one_over_alpha_u0_squared)-1.0;
  //u0_out          = (alpha_u0_minus_one + 1.0)*metric.lapseinv;
  const double alpha_u0 = 1.0/sqrt(1.0-one_minus_one_over_alpha_u0_squared);
  prims->u0 = alpha_u0*metric->lapseinv;
  if(isnan(prims->u0)) {
    // Leo asks: shouldn't this be an error?
    grhayl_error("*********************************************\n"
                 "Found nan while computing u^{0}\nMetric: %e %e %e %e %e %e\n"
                 "Lapse/shift: %e (=1/%e) / %e %e %e\nVelocities : %e %e %e\n"
                 "*********************************************\n",
                 metric->adm_gxx, metric->adm_gxy, metric->adm_gxz,
                 metric->adm_gyy, metric->adm_gyz, metric->adm_gzz,
                 metric->lapse, metric->lapseinv,
                 metric->betax, metric->betay, metric->betaz,
                 prims->vx, prims->vx, prims->vz);
  }
}
