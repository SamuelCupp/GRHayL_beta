#ifndef FLUX_SOURCE_H_
#define FLUX_SOURCE_H_

#include "ghl.h"


static const double TINYDOUBLE = 1e-100;

static const double SQRT_4_PI = 3.544907701811032054596334966682290365L;

#ifdef __cplusplus
extern "C" {
#endif

void ghl_calculate_characteristic_speed_dirn0(const ghl_primitive_quantities *restrict prims_r,
      const ghl_primitive_quantities *restrict prims_l,
      const ghl_eos_parameters *restrict eos,
      const ghl_metric_quantities *restrict metric_face,
      double *cmin_dirn0,
      double *cmax_dirn0);

void ghl_calculate_characteristic_speed_dirn1(const ghl_primitive_quantities *restrict prims_r,
      const ghl_primitive_quantities *restrict prims_l,
      const ghl_eos_parameters *restrict eos,
      const ghl_metric_quantities *restrict metric_face,
      double *cmin_dirn1,
      double *cmax_dirn1);

void ghl_calculate_characteristic_speed_dirn2(const ghl_primitive_quantities *restrict prims_r,
      const ghl_primitive_quantities *restrict prims_l,
      const ghl_eos_parameters *restrict eos,
      const ghl_metric_quantities *restrict metric_face,
      double *cmin_dirn2,
      double *cmax_dirn2);

void ghl_calculate_tau_tilde_source_term_extrinsic_curv(
      const ghl_primitive_quantities *restrict prims,
      struct ghl_eos_parameters const *restrict eos,
      const ghl_metric_quantities *restrict metric,
      const ghl_extrinsic_curvature *restrict curv,
      ghl_conservative_quantities *restrict cons);

void ghl_calculate_source_terms_dirn0(
      const ghl_primitive_quantities *restrict prims,
      struct ghl_eos_parameters const *restrict eos,
      const ghl_metric_quantities *restrict metric,
      const ghl_metric_quantities *restrict metric_derivs,
      ghl_conservative_quantities *restrict cons);

void ghl_calculate_source_terms_dirn1(
      const ghl_primitive_quantities *restrict prims,
      struct ghl_eos_parameters const *restrict eos,
      const ghl_metric_quantities *restrict metric,
      const ghl_metric_quantities *restrict metric_derivs,
      ghl_conservative_quantities *restrict cons);

void ghl_calculate_source_terms_dirn2(
      const ghl_primitive_quantities *restrict prims,
      struct ghl_eos_parameters const *restrict eos,
      const ghl_metric_quantities *restrict metric,
      const ghl_metric_quantities *restrict metric_derivs,
      ghl_conservative_quantities *restrict cons);

void ghl_calculate_HLLE_fluxes_dirn0_hybrid(const primitive_quantities *restrict prims_r,
      const primitive_quantities *restrict prims_l,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric_face,
      const double cmin_dirn0,
      const double cmax_dirn0,
      conservative_quantities *restrict cons);

void ghl_calculate_HLLE_fluxes_dirn1_hybrid(const primitive_quantities *restrict prims_r,
      const primitive_quantities *restrict prims_l,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric_face,
      const double cmin_dirn01,
      const double cmax_dirn01,
      conservative_quantities *restrict cons);

void ghl_calculate_HLLE_fluxes_dirn2_hybrid(const primitive_quantities *restrict prims_r,
      const primitive_quantities *restrict prims_l,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric_face,
      const double cmin_dirn2,
      const double cmax_dirn2,
      conservative_quantities *restrict cons);

void ghl_calculate_HLLE_fluxes_dirn0_hybrid_entropy(const primitive_quantities *restrict prims_r,
      const primitive_quantities *restrict prims_l,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric_face,
      const double cmin_dirn0,
      const double cmax_dirn0,
      conservative_quantities *restrict cons);

void ghl_calculate_HLLE_fluxes_dirn1_hybrid_entropy(const primitive_quantities *restrict prims_r,
      const primitive_quantities *restrict prims_l,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric_face,
      const double cmin_dirn01,
      const double cmax_dirn01,
      conservative_quantities *restrict cons);

void ghl_calculate_HLLE_fluxes_dirn2_hybrid_entropy(const primitive_quantities *restrict prims_r,
      const primitive_quantities *restrict prims_l,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric_face,
      const double cmin_dirn2,
      const double cmax_dirn2,
      conservative_quantities *restrict cons);

void ghl_calculate_HLLE_fluxes_dirn0_tabulated(const primitive_quantities *restrict prims_r,
      const primitive_quantities *restrict prims_l,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric_face,
      const double cmin_dirn0,
      const double cmax_dirn0,
      conservative_quantities *restrict cons);

void ghl_calculate_HLLE_fluxes_dirn1_tabulated(const primitive_quantities *restrict prims_r,
      const primitive_quantities *restrict prims_l,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric_face,
      const double cmin_dirn01,
      const double cmax_dirn01,
      conservative_quantities *restrict cons);

void ghl_calculate_HLLE_fluxes_dirn2_tabulated(const primitive_quantities *restrict prims_r,
      const primitive_quantities *restrict prims_l,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric_face,
      const double cmin_dirn2,
      const double cmax_dirn2,
      conservative_quantities *restrict cons);

void ghl_calculate_HLLE_fluxes_dirn0_tabulated_entropy(const primitive_quantities *restrict prims_r,
      const primitive_quantities *restrict prims_l,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric_face,
      const double cmin_dirn0,
      const double cmax_dirn0,
      conservative_quantities *restrict cons);

void ghl_calculate_HLLE_fluxes_dirn1_tabulated_entropy(const primitive_quantities *restrict prims_r,
      const primitive_quantities *restrict prims_l,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric_face,
      const double cmin_dirn01,
      const double cmax_dirn01,
      conservative_quantities *restrict cons);

void ghl_calculate_HLLE_fluxes_dirn2_tabulated_entropy(const primitive_quantities *restrict prims_r,
      const primitive_quantities *restrict prims_l,
      const eos_parameters *restrict eos,
      const metric_quantities *restrict metric_face,
      const double cmin_dirn2,
      const double cmax_dirn2,
      conservative_quantities *restrict cons);

#ifdef __cplusplus
}
#endif

#endif // FLUX_SOURCE_H_
