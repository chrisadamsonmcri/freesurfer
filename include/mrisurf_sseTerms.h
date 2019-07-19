#pragma once
/*
 * @file The SSE terms
 *
 */
/*
 * Author: Bevin R Brett
 *
 * $ © copyright-2019 The General Hospital Corporation (Boston, MA) "MGH"
 *
 * Terms and conditions for use, reproduction, distribution and contribution
 * are found in the 'FreeSurfer Software License Agreement' contained
 * in the file 'LICENSE' found in the FreeSurfer distribution, and here:
 *
 * https://surfer.nmr.mgh.harvard.edu/fswiki/FreeSurferSoftwareLicense
 *
 * Reporting: freesurfer@nmr.mgh.harvard.edu
 *
 */
 
//  Organize the ComputeSSE terms in a way that enables calculation of several of them in a single pass over either the vertices or the faces
//  and using either the MRIS or another representation of the Surface Face Vertex information
//
#include "mrisurf_metricProperties.h"


// misc
//
static const unsigned int   mrisurf_sse_MAX_VOXELS = 1500;
static const unsigned int   mrisurf_sse_MAX_DISPLACEMENT = 5;
static const double         mrisurf_sse_DISPLACEMENT_DELTA = 0.1;
static const float          mrisurf_sse_DEFAULT_STD = 4.0f;

double vlst_loglikelihood(
    MRI_SURFACE *mris, MRI *mri, int vno, double displacement, VOXEL_LIST *vl, HISTOGRAM *hin, HISTOGRAM *hout);

double vlst_loglikelihood2D(
    MRI_SURFACE *mris, MRI *mri, int vno, double displacement, VOXEL_LIST *vl, HISTOGRAM2D *h, FILE *fp);

int mrisCreateLikelihoodHistograms(MRI_SURFACE *mris, INTEGRATION_PARMS *parms);

void mrisComputeFaceRelevantAngleAndArea   (MRIS*    mris, INTEGRATION_PARMS *parms, double* relevant_angle, double* computed_neg_area, double* computed_area);
void mrismp_ComputeFaceRelevantAngleAndArea(MRIS_MP* mris, INTEGRATION_PARMS *parms, double* relevant_angle, double* computed_neg_area, double* computed_area);
int mrisAverageSignedGradients             (MRIS*    mris, int num_avgs);
int mrisComputePositioningGradients        (MRIS*    mris, INTEGRATION_PARMS *parms);

double MRIScomputeSSE        (MRI_SURFACE *mris, INTEGRATION_PARMS *parms);
double MRIScomputeSSEExternal(MRI_SURFACE *mris, INTEGRATION_PARMS *parms, double *ext_sse);

// MEF support
//
double mrisRmsValError_mef                      (MRI_SURFACE *mris, MRI *mri_30, MRI *mri_5, float weight30, float weight5);
double mrisComputeSSE_MEF                       (MRI_SURFACE *mris, INTEGRATION_PARMS *parms, MRI *mri30, MRI *mri5, double weight30, double weight5, MHT *mht);

int    mrisComputeIntensityTerm_mef             (MRI_SURFACE *mris,
                                                                    double l_intensity,
                                                                    MRI *mri_30,
                                                                    MRI *mri_5,
                                                                    double sigma_global,
                                                                    float weight30,
                                                                    float weight5,
                                                                    INTEGRATION_PARMS *parms);
int mrisComputeIntensityTerm_mef                (MRI_SURFACE *mris,
                                                                    double l_intensity,
                                                                    MRI *mri_30,
                                                                    MRI *mri_5,
                                                                    double sigma_global,
                                                                    float weight30,
                                                                    float weight5,
                                                                    INTEGRATION_PARMS *parms);

