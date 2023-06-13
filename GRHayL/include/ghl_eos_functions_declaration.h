#ifndef GHL_EOS_FUNCTIONS_DECLARATION_H_
#define GHL_EOS_FUNCTIONS_DECLARATION_H_

void (*ghl_compute_h_and_cs2)(
      eos_parameters const *restrict eos,
      primitive_quantities const *restrict prims,
      double *restrict h,
      double *restrict cs2);


int  (*ghl_hybrid_find_polytropic_index)(
      const eos_parameters *restrict eos,
      const double rho_in);

void (*ghl_hybrid_get_K_and_Gamma)(
      const eos_parameters *restrict eos,
      const double rho_in,
      double *restrict K,
      double *restrict Gamma);

void (*ghl_hybrid_set_K_ppoly_and_eps_integ_consts)(eos_parameters *restrict eos);

void (*ghl_hybrid_compute_P_cold)(
      const eos_parameters *restrict eos,
      const double rho_in,
      double *restrict P_cold_ptr);

void (*ghl_hybrid_compute_P_cold_and_eps_cold)(
      const eos_parameters *restrict eos,
      const double rho_in,
      double *restrict P_cold_ptr,
      double *restrict eps_cold_ptr);

void (*ghl_hybrid_compute_entropy_function)(
      const eos_parameters *restrict eos,
      const double rho,
      const double P,
      double *restrict S );

  // Function prototypes
void (*ghl_tabulated_read_table_set_EOS_params)(
      const char *nuceos_table_name,
      eos_parameters *restrict eos);

void (*ghl_tabulated_free_memory)(eos_parameters *restrict eos);

void (*ghl_tabulated_compute_P_from_T)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double T,
      double *restrict P);

void (*ghl_tabulated_compute_eps_from_T)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double T,
      double *restrict eps);

void (*ghl_tabulated_compute_P_eps_from_T)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double T,
      double *restrict P,
      double *restrict eps);

void (*ghl_tabulated_compute_P_eps_S_from_T)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double T,
      double *restrict P,
      double *restrict eps,
      double *restrict S);

void (*ghl_tabulated_compute_P_eps_cs2_from_T)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double T,
      double *restrict P,
      double *restrict eps,
      double *restrict cs2);

void (*ghl_tabulated_compute_P_eps_S_cs2_from_T)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double T,
      double *restrict P,
      double *restrict eps,
      double *restrict S,
      double *restrict cs2);

void (*ghl_tabulated_compute_P_eps_depsdT_from_T)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double T,
      double *restrict P,
      double *restrict eps,
      double *restrict depsdT);

void (*ghl_tabulated_compute_P_eps_muhat_mue_mup_mun_from_T)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double T,
      double *restrict P,
      double *restrict eps,
      double *restrict muhat,
      double *restrict mu_e,
      double *restrict mu_p,
      double *restrict mu_n);

void (*ghl_tabulated_compute_muhat_mue_mup_mun_Xn_Xp_from_T)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double T,
      double *restrict muhat,
      double *restrict mu_e,
      double *restrict mu_p,
      double *restrict mu_n,
      double *restrict X_n,
      double *restrict X_p);

void (*ghl_tabulated_compute_T_from_eps)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double eps,
      double *restrict T);

void (*ghl_tabulated_compute_P_T_from_eps)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double eps,
      double *restrict P,
      double *restrict T);

void (*ghl_tabulated_compute_P_cs2_T_from_eps)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double eps,
      double *restrict P,
      double *restrict cs2,
      double *restrict T);

void (*ghl_tabulated_compute_eps_cs2_T_from_P)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double P,
      double *restrict eps,
      double *restrict cs2,
      double *restrict T);

void (*ghl_tabulated_compute_P_T_from_S)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double S,
      double *restrict P,
      double *restrict T);

void (*ghl_tabulated_compute_P_S_depsdT_T_from_eps)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double eps,
      double *restrict P,
      double *restrict S,
      double *restrict depsdT,
      double *restrict T);

void (*ghl_tabulated_compute_eps_S_T_from_P)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double P,
      double *restrict eps,
      double *restrict S,
      double *restrict T);

void (*ghl_tabulated_compute_P_eps_T_from_S)(
      const eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double S,
      double *restrict P,
      double *restrict eps,
      double *restrict T);

#endif // GHL_EOS_FUNCTIONS_DECLARATION_H_
