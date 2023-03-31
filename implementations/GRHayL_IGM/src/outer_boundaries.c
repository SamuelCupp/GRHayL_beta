/*******************************************************
 * Outer boundaries are handled as follows:
 * (-1) Update RHS quantities, leave RHS quantities zero on all outer ghostzones (including outer AMR refinement, processor, and outer boundaries)
 * ( 0) Let MoL update all evolution variables
 * ( 1) Apply outer boundary conditions (BCs) on A_{\mu}
 * ( 2) Compute B^i from A_i everywhere, synchronize B^i
 * ( 3) Call con2prim to get primitives on interior pts
 * ( 4) Apply outer BCs on {P,rho_b,vx,vy,vz}.
 * ( 5) (optional) set conservatives on outer boundary.
 *******************************************************/

#include "cctk.h"
#include "cctk_Arguments.h"
#include "cctk_Parameters.h"
#include "IGM.h"

#define IDX(i,j,k) CCTK_GFINDEX3D(cctkGH,(i),(j),(k))

/*********************************************
 * Apply outer boundary conditions on A_{\mu}
 ********************************************/
void GRHayL_IGM_outer_boundaries_on_A_mu(CCTK_ARGUMENTS) {
  DECLARE_CCTK_ARGUMENTS_GRHayL_IGM_outer_boundaries_on_A_mu;
  DECLARE_CCTK_PARAMETERS;

  if(CCTK_EQUALS(EM_BC,"frozen")) return;

  bool Symmetry_none=false; if(CCTK_EQUALS(Symmetry,"none")) Symmetry_none=true;

  const int levelnumber = GetRefinementLevel(cctkGH);

  // Don't apply approximate outer boundary conditions on initial data, which should be defined everywhere, or on levels != [coarsest level].
  if(cctk_iteration==0 || levelnumber!=0) return;

  if(cctk_nghostzones[0]!=cctk_nghostzones[1] || cctk_nghostzones[0]!=cctk_nghostzones[2])
    CCTK_VERROR("ERROR: GRHayL_IGM outer BC driver does not support unequal number of ghostzones in different directions!");
  for(int which_bdry_pt=0; which_bdry_pt<cctk_nghostzones[0]; which_bdry_pt++) {

//TODO: These are in blocks of 4 already; this is screaming for some SIMD
    // for cctk_nghostzones==3, max indices go {cctk_lsh-3,cctk_lsh-2,cctk_lsh-1}; outer bdry pt is at cctk_lsh-1
    // for cctk_nghostzones==3, minimum indices go {2,1,0}
    if(cctk_bbox[1]) {
      const int imax=cctk_lsh[0]-cctk_nghostzones[0]+which_bdry_pt;
#pragma omp parallel for
      for(int k=0; k<cctk_lsh[2]; k++)
        for(int j=0; j<cctk_lsh[1]; j++) {
          phitilde[IDX(imax,j,k)] = 2.0 * phitilde[IDX(imax-1,j,k)] - phitilde[IDX(imax-2,j,k)];
          Ax[IDX(imax,j,k)] = 2.0 * Ax[IDX(imax-1,j,k)] - Ax[IDX(imax-2,j,k)];
          Ay[IDX(imax,j,k)] = 2.0 * Ay[IDX(imax-1,j,k)] - Ay[IDX(imax-2,j,k)];
          Az[IDX(imax,j,k)] = 2.0 * Az[IDX(imax-1,j,k)] - Az[IDX(imax-2,j,k)];
      }
    }
    if(cctk_bbox[3]) {
      const int jmax=cctk_lsh[1]-cctk_nghostzones[1]+which_bdry_pt;
#pragma omp parallel for
      for(int k=0; k<cctk_lsh[2]; k++)
        for(int i=0; i<cctk_lsh[0]; i++) {
          phitilde[IDX(i,jmax,k)] = 2.0 * phitilde[IDX(i,jmax-1,k)] - phitilde[IDX(i,jmax-2,k)];
          Ax[IDX(i,jmax,k)] = 2.0 * Ax[IDX(i,jmax-1,k)] - Ax[IDX(i,jmax-2,k)];
          Ay[IDX(i,jmax,k)] = 2.0 * Ay[IDX(i,jmax-1,k)] - Ay[IDX(i,jmax-2,k)];
          Az[IDX(i,jmax,k)] = 2.0 * Az[IDX(i,jmax-1,k)] - Az[IDX(i,jmax-2,k)];
      }
    }
    if(cctk_bbox[5]) {
      const int kmax=cctk_lsh[2]-cctk_nghostzones[2]+which_bdry_pt;
#pragma omp parallel for
      for(int j=0; j<cctk_lsh[1]; j++)
        for(int i=0; i<cctk_lsh[0]; i++) {
          phitilde[IDX(i,j,kmax)] = 2.0 * phitilde[IDX(i,j,kmax-1)] - phitilde[IDX(i,j,kmax-2)];
          Ax[IDX(i,j,kmax)] = 2.0 * Ax[IDX(i,j,kmax-1)] - Ax[IDX(i,j,kmax-2)];
          Ay[IDX(i,j,kmax)] = 2.0 * Ay[IDX(i,j,kmax-1)] - Ay[IDX(i,j,kmax-2)];
          Az[IDX(i,j,kmax)] = 2.0 * Az[IDX(i,j,kmax-1)] - Az[IDX(i,j,kmax-2)];
      }
    }

    if(cctk_bbox[0]) {
      const int imin=cctk_nghostzones[0]-which_bdry_pt-1;
#pragma omp parallel for
      for(int k=0; k<cctk_lsh[2]; k++)
        for(int j=0; j<cctk_lsh[1]; j++) {
          phitilde[IDX(imin,j,k)] = 2.0 * phitilde[IDX(imin+1,j,k)] - phitilde[IDX(imin+2,j,k)];
          Ax[IDX(imin,j,k)] = 2.0 * Ax[IDX(imin+1,j,k)] - Ax[IDX(imin+2,j,k)];
          Ay[IDX(imin,j,k)] = 2.0 * Ay[IDX(imin+1,j,k)] - Ay[IDX(imin+2,j,k)];
          Az[IDX(imin,j,k)] = 2.0 * Az[IDX(imin+1,j,k)] - Az[IDX(imin+2,j,k)];
      }
    }
    if(cctk_bbox[2]) {
      const int jmin=cctk_nghostzones[1]-which_bdry_pt-1;
#pragma omp parallel for
      for(int k=0; k<cctk_lsh[2]; k++)
        for(int i=0; i<cctk_lsh[0]; i++) {
          phitilde[IDX(i,jmin,k)] = 2.0 * phitilde[IDX(i,jmin+1,k)] - phitilde[IDX(i,jmin+2,k)];
          Ax[IDX(i,jmin,k)] = 2.0 * Ax[IDX(i,jmin+1,k)] - Ax[IDX(i,jmin+2,k)];
          Ay[IDX(i,jmin,k)] = 2.0 * Ay[IDX(i,jmin+1,k)] - Ay[IDX(i,jmin+2,k)];
          Az[IDX(i,jmin,k)] = 2.0 * Az[IDX(i,jmin+1,k)] - Az[IDX(i,jmin+2,k)];
      }
    }
    if((cctk_bbox[4]) && Symmetry_none) {
      const int kmin=cctk_nghostzones[2]-which_bdry_pt-1;
#pragma omp parallel for
      for(int j=0; j<cctk_lsh[1]; j++)
        for(int i=0; i<cctk_lsh[0]; i++) {
          phitilde[IDX(i,j,kmin)] = 2.0 * phitilde[IDX(i,j,kmin+1)] - phitilde[IDX(i,j,kmin+2)];
          Ax[IDX(i,j,kmin)] = 2.0 * Ax[IDX(i,j,kmin+1)] - Ax[IDX(i,j,kmin+2)];
          Ay[IDX(i,j,kmin)] = 2.0 * Ay[IDX(i,j,kmin+1)] - Ay[IDX(i,j,kmin+2)];
          Az[IDX(i,j,kmin)] = 2.0 * Az[IDX(i,j,kmin+1)] - Az[IDX(i,j,kmin+2)];
      }
    }
  }
}