// energy measurements
//      the selector parameter lets the caller compute the term on the specified vertex or face rather than summing all of them
//      mostly it is NYI
//
#define LIST_OF_PER_VERTEX_SSETERMS_NYI \
    ELT(AshburnerTriangleEnergy       , (MRIS_PARAMETER_COMMA double l_ashburner_triangle, INTEGRATION_PARMS *parms                         ,int selector DEFSEL), (l_ashburner_triangle,parms,selector)) SEP \
    ELT(LaplacianEnergy               , (MRIS_PARAMETER_COMMA                                                                                int selector DEFSEL), (selector)) SEP \
    ELT(NonlinearSpringEnergy         , (MRIS_PARAMETER_COMMA                              INTEGRATION_PARMS *parms                         ,int selector DEFSEL), (parms,selector)) SEP \
    ELT(RepulsiveEnergy               , (MRIS_PARAMETER_COMMA double l_repulse, MHT *mht_v_current, MHT *mht_f_current                      ,int selector DEFSEL), (l_repulse, mht_v_current, mht_f_current,selector)) SEP \
    ELT(SurfaceRepulsionEnergy        , (MRIS_PARAMETER_COMMA double l_repulse, MHT *mht                                                    ,int selector DEFSEL), (l_repulse, mht,selector)) SEP \
    ELT(TangentialSpringEnergy        , (MRIS_PARAMETER_COMMA                                                                                int selector DEFSEL), (selector))     \
    // end of macro

#define LIST_OF_PER_VERTEX_SSETERMS_Implemented \
    ELT(RepulsiveRatioEnergy          , (MRIS_PARAMETER_COMMA double l_repulse                                                              ,int selector DEFSEL), (l_repulse,selector)) SEP \
    ELT(SpringEnergy                  , (MRIS_PARAMETER_COMMA                                                                                int selector DEFSEL), (selector)) SEP \
    ELT(ThicknessMinimizationEnergy   , (MRIS_PARAMETER_COMMA double l_thick_min,        INTEGRATION_PARMS *parms                           ,int selector DEFSEL), (l_thick_min,parms,selector)) SEP \
    ELT(ThicknessNormalEnergy         , (MRIS_PARAMETER_COMMA double l_thick_normal,     INTEGRATION_PARMS *parms                           ,int selector DEFSEL), (l_thick_normal,parms,selector)) SEP \
    ELT(ThicknessSpringEnergy         , (MRIS_PARAMETER_COMMA double l_thick_spring,     INTEGRATION_PARMS *parms                           ,int selector DEFSEL), (l_thick_spring,parms,selector)) SEP \
    ELT(ThicknessParallelEnergy       , (MRIS_PARAMETER_COMMA double l_thick_parallel,   INTEGRATION_PARMS *parms                           ,int selector DEFSEL), (l_thick_parallel,parms,selector)) SEP \
    ELT(ThicknessSmoothnessEnergy     , (MRIS_PARAMETER_COMMA double l_tsmooth,          INTEGRATION_PARMS *parms                           ,int selector DEFSEL), (l_tsmooth,parms,selector)) SEP \
    /**/ \
    ELT(NonlinearDistanceSSE          , (MRIS_PARAMETER_COMMA                                                                                int selector DEFSEL), (selector)) SEP \
    ELT(QuadraticCurvatureSSE         , (MRIS_PARAMETER_COMMA double l_curv                                                                 ,int selector DEFSEL), (l_curv,selector)) SEP \
    ELT(HistoNegativeLikelihood       , (MRIS_PARAMETER_COMMA                            INTEGRATION_PARMS *parms                           ,int selector DEFSEL), (parms,selector)) SEP \
    ELT(NegativeLogPosterior          , (MRIS_PARAMETER_COMMA                            INTEGRATION_PARMS *parms, int *pnvox               ,int selector DEFSEL), (parms,pnvox,selector)) SEP \
    ELT(NegativeLogPosterior2D        , (MRIS_PARAMETER_COMMA                            INTEGRATION_PARMS *parms, int *pnvox               ,int selector DEFSEL), (parms,pnvox,selector)) SEP \
    /**/ \
    ELT(CorrelationError              , (MRIS_PARAMETER_COMMA                            INTEGRATION_PARMS *parms, int use_stds             ,int selector DEFSEL), (parms,use_stds,selector)) SEP \
    ELT(DistanceError                 , (MRIS_PARAMETER_COMMA                            INTEGRATION_PARMS *parms                           ,int selector DEFSEL), (parms,selector)) SEP \
    ELT(DuraError                     , (MRIS_PARAMETER_COMMA                            INTEGRATION_PARMS *parms                           ,int selector DEFSEL), (parms,selector)) SEP \
    ELT(IntensityError                , (MRIS_PARAMETER_COMMA                            INTEGRATION_PARMS *parms                           ,int selector DEFSEL), (parms,selector)) SEP \
    ELT(TargetLocationError           , (MRIS_PARAMETER_COMMA                            INTEGRATION_PARMS *parms                           ,int selector DEFSEL), (parms,selector)) SEP \
    ELT(IntensityGradientError        , (MRIS_PARAMETER_COMMA                            INTEGRATION_PARMS *parms                           ,int selector DEFSEL), (parms,selector)) SEP \
    ELT(VectorCorrelationError        , (MRIS_PARAMETER_COMMA                            INTEGRATION_PARMS *parms, int use_stds             ,int selector DEFSEL), (parms,use_stds,selector)) SEP \
    ELT(ExpandwrapError               , (MRIS_PARAMETER_COMMA                      MRI *mri_brain, double l_expandwrap, double target_radius,int selector DEFSEL), (mri_brain,l_expandwrap,target_radius,selector)) SEP \
    ELT(ShrinkwrapError               , (MRIS_PARAMETER_COMMA                            MRI *mri_brain, double l_shrinkwrap                ,int selector DEFSEL), (mri_brain,l_shrinkwrap,selector)) SEP \
    ELT(SphereError                   , (MRIS_PARAMETER_COMMA double l_sphere, double r0                                                    ,int selector DEFSEL), (l_sphere,r0,selector)) SEP \
    ELT(RmsDistanceError              , (MRIS_PARAMETER_COMMA                                                                                int selector DEFSEL), (selector)) SEP \
    ELT(Error                         , (MRIS_PARAMETER_COMMA                            INTEGRATION_PARMS *parms, \
                                                                                            float *parea_rms, \
                                                                                            float *pangle_rms,\
                                                                                            float *pcurv_rms, \
                                                                                            float *pdist_rms, \
                                                                                            float *pcorr_rms                                ,int selector DEFSEL), (parms,parea_rms,pangle_rms,pcurv_rms,pdist_rms,pcorr_rms,selector))     \
    // end of macro

