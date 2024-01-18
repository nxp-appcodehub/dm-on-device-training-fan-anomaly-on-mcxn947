
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
void* svm_train_mode( float train_data[][FEATURE_NUMS],int XLEN);
float svm_data_pre(float pre_data[][FEATURE_NUMS], int Test_Len);
float svm_model_pre(void* model, float pre_data[FEATURE_NUMS]);
void draw_svm_toy_map(void* model, void* buffer_toy, uint32_t toy_w, uint32_t toy_h);
void draw_points_on_svm_toy(void* buffer_toy, uint32_t toy_w, uint32_t toy_h, svm_features_t* features, lv_color_t col, bool del_old);
void draw_svm_toy_update();
void destory_model(void);

#if defined(__cplusplus)
}
#endif