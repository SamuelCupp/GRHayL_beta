#include "neutrinos.h"

static double EnsureFinite(const double x) {
  if(robust_isfinite(x))
    return x;
  else
    return 1e-15;
}

/*
 * (c) Leo Werneck
 * Compute GRMHD source terms following Ruffert et al. (1996)
 * https://adsabs.harvard.edu/pdf/1996A%26A...311..532R
 */
void NRPyLeakage_compute_neutrino_opacities_and_GRMHD_source_terms(
      const ghl_eos_parameters *restrict eos,
      const double rho,
      const double Y_e,
      const double T,
      const ghl_neutrino_optical_depths *restrict tau,
      ghl_neutrino_opacities *restrict kappa,
      double *restrict R_source,
      double *restrict Q_source) {


  // Step 1: Get chemical potentials and mass
  //         fractions using the EOS
  double muhat, mu_e, mu_p, mu_n, X_n, X_p;
  ghl_tabulated_compute_muhat_mue_mup_mun_Xn_Xp_from_T(eos, rho, Y_e, T, &muhat, &mu_e, &mu_p, &mu_n, &X_n, &X_p);

  // Step 2: Compute rho in cgs units
  const double rho_cgs = rho * NRPyLeakage_units_geom_to_cgs_D;

  // Step 3: Compute Y_{pn} and Y_{np}
  const double Y_p = Y_e;
  const double Y_n = 1-Y_e;
  const double exp_metahat = exp(-muhat/T);
  // Step 3.a: Compute Y_{np}
  double Y_np = (Y_e < 0.5) ? (2.0*Y_e-1.0)/(exp_metahat-1.0) : Y_n;

  // Step 3.b: Compute Y_{pn}
  double Y_pn = (Y_e > 0.5) ? exp_metahat*(2.0*Y_e-1.0)/(exp_metahat-1.0) : Y_p;

  // Step 3.c: Make sure both Y_np and Y_pn are non-negative
  if(Y_np < 0.0) Y_np = Y_n;
  if(Y_pn < 0.0) Y_pn = Y_p;

  // Step 4: Compute the source terms
  //         Note: The code below is generated by NRPy+
  const double tmp_0 = (1.0/(T));
  const double tmp_1 = mu_e*tmp_0;
  const double tmp_2 = NRPyLeakage_Fermi_Dirac_integrals(4, tmp_1);
  const double tmp_3 = NRPyLeakage_Fermi_Dirac_integrals(5, tmp_1)/tmp_2;
  const double tmp_4 = exp(-tau->nue[0]);
  const double tmp_6 = NRPyLeakage_eta_nue_0*tmp_4 + (1 - tmp_4)*(-muhat*tmp_0 + tmp_1);
  const double tmp_7 = ((NRPyLeakage_alpha)*(NRPyLeakage_alpha));
  const double tmp_8 = M_PI/NRPyLeakage_hc3;
  const double tmp_10 = 8*NRPyLeakage_N_A*NRPyLeakage_beta*((T)*(T)*(T)*(T)*(T))*rho_cgs*tmp_8*((3.0/8.0)*tmp_7 + 1.0/8.0);
  const double tmp_11 = NRPyLeakage_enable_beta_nue*EnsureFinite(Y_pn*tmp_10*tmp_2/(exp(-tmp_3 + tmp_6) + 1));
  const double tmp_12 = NRPyLeakage_enable_brems_nui_anui*EnsureFinite(NRPyLeakage_Brems_C1*NRPyLeakage_Brems_zeta*pow(T, 4.5)*rho_cgs*(((X_n)*(X_n)) + (28.0/3.0)*X_n*X_p + ((X_p)*(X_p))));
  const double tmp_13 = exp(-tau->anue[0]);
  const double tmp_15 = NRPyLeakage_eta_anue_0*tmp_13 + (1 - tmp_13)*(muhat*tmp_0 - tmp_1);
  const double tmp_16 = NRPyLeakage_Fermi_Dirac_integrals(3, tmp_1);
  const double tmp_17 = (1.0/(tmp_16));
  const double tmp_18 = NRPyLeakage_Fermi_Dirac_integrals(4, -tmp_1);
  const double tmp_19 = NRPyLeakage_Fermi_Dirac_integrals(3, -tmp_1);
  const double tmp_20 = (1.0/(tmp_19));
  const double tmp_21 = -1.0/2.0*tmp_17*tmp_2 - 1.0/2.0*tmp_18*tmp_20;
  const double tmp_22 = ((M_PI)*(M_PI));
  const double tmp_24 = (1.0/((NRPyLeakage_hc3)*(NRPyLeakage_hc3)));
  const double tmp_25 = tmp_16*tmp_22*tmp_24;
  const double tmp_26 = pow(T, 8);
  const double tmp_28 = (16.0/9.0)*NRPyLeakage_beta*tmp_19*tmp_25*tmp_26;
  const double tmp_29 = NRPyLeakage_enable_pair_nue_anue*EnsureFinite(NRPyLeakage_C1pC2_nue_anue*tmp_28/((exp(tmp_15 + tmp_21) + 1)*(exp(tmp_21 + tmp_6) + 1)));
  const double tmp_31 = (1.0/3.0)*tmp_22 + ((mu_e)*(mu_e))/((T)*(T));
  const double tmp_32 = NRPyLeakage_gamma_0*sqrt(tmp_31);
  const double tmp_34 = ((NRPyLeakage_gamma_0)*(NRPyLeakage_gamma_0))*tmp_31/(tmp_32 + 1);
  const double tmp_35 = -1.0/2.0*tmp_34 - 1;
  const double tmp_36 = (1.0/3.0)*((M_PI)*(M_PI)*(M_PI))*NRPyLeakage_beta*pow(NRPyLeakage_gamma_0, 6)*tmp_24*tmp_26*((tmp_31)*(tmp_31)*(tmp_31))*(tmp_32 + 1)*exp(-tmp_32)/NRPyLeakage_alpha_fs;
  const double tmp_37 = NRPyLeakage_enable_plasmon_nue_anue*EnsureFinite(((NRPyLeakage_C_V)*(NRPyLeakage_C_V))*tmp_36/((exp(tmp_15 + tmp_35) + 1)*(exp(tmp_35 + tmp_6) + 1)));
  const double tmp_38 = tmp_12 + tmp_29 + tmp_37;
  const double tmp_41 = (1 - Y_e)*((5.0/24.0)*tmp_7 + 1.0/24.0)/((2.0/3.0)*MAX(mu_n*tmp_0, 0) + 1);
  const double tmp_42 = NRPyLeakage_N_A*NRPyLeakage_sigma_0*((T)*(T))*rho_cgs/((NRPyLeakage_m_e_c2)*(NRPyLeakage_m_e_c2));
  const double tmp_43 = NRPyLeakage_Fermi_Dirac_integrals(2, tmp_6);
  const double tmp_44 = NRPyLeakage_Fermi_Dirac_integrals(4, tmp_6);
  const double tmp_45 = tmp_44/tmp_43;
  const double tmp_47 = ((NRPyLeakage_C_V - 1)*(NRPyLeakage_C_V - 1));
  const double tmp_48 = Y_e*((1.0/6.0)*tmp_47 + (5.0/24.0)*tmp_7)/((2.0/3.0)*MAX(mu_p*tmp_0, 0) + 1);
  const double tmp_49 = tmp_42*tmp_48;
  const double tmp_50 = (3.0/4.0)*tmp_7 + 1.0/4.0;
  const double tmp_51 = NRPyLeakage_Fermi_Dirac_integrals(5, tmp_6);
  const double tmp_52 = Y_np*tmp_50/(exp(tmp_1 - tmp_51/tmp_44) + 1);
  const double tmp_53 = EnsureFinite(tmp_45*tmp_49) + EnsureFinite(tmp_41*tmp_42*tmp_45) + EnsureFinite(tmp_42*tmp_45*tmp_52);
  const double tmp_55 = 4*((T)*(T)*(T))*tmp_8;
  const double tmp_56 = 6/NRPyLeakage_units_geom_to_cgs_L;
  const double tmp_57 = NRPyLeakage_Fermi_Dirac_integrals(5, -tmp_1)/tmp_18;
  const double tmp_58 = NRPyLeakage_enable_beta_anue*EnsureFinite(Y_np*tmp_10*tmp_18/(exp(tmp_15 - tmp_57) + 1));
  const double tmp_60 = NRPyLeakage_Fermi_Dirac_integrals(2, tmp_15);
  const double tmp_61 = NRPyLeakage_Fermi_Dirac_integrals(4, tmp_15);
  const double tmp_62 = tmp_42*tmp_61/tmp_60;
  const double tmp_63 = NRPyLeakage_Fermi_Dirac_integrals(5, tmp_15);
  const double tmp_64 = Y_pn*tmp_50/(exp(-tmp_1 - tmp_63/tmp_61) + 1);
  const double tmp_65 = EnsureFinite(tmp_41*tmp_62) + EnsureFinite(tmp_48*tmp_62) + EnsureFinite(tmp_62*tmp_64);
  const double tmp_66 = EnsureFinite(NRPyLeakage_Brems_C2*T*tmp_12/NRPyLeakage_Brems_C1);
  const double tmp_67 = 32*pow(T, 9);
  const double tmp_68 = (1.0/64.0)*((NRPyLeakage_hc3)*(NRPyLeakage_hc3))*tmp_17*tmp_20*(tmp_18*tmp_25*tmp_67 + tmp_19*tmp_2*tmp_22*tmp_24*tmp_67)/(tmp_22*tmp_26);
  const double tmp_69 = (1.0/2.0)*T*(tmp_34 + 2);
  const double tmp_70 = NRPyLeakage_Fermi_Dirac_integrals(3, 0);
  const double tmp_71 = NRPyLeakage_Fermi_Dirac_integrals(5, 0)/tmp_70;
  const double tmp_73 = EnsureFinite(tmp_49*tmp_71) + EnsureFinite(tmp_41*tmp_42*tmp_71);
  const double tmp_74 = 4*((T)*(T)*(T)*(T))*tmp_8;
  const double tmp_75 = tmp_66 + EnsureFinite(tmp_29*tmp_68) + EnsureFinite(tmp_37*tmp_69);
  const double tmp_76 = tmp_75 + EnsureFinite(T*tmp_11*tmp_3);
  const double tmp_78 = NRPyLeakage_Fermi_Dirac_integrals(3, tmp_6);
  const double tmp_79 = tmp_51/tmp_78;
  const double tmp_81 = EnsureFinite(tmp_49*tmp_79) + EnsureFinite(tmp_41*tmp_42*tmp_79) + EnsureFinite(tmp_42*tmp_52*tmp_79);
  const double tmp_82 = tmp_75 + EnsureFinite(T*tmp_57*tmp_58);
  const double tmp_83 = NRPyLeakage_Fermi_Dirac_integrals(3, tmp_15);
  const double tmp_84 = tmp_63/tmp_83;
  const double tmp_86 = EnsureFinite(tmp_49*tmp_84) + EnsureFinite(tmp_41*tmp_42*tmp_84) + EnsureFinite(tmp_42*tmp_64*tmp_84);
  const double tmp_87 = NRPyLeakage_Fermi_Dirac_integrals(4, 0)/NRPyLeakage_Fermi_Dirac_integrals(2, 0);
  *R_source = NRPyLeakage_amu*NRPyLeakage_units_cgs_to_geom_R*(-(tmp_11 + tmp_38)/(((tau->nue[0])*(tau->nue[0]))*tmp_56*(tmp_11 + tmp_38)/(tmp_53*MAX(tmp_43*tmp_55, 1.0000000000000001e-15)) + 1) + (tmp_38 + tmp_58)/(((tau->anue[0])*(tau->anue[0]))*tmp_56*(tmp_38 + tmp_58)/(tmp_65*MAX(tmp_55*tmp_60, 1.0000000000000001e-15)) + 1));
  *Q_source = NRPyLeakage_units_cgs_to_geom_Q*(-tmp_76/(((tau->nue[1])*(tau->nue[1]))*tmp_56*tmp_76/(tmp_81*MAX(tmp_74*tmp_78, 1.0000000000000001e-15)) + 1) - tmp_82/(((tau->anue[1])*(tau->anue[1]))*tmp_56*tmp_82/(tmp_86*MAX(tmp_74*tmp_83, 1.0000000000000001e-15)) + 1) - 4*(tmp_66 + EnsureFinite(NRPyLeakage_enable_pair_nux_anux*tmp_68*EnsureFinite(NRPyLeakage_C1pC2_nux_anux*tmp_28/((exp(tmp_21) + 1)*(exp(tmp_21) + 1)))) + EnsureFinite(NRPyLeakage_enable_plasmon_nux_anux*tmp_69*EnsureFinite(tmp_36*tmp_47/((exp(tmp_35) + 1)*(exp(tmp_35) + 1)))))/(((tau->nux[1])*(tau->nux[1]))*tmp_56*tmp_76/(tmp_73*MAX(tmp_70*tmp_74, 1.0000000000000001e-15)) + 1));
  kappa->nue[0] = NRPyLeakage_units_geom_to_cgs_L*tmp_53;
  kappa->nue[1] = NRPyLeakage_units_geom_to_cgs_L*tmp_81;
  kappa->anue[0] = NRPyLeakage_units_geom_to_cgs_L*tmp_65;
  kappa->anue[1] = NRPyLeakage_units_geom_to_cgs_L*tmp_86;
  kappa->nux[0] = NRPyLeakage_units_geom_to_cgs_L*(EnsureFinite(tmp_49*tmp_87) + EnsureFinite(tmp_41*tmp_42*tmp_87));
  kappa->nux[1] = NRPyLeakage_units_geom_to_cgs_L*tmp_73;
}