#define LIST_OF_PER_VERTEX_SSETERMS LIST_OF_PER_VERTEX_SSETERMS_NYI SEP LIST_OF_PER_VERTEX_SSETERMS_Implemented

#define LIST_OF_PER_FACE_SSETERMS \
    ELT(NonlinearAreaSSE              , (MRIS_PARAMETER_COMMA                                                                                int selector DEFSEL), (selector))     \
    // end of macro


#define LIST_OF_SSETERMS LIST_OF_PER_VERTEX_SSETERMS SEP LIST_OF_PER_FACE_SSETERMS

#define MRIS_PARAMETER          MRIS* mris
#define MRIS_PARAMETER_COMMA    MRIS_PARAMETER,
#define DEFSEL                  =-1
#define SEP 
#define ELT(NAME, SIGNATURE, CALL)    double mrisCompute##NAME SIGNATURE;
LIST_OF_SSETERMS
#undef ELT
#undef SEP
#undef DEFSEL
#undef MRIS_PARAMETER_COMMA
#undef MRIS_PARAMETER

#define MRIS_PARAMETER          MRIS* mris
#define MRIS_PARAMETER_COMMA    MRIS_PARAMETER,
#define DEFSEL                  =-1
#define SEP 
#define MRIS MRIS_MP
#define ELT(NAME, SIGNATURE, CALL)    double mrismp_Compute##NAME SIGNATURE;
LIST_OF_PER_VERTEX_SSETERMS_Implemented SEP LIST_OF_PER_FACE_SSETERMS
#undef ELT
#undef MRIS
#undef SEP
#undef DEFSEL
#undef MRIS_PARAMETER_COMMA
#undef MRIS_PARAMETER


#define SEP
#define ELT(NAME, SIGNATURE,CALL)    MRIS_sseTermFlag_##NAME##_bitPosition,
enum {
    LIST_OF_SSETERMS
    MRIS_sseTermFlag_bitPosition_LAST };
#undef ELT
#undef SEP
#define SEP ,
#define ELT(NAME, SIGNATURE,CALL)    MRIS_sseTermFlag_##NAME = (1L << (int)MRIS_sseTermFlag_##NAME##_bitPosition)
typedef enum {
    LIST_OF_SSETERMS } MRIS_sseTermFlags;
#undef ELT
#undef SEP


typedef struct MRIS_sseTerms {
    double term[MRIS_sseTermFlag_bitPosition_LAST];
} MRIS_sseTerms;