
#include <stdint.h>
#include <stdbool.h>

#include "src/misc/lv_color.h"
#include "gui_guider.h"

#if defined(__cplusplus)
extern "C" {
#endif
/*******************************************************************************
 * API
 ******************************************************************************/
typedef struct {
    uint32_t row, col;
    float* p_feature;
}svm_features_t;

#define FEATURE_NUMS   (2)
/*
 * Return value:
 *  - 0: Initialize success.
 *  - Not 0: Failed.
 */
void* svm_train_mode( float train_data[][FEATURE_NUMS],int XLEN,float gamma, float nu);
float svm_data_pre(float pre_data[][FEATURE_NUMS], int Test_Len);
float svm_model_pre(void* model, float pre_data[FEATURE_NUMS]);
void svm_model_get_SVs(void* model, void* SVs, int32_t* svs_cnt);
void  svm_get_model_param_gamma_nu(void *model, float *gamma, float  *nu);
void destory_model(void);

#if defined(__cplusplus)
}
#endif