/*******************************************************
 * Apply outer boundary conditions on {P,rho_b,vx,vy,vz}
 * It is better to apply BCs on primitives than conservs,
 * because small errors in conservs can be greatly
 * amplified in con2prim, sometimes leading to unphysical
 * primitives & unnecessary fixes.
 *******************************************************/
void GRHayL_IGM_outer_boundaries_on_P_rho_b_vx_vy_vz(CCTK_ARGUMENTS) {
  DECLARE_CCTK_ARGUMENTS_GRHayL_IGM_outer_boundaries_on_P_rho_b_vx_vy_vz;
  DECLARE_CCTK_PARAMETERS;

  if(CCTK_EQUALS(Matter_BC,"frozen")) return;

  bool Symmetry_none=false; if(CCTK_EQUALS(Symmetry,"none")) Symmetry_none=true;

  int levelnumber = GetRefinementLevel(cctkGH);

  // Don't apply approximate outer boundary conditions on initial data, which should be defined everywhere, or on levels != [coarsest level].
  if(cctk_iteration==0 || levelnumber!=0) return;

  if(cctk_nghostzones[0]!=cctk_nghostzones[1] || cctk_nghostzones[0]!=cctk_nghostzones[2])
    CCTK_VERROR("ERROR: GRHayL_IGM outer BC driver does not support unequal number of ghostzones in different directions!");
  for(int which_bdry_pt=0;which_bdry_pt<cctk_nghostzones[0];which_bdry_pt++) {

    /* XMIN & XMAX */
    // i=imax=outer boundary
    if(cctk_bbox[1]) {
      const int imax=cctk_lsh[0]-cctk_nghostzones[0]+which_bdry_pt;
#pragma omp parallel for
      for(int k=0; k<cctk_lsh[2]; k++)
        for(int j=0; j<cctk_lsh[1]; j++) {
          press[IDX(imax,j,k)] = press[IDX(imax-1,j,k)];
          rho[IDX(imax,j,k)]   = rho[IDX(imax-1,j,k)];
          vx[IDX(imax,j,k)]    = vx[IDX(imax-1,j,k)];
          vy[IDX(imax,j,k)]    = vy[IDX(imax-1,j,k)];
          vz[IDX(imax,j,k)]    = vz[IDX(imax-1,j,k)]; 
          if(vx[IDX(imax,j,k)]<0.) vx[IDX(imax,j,k)] = 0.0;
      }
    }
    // i=imin=outer boundary
    if(cctk_bbox[0]) {
      const int imin=cctk_nghostzones[0]-which_bdry_pt-1;
#pragma omp parallel for
      for(int k=0; k<cctk_lsh[2]; k++)
        for(int j=0; j<cctk_lsh[1]; j++) {
          press[IDX(imin,j,k)] = press[IDX(imin+1,j,k)];
          rho[IDX(imin,j,k)]   = rho[IDX(imin+1,j,k)];
          vx[IDX(imin,j,k)]    = vx[IDX(imin+1,j,k)];
          vy[IDX(imin,j,k)]    = vy[IDX(imin+1,j,k)];
          vz[IDX(imin,j,k)]    = vz[IDX(imin+1,j,k)]; 
          if(vx[IDX(imin,j,k)]>0.) vx[IDX(imin,j,k)] = 0.0;
      }
    }

    /* YMIN & YMAX */
    // j=jmax=outer boundary
    if(cctk_bbox[3]) {
      const int jmax=cctk_lsh[1]-cctk_nghostzones[1]+which_bdry_pt;
#pragma omp parallel for
      for(int k=0; k<cctk_lsh[2]; k++)
        for(int i=0; i<cctk_lsh[0]; i++) {
          press[IDX(i,jmax,k)] = press[IDX(i,jmax-1,k)];
          rho[IDX(i,jmax,k)]   = rho[IDX(i,jmax-1,k)];
          vx[IDX(i,jmax,k)]    = vx[IDX(i,jmax-1,k)];
          vy[IDX(i,jmax,k)]    = vy[IDX(i,jmax-1,k)];
          vz[IDX(i,jmax,k)]    = vz[IDX(i,jmax-1,k)]; 
          if(vx[IDX(i,jmax,k)]<0.) vx[IDX(i,jmax,k)] = 0.0;
      }
    }
    // j=jmin=outer boundary
    if(cctk_bbox[2]) {
      const int jmin=cctk_nghostzones[1]-which_bdry_pt-1;
#pragma omp parallel for
      for(int k=0; k<cctk_lsh[2]; k++)
        for(int i=0; i<cctk_lsh[0]; i++) {
          press[IDX(i,jmin,k)] = press[IDX(i,jmin+1,k)];
          rho[IDX(i,jmin,k)]   = rho[IDX(i,jmin+1,k)];
          vx[IDX(i,jmin,k)]    = vx[IDX(i,jmin+1,k)];
          vy[IDX(i,jmin,k)]    = vy[IDX(i,jmin+1,k)];
          vz[IDX(i,jmin,k)]    = vz[IDX(i,jmin+1,k)]; 
          if(vx[IDX(i,jmin,k)]>0.) vx[IDX(i,jmin,k)] = 0.0;
      }
    }

    /* ZMIN & ZMAX */
    // k=kmax=outer boundary
    if(cctk_bbox[5]) {
      const int kmax=cctk_lsh[2]-cctk_nghostzones[2]+which_bdry_pt;
#pragma omp parallel for
      for(int j=0; j<cctk_lsh[1]; j++)
        for(int i=0; i<cctk_lsh[0]; i++) {
          press[IDX(i,j,kmax)] = press[IDX(i,j,kmax-1)];
          rho[IDX(i,j,kmax)]   = rho[IDX(i,j,kmax-1)];
          vx[IDX(i,j,kmax)]    = vx[IDX(i,j,kmax-1)];
          vy[IDX(i,j,kmax)]    = vy[IDX(i,j,kmax-1)];
          vz[IDX(i,j,kmax)]    = vz[IDX(i,j,kmax-1)]; 
          if(vx[IDX(i,j,kmax)]<0.) vx[IDX(i,j,kmax)] = 0.0;
      }
    }
    // k=kmin=outer boundary
    if((cctk_bbox[4]) && Symmetry_none) {
      const int kmin=cctk_nghostzones[2]-which_bdry_pt-1;
#pragma omp parallel for
      for(int j=0; j<cctk_lsh[1]; j++)
        for(int i=0; i<cctk_lsh[0]; i++) {
          press[IDX(i,j,kmin)] = press[IDX(i,j,kmin+1)];
          rho[IDX(i,j,kmin)]   = rho[IDX(i,j,kmin+1)];
          vx[IDX(i,j,kmin)]    = vx[IDX(i,j,kmin+1)];
          vy[IDX(i,j,kmin)]    = vy[IDX(i,j,kmin+1)];
          vz[IDX(i,j,kmin)]    = vz[IDX(i,j,kmin+1)]; 
          if(vx[IDX(i,j,kmin)]>0.) vx[IDX(i,j,kmin)] = 0.0;
      }
    }
  }

  const double poison = 0.0/0.0;
  double dummy1, dummy2, dummy3;

#pragma omp parallel for
  for(int k=0; k<cctk_lsh[2]; k++)
    for(int j=0; j<cctk_lsh[1]; j++)
      for(int i=0; i<cctk_lsh[0]; i++) {
        if(((cctk_bbox[0]) && i<cctk_nghostzones[0]) ||
           ((cctk_bbox[1]) && i>=cctk_lsh[0]-cctk_nghostzones[0]) ||
           ((cctk_bbox[2]) && j<cctk_nghostzones[1]) ||
           ((cctk_bbox[3]) && j>=cctk_lsh[1]-cctk_nghostzones[1]) ||
           ((cctk_bbox[4]) && k<cctk_nghostzones[2] && CCTK_EQUALS(Symmetry,"none")) ||
           ((cctk_bbox[5]) && k>=cctk_lsh[2]-cctk_nghostzones[2])) {
          const int index = CCTK_GFINDEX3D(cctkGH,i,j,k);

          metric_quantities metric;
          GRHayL_enforce_detgtij_and_initialize_metric(
                alp[index],
                gxx[index], gxy[index], gxz[index],
                gyy[index], gyz[index], gzz[index],
                betax[index], betay[index], betaz[index],
                &metric);
      
          primitive_quantities prims;
          initialize_primitives(rho[index],
                press[index], eps[index],
                vx[index], vy[index], vz[index],
                Bx_center[index], By_center[index], Bz_center[index],
                poison, poison, poison, &prims);
                //wont have storage for these vars for hybrid
                //entropy[index], Y_e[index], temperature[index], &prims);
      
          conservative_quantities cons;
          stress_energy Tmunu;
          int speed_limited = 0;
          enforce_primitive_limits_and_compute_u0(grhayl_params, grhayl_eos, &metric, &prims, &speed_limited);
          compute_conservs_and_Tmunu(grhayl_params, grhayl_eos, &metric, &prims, &cons, &Tmunu);
      
          return_primitives(&prims,
                &rho[index], &press[index], &eps[index],
                &vx[index], &vy[index], &vz[index],
                //&Bvec[index0], &Bvec[index1], &Bvec[index2],
                &Bx_center[index], &By_center[index], &Bz_center[index],
                &dummy1, &dummy2, &dummy3);
                //wont have storage for these vars for hybrid
                //&entropy[index], &Y_e[index], &temperature[index]);
      
          return_conservatives(&cons,
                &rho_star[index], &tau[index],
                &Stildex[index], &Stildey[index], &Stildez[index],
                &dummy1, &dummy2);
                //&Y_e[index], &entropy[index]);
      
          if(grhayl_params->update_Tmunu) {
            return_stress_energy(&Tmunu, &eTtt[index], &eTtx[index],
                  &eTty[index], &eTtz[index], &eTxx[index],
                  &eTxy[index], &eTxz[index], &eTyy[index],
                  &eTyz[index], &eTzz[index]);
          }
        }
  }
}
