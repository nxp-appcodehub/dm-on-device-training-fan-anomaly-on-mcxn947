# Copyright 2023 NXP
# SPDX-License-Identifier: MIT
# The auto-generated can only be used on NXP devices

import SDL
import utime as time
import usys as sys
import lvgl as lv
import lodepng as png
import ustruct

lv.init()
SDL.init(w=800,h=480)

# Register SDL display driver.
disp_buf1 = lv.disp_draw_buf_t()
buf1_1 = bytearray(800*10)
disp_buf1.init(buf1_1, None, len(buf1_1)//4)
disp_drv = lv.disp_drv_t()
disp_drv.init()
disp_drv.draw_buf = disp_buf1
disp_drv.flush_cb = SDL.monitor_flush
disp_drv.hor_res = 800
disp_drv.ver_res = 480
disp_drv.register()

# Regsiter SDL mouse driver
indev_drv = lv.indev_drv_t()
indev_drv.init() 
indev_drv.type = lv.INDEV_TYPE.POINTER
indev_drv.read_cb = SDL.mouse_read
indev_drv.register()

# Below: Taken from https://github.com/lvgl/lv_binding_micropython/blob/master/driver/js/imagetools.py#L22-L94

COLOR_SIZE = lv.color_t.__SIZE__
COLOR_IS_SWAPPED = hasattr(lv.color_t().ch,'green_h')

class lodepng_error(RuntimeError):
    def __init__(self, err):
        if type(err) is int:
            super().__init__(png.error_text(err))
        else:
            super().__init__(err)

# Parse PNG file header
# Taken from https://github.com/shibukawa/imagesize_py/blob/ffef30c1a4715c5acf90e8945ceb77f4a2ed2d45/imagesize.py#L63-L85

def get_png_info(decoder, src, header):
    # Only handle variable image types

    if lv.img.src_get_type(src) != lv.img.SRC.VARIABLE:
        return lv.RES.INV

    data = lv.img_dsc_t.__cast__(src).data
    if data == None:
        return lv.RES.INV

    png_header = bytes(data.__dereference__(24))

    if png_header.startswith(b'\211PNG\r\n\032\n'):
        if png_header[12:16] == b'IHDR':
            start = 16
        # Maybe this is for an older PNG version.
        else:
            start = 8
        try:
            width, height = ustruct.unpack(">LL", png_header[start:start+8])
        except ustruct.error:
            return lv.RES.INV
    else:
        return lv.RES.INV

    header.always_zero = 0
    header.w = width
    header.h = height
    header.cf = lv.img.CF.TRUE_COLOR_ALPHA

    return lv.RES.OK

def convert_rgba8888_to_bgra8888(img_view):
    for i in range(0, len(img_view), lv.color_t.__SIZE__):
        ch = lv.color_t.__cast__(img_view[i:i]).ch
        ch.red, ch.blue = ch.blue, ch.red

# Read and parse PNG file

def open_png(decoder, dsc):
    img_dsc = lv.img_dsc_t.__cast__(dsc.src)
    png_data = img_dsc.data
    png_size = img_dsc.data_size
    png_decoded = png.C_Pointer()
    png_width = png.C_Pointer()
    png_height = png.C_Pointer()
    error = png.decode32(png_decoded, png_width, png_height, png_data, png_size)
    if error:
        raise lodepng_error(error)
    img_size = png_width.int_val * png_height.int_val * 4
    img_data = png_decoded.ptr_val
    img_view = img_data.__dereference__(img_size)

    if COLOR_SIZE == 4:
        convert_rgba8888_to_bgra8888(img_view)
    else:
        raise lodepng_error("Error: Color mode not supported yet!")

    dsc.img_data = img_data
    return lv.RES.OK

# Above: Taken from https://github.com/lvgl/lv_binding_micropython/blob/master/driver/js/imagetools.py#L22-L94

decoder = lv.img.decoder_create()
decoder.info_cb = get_png_info
decoder.open_cb = open_png

def anim_x_cb(obj, v):
    obj.set_x(v)

def anim_y_cb(obj, v):
    obj.set_y(v)

def ta_event_cb(e,kb):
    code = e.get_code()
    ta = e.get_target()
    if code == lv.EVENT.FOCUSED:
        kb.set_textarea(ta)
        kb.move_foreground()
        kb.clear_flag(lv.obj.FLAG.HIDDEN)

    if code == lv.EVENT.DEFOCUSED:
        kb.set_textarea(None)
        kb.move_background()
        kb.add_flag(lv.obj.FLAG.HIDDEN)
        
def ta_zh_event_cb(e,kb):
    code = e.get_code()
    ta = e.get_target()
    if code == lv.EVENT.FOCUSED:
        kb.keyboard_set_textarea(ta)
        kb.move_foreground()
        kb.clear_flag(lv.obj.FLAG.HIDDEN)

    if code == lv.EVENT.DEFOCUSED:
        kb.keyboard_set_textarea(None)
        kb.move_background()
        kb.add_flag(lv.obj.FLAG.HIDDEN)



# create Main
Main = lv.obj()
Main.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# create style style_main_main_main_default
style_main_main_main_default = lv.style_t()
style_main_main_main_default.init()
style_main_main_main_default.set_bg_color(lv.color_make(0xff,0xff,0xff))
style_main_main_main_default.set_bg_grad_color(lv.color_make(0x2F,0x92,0xDA))
style_main_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.VER)
style_main_main_main_default.set_bg_opa(255)

# add style for Main
Main.add_style(style_main_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create Main_imgMainNXP
Main_imgMainNXP = lv.img(Main)
Main_imgMainNXP.set_pos(int(678),int(419))
Main_imgMainNXP.set_size(100,37)
Main_imgMainNXP.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
Main_imgMainNXP.add_flag(lv.obj.FLAG.CLICKABLE)
try:
    with open('C:\\LocalData\\gui\\anormaly_detect_mcx\\HMI_MCX947_GG151\\generated\\mPythonImages\\mp-284701175.png','rb') as f:
        Main_imgMainNXP_img_data = f.read()
except:
    print('Could not open C:\\LocalData\\gui\\anormaly_detect_mcx\\HMI_MCX947_GG151\\generated\\mPythonImages\\mp-284701175.png')
    sys.exit()

Main_imgMainNXP_img = lv.img_dsc_t({
  'data_size': len(Main_imgMainNXP_img_data),
  'header': {'always_zero': 0, 'w': 100, 'h': 37, 'cf': lv.img.CF.TRUE_COLOR_ALPHA},
  'data': Main_imgMainNXP_img_data
})

Main_imgMainNXP.set_src(Main_imgMainNXP_img)
Main_imgMainNXP.set_pivot(50,50)
Main_imgMainNXP.set_angle(0)
# create style style_main_imgmainnxp_main_main_default
style_main_imgmainnxp_main_main_default = lv.style_t()
style_main_imgmainnxp_main_main_default.init()
style_main_imgmainnxp_main_main_default.set_img_recolor(lv.color_make(0xff,0xff,0xff))
style_main_imgmainnxp_main_main_default.set_img_recolor_opa(0)
style_main_imgmainnxp_main_main_default.set_img_opa(255)

# add style for Main_imgMainNXP
Main_imgMainNXP.add_style(style_main_imgmainnxp_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create Main_labelMainCurFunc
Main_labelMainCurFunc = lv.label(Main)
Main_labelMainCurFunc.set_pos(int(280),int(9))
Main_labelMainCurFunc.set_size(216,43)
Main_labelMainCurFunc.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
Main_labelMainCurFunc.set_text("Anomaly Detect")
Main_labelMainCurFunc.set_long_mode(lv.label.LONG.WRAP)
# create style style_main_labelmaincurfunc_main_main_default
style_main_labelmaincurfunc_main_main_default = lv.style_t()
style_main_labelmaincurfunc_main_main_default.init()
style_main_labelmaincurfunc_main_main_default.set_radius(0)
style_main_labelmaincurfunc_main_main_default.set_bg_color(lv.color_make(0xda,0xf6,0xfd))
style_main_labelmaincurfunc_main_main_default.set_bg_grad_color(lv.color_make(0x2F,0xCA,0xDA))
style_main_labelmaincurfunc_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_main_labelmaincurfunc_main_main_default.set_bg_opa(0)
style_main_labelmaincurfunc_main_main_default.set_text_color(lv.color_make(0x0d,0x30,0x08))
try:
    style_main_labelmaincurfunc_main_main_default.set_text_font(lv.font_montserratMedium_20)
except AttributeError:
    try:
        style_main_labelmaincurfunc_main_main_default.set_text_font(lv.font_montserrat_20)
    except AttributeError:
        style_main_labelmaincurfunc_main_main_default.set_text_font(lv.font_montserrat_16)
style_main_labelmaincurfunc_main_main_default.set_text_letter_space(2)
style_main_labelmaincurfunc_main_main_default.set_text_line_space(0)
style_main_labelmaincurfunc_main_main_default.set_text_align(lv.TEXT_ALIGN.CENTER)
style_main_labelmaincurfunc_main_main_default.set_pad_left(0)
style_main_labelmaincurfunc_main_main_default.set_pad_right(0)
style_main_labelmaincurfunc_main_main_default.set_pad_top(8)
style_main_labelmaincurfunc_main_main_default.set_pad_bottom(0)

# add style for Main_labelMainCurFunc
Main_labelMainCurFunc.add_style(style_main_labelmaincurfunc_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create Main_btnMainSwScr
Main_btnMainSwScr = lv.btn(Main)
Main_btnMainSwScr.set_pos(int(666),int(340))
Main_btnMainSwScr.set_size(112,50)
Main_btnMainSwScr.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
Main_btnMainSwScr_label = lv.label(Main_btnMainSwScr)
Main_btnMainSwScr_label.set_text("Model Train")
Main_btnMainSwScr.set_style_pad_all(0, lv.STATE.DEFAULT)
Main_btnMainSwScr_label.align(lv.ALIGN.CENTER,0,0)
# create style style_main_btnmainswscr_main_main_default
style_main_btnmainswscr_main_main_default = lv.style_t()
style_main_btnmainswscr_main_main_default.init()
style_main_btnmainswscr_main_main_default.set_radius(10)
style_main_btnmainswscr_main_main_default.set_bg_color(lv.color_make(0x2F,0x92,0xDA))
style_main_btnmainswscr_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_main_btnmainswscr_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_main_btnmainswscr_main_main_default.set_bg_opa(255)
style_main_btnmainswscr_main_main_default.set_border_color(lv.color_make(0x21,0x95,0xf6))
style_main_btnmainswscr_main_main_default.set_border_width(0)
style_main_btnmainswscr_main_main_default.set_border_opa(255)
style_main_btnmainswscr_main_main_default.set_text_color(lv.color_make(0xff,0xff,0xff))
try:
    style_main_btnmainswscr_main_main_default.set_text_font(lv.font_montserratMedium_16)
except AttributeError:
    try:
        style_main_btnmainswscr_main_main_default.set_text_font(lv.font_montserrat_16)
    except AttributeError:
        style_main_btnmainswscr_main_main_default.set_text_font(lv.font_montserrat_16)
style_main_btnmainswscr_main_main_default.set_text_align(lv.TEXT_ALIGN.CENTER)

# add style for Main_btnMainSwScr
Main_btnMainSwScr.add_style(style_main_btnmainswscr_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create Main_img_status
Main_img_status = lv.img(Main)
Main_img_status.set_pos(int(594),int(100))
Main_img_status.set_size(160,160)
Main_img_status.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
Main_img_status.add_flag(lv.obj.FLAG.CLICKABLE)
try:
    with open('C:\\LocalData\\gui\\anormaly_detect_mcx\\HMI_MCX947_GG151\\generated\\mPythonImages\\mp-522958393.png','rb') as f:
        Main_img_status_img_data = f.read()
except:
    print('Could not open C:\\LocalData\\gui\\anormaly_detect_mcx\\HMI_MCX947_GG151\\generated\\mPythonImages\\mp-522958393.png')
    sys.exit()

Main_img_status_img = lv.img_dsc_t({
  'data_size': len(Main_img_status_img_data),
  'header': {'always_zero': 0, 'w': 160, 'h': 160, 'cf': lv.img.CF.TRUE_COLOR_ALPHA},
  'data': Main_img_status_img_data
})

Main_img_status.set_src(Main_img_status_img)
Main_img_status.set_pivot(50,50)
Main_img_status.set_angle(0)
# create style style_main_img_status_main_main_default
style_main_img_status_main_main_default = lv.style_t()
style_main_img_status_main_main_default.init()
style_main_img_status_main_main_default.set_img_recolor(lv.color_make(0xff,0xff,0xff))
style_main_img_status_main_main_default.set_img_recolor_opa(0)
style_main_img_status_main_main_default.set_img_opa(255)

# add style for Main_img_status
Main_img_status.add_style(style_main_img_status_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create Main_chart_1
Main_chart_1 = lv.chart(Main)
Main_chart_1.set_pos(int(97),int(100))
Main_chart_1.set_size(480,160)
Main_chart_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
Main_chart_1.set_type(lv.chart.TYPE.LINE)
Main_chart_1.set_range(lv.chart.AXIS.PRIMARY_Y, 0, 100)
Main_chart_1.set_div_line_count(3, 5)
Main_chart_1.set_point_count(7)
chart_series_0 = lv.chart.add_series(Main_chart_1, lv.color_make(0x00,0x00,0x00), lv.chart.AXIS.PRIMARY_Y);
Main_chart_1.set_next_value(chart_series_0, 1)
Main_chart_1.set_next_value(chart_series_0, 20)
Main_chart_1.set_next_value(chart_series_0, 30)
Main_chart_1.set_next_value(chart_series_0, 40)
Main_chart_1.set_next_value(chart_series_0, 5)
chart_series_1 = lv.chart.add_series(Main_chart_1, lv.color_make(0xcc,0x2d,0x2d), lv.chart.AXIS.PRIMARY_Y);
Main_chart_1.set_next_value(chart_series_1, 0)
Main_chart_1.set_next_value(chart_series_1, 15)
Main_chart_1.set_next_value(chart_series_1, 16)
Main_chart_1.set_next_value(chart_series_1, 18)
Main_chart_1.set_next_value(chart_series_1, 6)
Main_chart_1.set_next_value(chart_series_1, 23)
chart_series_2 = lv.chart.add_series(Main_chart_1, lv.color_make(0x37,0xa5,0x18), lv.chart.AXIS.PRIMARY_Y);
Main_chart_1.set_next_value(chart_series_2, 0)
Main_chart_1.set_next_value(chart_series_2, 7)
Main_chart_1.set_next_value(chart_series_2, 9)
Main_chart_1.set_next_value(chart_series_2, 11)
Main_chart_1.set_next_value(chart_series_2, 16)
Main_chart_1.set_next_value(chart_series_2, 16)
Main_chart_1.set_next_value(chart_series_2, 31)
# create style style_main_chart_1_main_main_default
style_main_chart_1_main_main_default = lv.style_t()
style_main_chart_1_main_main_default.init()
style_main_chart_1_main_main_default.set_radius(0)
style_main_chart_1_main_main_default.set_bg_color(lv.color_make(0xff,0xff,0xff))
style_main_chart_1_main_main_default.set_bg_grad_color(lv.color_make(0xff,0xff,0xff))
style_main_chart_1_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_main_chart_1_main_main_default.set_bg_opa(255)
style_main_chart_1_main_main_default.set_border_color(lv.color_make(0xe8,0xe8,0xe8))
style_main_chart_1_main_main_default.set_border_width(1)
style_main_chart_1_main_main_default.set_border_opa(255)
style_main_chart_1_main_main_default.set_line_color(lv.color_make(0xe8,0xe8,0xe8))
style_main_chart_1_main_main_default.set_line_width(2)
style_main_chart_1_main_main_default.set_line_opa(255)

# add style for Main_chart_1
Main_chart_1.add_style(style_main_chart_1_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create ANModel_Training
ANModel_Training = lv.obj()
ANModel_Training.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# create style style_anmodel_training_main_main_default
style_anmodel_training_main_main_default = lv.style_t()
style_anmodel_training_main_main_default.init()
style_anmodel_training_main_main_default.set_bg_color(lv.color_make(0xff,0xff,0xff))
style_anmodel_training_main_main_default.set_bg_grad_color(lv.color_make(0x2F,0x92,0xDA))
style_anmodel_training_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.VER)
style_anmodel_training_main_main_default.set_bg_opa(255)

# add style for ANModel_Training
ANModel_Training.add_style(style_anmodel_training_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create ANModel_Training_imgANModelTrainNXP
ANModel_Training_imgANModelTrainNXP = lv.img(ANModel_Training)
ANModel_Training_imgANModelTrainNXP.set_pos(int(678),int(419))
ANModel_Training_imgANModelTrainNXP.set_size(100,37)
ANModel_Training_imgANModelTrainNXP.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
ANModel_Training_imgANModelTrainNXP.add_flag(lv.obj.FLAG.CLICKABLE)
try:
    with open('C:\\LocalData\\gui\\anormaly_detect_mcx\\HMI_MCX947_GG151\\generated\\mPythonImages\\mp-284701175.png','rb') as f:
        ANModel_Training_imgANModelTrainNXP_img_data = f.read()
except:
    print('Could not open C:\\LocalData\\gui\\anormaly_detect_mcx\\HMI_MCX947_GG151\\generated\\mPythonImages\\mp-284701175.png')
    sys.exit()

ANModel_Training_imgANModelTrainNXP_img = lv.img_dsc_t({
  'data_size': len(ANModel_Training_imgANModelTrainNXP_img_data),
  'header': {'always_zero': 0, 'w': 100, 'h': 37, 'cf': lv.img.CF.TRUE_COLOR_ALPHA},
  'data': ANModel_Training_imgANModelTrainNXP_img_data
})

ANModel_Training_imgANModelTrainNXP.set_src(ANModel_Training_imgANModelTrainNXP_img)
ANModel_Training_imgANModelTrainNXP.set_pivot(50,50)
ANModel_Training_imgANModelTrainNXP.set_angle(0)
# create style style_anmodel_training_imganmodeltrainnxp_main_main_default
style_anmodel_training_imganmodeltrainnxp_main_main_default = lv.style_t()
style_anmodel_training_imganmodeltrainnxp_main_main_default.init()
style_anmodel_training_imganmodeltrainnxp_main_main_default.set_img_recolor(lv.color_make(0xff,0xff,0xff))
style_anmodel_training_imganmodeltrainnxp_main_main_default.set_img_recolor_opa(0)
style_anmodel_training_imganmodeltrainnxp_main_main_default.set_img_opa(255)

# add style for ANModel_Training_imgANModelTrainNXP
ANModel_Training_imgANModelTrainNXP.add_style(style_anmodel_training_imganmodeltrainnxp_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create ANModel_Training_labelANModelTrainCurFunc
ANModel_Training_labelANModelTrainCurFunc = lv.label(ANModel_Training)
ANModel_Training_labelANModelTrainCurFunc.set_pos(int(38),int(43))
ANModel_Training_labelANModelTrainCurFunc.set_size(216,43)
ANModel_Training_labelANModelTrainCurFunc.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
ANModel_Training_labelANModelTrainCurFunc.set_text("Anomaly Detect")
ANModel_Training_labelANModelTrainCurFunc.set_long_mode(lv.label.LONG.WRAP)
# create style style_anmodel_training_labelanmodeltraincurfunc_main_main_default
style_anmodel_training_labelanmodeltraincurfunc_main_main_default = lv.style_t()
style_anmodel_training_labelanmodeltraincurfunc_main_main_default.init()
style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_radius(0)
style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_bg_color(lv.color_make(0x52,0xe3,0x24))
style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_bg_opa(0)
style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_text_color(lv.color_make(0x0d,0x30,0x08))
try:
    style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_text_font(lv.font_montserratMedium_20)
except AttributeError:
    try:
        style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_text_font(lv.font_montserrat_20)
    except AttributeError:
        style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_text_font(lv.font_montserrat_16)
style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_text_letter_space(2)
style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_text_line_space(0)
style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_text_align(lv.TEXT_ALIGN.LEFT)
style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_pad_left(0)
style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_pad_right(0)
style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_pad_top(8)
style_anmodel_training_labelanmodeltraincurfunc_main_main_default.set_pad_bottom(0)

# add style for ANModel_Training_labelANModelTrainCurFunc
ANModel_Training_labelANModelTrainCurFunc.add_style(style_anmodel_training_labelanmodeltraincurfunc_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create ANModel_Training_btnANModelTrainReturn
ANModel_Training_btnANModelTrainReturn = lv.btn(ANModel_Training)
ANModel_Training_btnANModelTrainReturn.set_pos(int(496),int(340))
ANModel_Training_btnANModelTrainReturn.set_size(112,50)
ANModel_Training_btnANModelTrainReturn.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
ANModel_Training_btnANModelTrainReturn_label = lv.label(ANModel_Training_btnANModelTrainReturn)
ANModel_Training_btnANModelTrainReturn_label.set_text("Return")
ANModel_Training_btnANModelTrainReturn.set_style_pad_all(0, lv.STATE.DEFAULT)
ANModel_Training_btnANModelTrainReturn_label.align(lv.ALIGN.CENTER,0,0)
# create style style_anmodel_training_btnanmodeltrainreturn_main_main_default
style_anmodel_training_btnanmodeltrainreturn_main_main_default = lv.style_t()
style_anmodel_training_btnanmodeltrainreturn_main_main_default.init()
style_anmodel_training_btnanmodeltrainreturn_main_main_default.set_radius(10)
style_anmodel_training_btnanmodeltrainreturn_main_main_default.set_bg_color(lv.color_make(0x2F,0x92,0xDA))
style_anmodel_training_btnanmodeltrainreturn_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_anmodel_training_btnanmodeltrainreturn_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_anmodel_training_btnanmodeltrainreturn_main_main_default.set_bg_opa(255)
style_anmodel_training_btnanmodeltrainreturn_main_main_default.set_border_color(lv.color_make(0x21,0x95,0xf6))
style_anmodel_training_btnanmodeltrainreturn_main_main_default.set_border_width(0)
style_anmodel_training_btnanmodeltrainreturn_main_main_default.set_border_opa(255)
style_anmodel_training_btnanmodeltrainreturn_main_main_default.set_text_color(lv.color_make(0xff,0xff,0xff))
try:
    style_anmodel_training_btnanmodeltrainreturn_main_main_default.set_text_font(lv.font_montserratMedium_16)
except AttributeError:
    try:
        style_anmodel_training_btnanmodeltrainreturn_main_main_default.set_text_font(lv.font_montserrat_16)
    except AttributeError:
        style_anmodel_training_btnanmodeltrainreturn_main_main_default.set_text_font(lv.font_montserrat_16)
style_anmodel_training_btnanmodeltrainreturn_main_main_default.set_text_align(lv.TEXT_ALIGN.CENTER)

# add style for ANModel_Training_btnANModelTrainReturn
ANModel_Training_btnANModelTrainReturn.add_style(style_anmodel_training_btnanmodeltrainreturn_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create ANModel_Training_labelANModelTrainInfo
ANModel_Training_labelANModelTrainInfo = lv.label(ANModel_Training)
ANModel_Training_labelANModelTrainInfo.set_pos(int(38),int(117))
ANModel_Training_labelANModelTrainInfo.set_size(216,43)
ANModel_Training_labelANModelTrainInfo.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
ANModel_Training_labelANModelTrainInfo.set_text("Information")
ANModel_Training_labelANModelTrainInfo.set_long_mode(lv.label.LONG.WRAP)
# create style style_anmodel_training_labelanmodeltraininfo_main_main_default
style_anmodel_training_labelanmodeltraininfo_main_main_default = lv.style_t()
style_anmodel_training_labelanmodeltraininfo_main_main_default.init()
style_anmodel_training_labelanmodeltraininfo_main_main_default.set_radius(0)
style_anmodel_training_labelanmodeltraininfo_main_main_default.set_bg_color(lv.color_make(0x52,0xe3,0x24))
style_anmodel_training_labelanmodeltraininfo_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_anmodel_training_labelanmodeltraininfo_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_anmodel_training_labelanmodeltraininfo_main_main_default.set_bg_opa(0)
style_anmodel_training_labelanmodeltraininfo_main_main_default.set_text_color(lv.color_make(0x0d,0x30,0x08))
try:
    style_anmodel_training_labelanmodeltraininfo_main_main_default.set_text_font(lv.font_montserratMedium_20)
except AttributeError:
    try:
        style_anmodel_training_labelanmodeltraininfo_main_main_default.set_text_font(lv.font_montserrat_20)
    except AttributeError:
        style_anmodel_training_labelanmodeltraininfo_main_main_default.set_text_font(lv.font_montserrat_16)
style_anmodel_training_labelanmodeltraininfo_main_main_default.set_text_letter_space(2)
style_anmodel_training_labelanmodeltraininfo_main_main_default.set_text_line_space(0)
style_anmodel_training_labelanmodeltraininfo_main_main_default.set_text_align(lv.TEXT_ALIGN.LEFT)
style_anmodel_training_labelanmodeltraininfo_main_main_default.set_pad_left(0)
style_anmodel_training_labelanmodeltraininfo_main_main_default.set_pad_right(0)
style_anmodel_training_labelanmodeltraininfo_main_main_default.set_pad_top(8)
style_anmodel_training_labelanmodeltraininfo_main_main_default.set_pad_bottom(0)

# add style for ANModel_Training_labelANModelTrainInfo
ANModel_Training_labelANModelTrainInfo.add_style(style_anmodel_training_labelanmodeltraininfo_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create ANModel_Training_labelANModelTrainStat
ANModel_Training_labelANModelTrainStat = lv.label(ANModel_Training)
ANModel_Training_labelANModelTrainStat.set_pos(int(419),int(74))
ANModel_Training_labelANModelTrainStat.set_size(216,43)
ANModel_Training_labelANModelTrainStat.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
ANModel_Training_labelANModelTrainStat.set_text("Status")
ANModel_Training_labelANModelTrainStat.set_long_mode(lv.label.LONG.WRAP)
# create style style_anmodel_training_labelanmodeltrainstat_main_main_default
style_anmodel_training_labelanmodeltrainstat_main_main_default = lv.style_t()
style_anmodel_training_labelanmodeltrainstat_main_main_default.init()
style_anmodel_training_labelanmodeltrainstat_main_main_default.set_radius(0)
style_anmodel_training_labelanmodeltrainstat_main_main_default.set_bg_color(lv.color_make(0x52,0xe3,0x24))
style_anmodel_training_labelanmodeltrainstat_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_anmodel_training_labelanmodeltrainstat_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_anmodel_training_labelanmodeltrainstat_main_main_default.set_bg_opa(0)
style_anmodel_training_labelanmodeltrainstat_main_main_default.set_text_color(lv.color_make(0x0d,0x30,0x08))
try:
    style_anmodel_training_labelanmodeltrainstat_main_main_default.set_text_font(lv.font_montserratMedium_20)
except AttributeError:
    try:
        style_anmodel_training_labelanmodeltrainstat_main_main_default.set_text_font(lv.font_montserrat_20)
    except AttributeError:
        style_anmodel_training_labelanmodeltrainstat_main_main_default.set_text_font(lv.font_montserrat_16)
style_anmodel_training_labelanmodeltrainstat_main_main_default.set_text_letter_space(2)
style_anmodel_training_labelanmodeltrainstat_main_main_default.set_text_line_space(0)
style_anmodel_training_labelanmodeltrainstat_main_main_default.set_text_align(lv.TEXT_ALIGN.CENTER)
style_anmodel_training_labelanmodeltrainstat_main_main_default.set_pad_left(0)
style_anmodel_training_labelanmodeltrainstat_main_main_default.set_pad_right(0)
style_anmodel_training_labelanmodeltrainstat_main_main_default.set_pad_top(8)
style_anmodel_training_labelanmodeltrainstat_main_main_default.set_pad_bottom(0)

# add style for ANModel_Training_labelANModelTrainStat
ANModel_Training_labelANModelTrainStat.add_style(style_anmodel_training_labelanmodeltrainstat_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create ANModel_Training_barANModelTrainProgress
ANModel_Training_barANModelTrainProgress = lv.bar(ANModel_Training)
ANModel_Training_barANModelTrainProgress.set_pos(int(138),int(227))
ANModel_Training_barANModelTrainProgress.set_size(525,26)
ANModel_Training_barANModelTrainProgress.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
ANModel_Training_barANModelTrainProgress.set_style_anim_time(1000, 0)
ANModel_Training_barANModelTrainProgress.set_mode(lv.bar.MODE.NORMAL)
ANModel_Training_barANModelTrainProgress.set_value(50, lv.ANIM.OFF)
# create style style_anmodel_training_baranmodeltrainprogress_main_main_default
style_anmodel_training_baranmodeltrainprogress_main_main_default = lv.style_t()
style_anmodel_training_baranmodeltrainprogress_main_main_default.init()
style_anmodel_training_baranmodeltrainprogress_main_main_default.set_radius(10)
style_anmodel_training_baranmodeltrainprogress_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_anmodel_training_baranmodeltrainprogress_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_anmodel_training_baranmodeltrainprogress_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_anmodel_training_baranmodeltrainprogress_main_main_default.set_bg_opa(60)

# add style for ANModel_Training_barANModelTrainProgress
ANModel_Training_barANModelTrainProgress.add_style(style_anmodel_training_baranmodeltrainprogress_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

# create style style_anmodel_training_baranmodeltrainprogress_main_indicator_default
style_anmodel_training_baranmodeltrainprogress_main_indicator_default = lv.style_t()
style_anmodel_training_baranmodeltrainprogress_main_indicator_default.init()
style_anmodel_training_baranmodeltrainprogress_main_indicator_default.set_radius(10)
style_anmodel_training_baranmodeltrainprogress_main_indicator_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_anmodel_training_baranmodeltrainprogress_main_indicator_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_anmodel_training_baranmodeltrainprogress_main_indicator_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_anmodel_training_baranmodeltrainprogress_main_indicator_default.set_bg_opa(255)

# add style for ANModel_Training_barANModelTrainProgress
ANModel_Training_barANModelTrainProgress.add_style(style_anmodel_training_baranmodeltrainprogress_main_indicator_default, lv.PART.INDICATOR|lv.STATE.DEFAULT)


# create ANModel_Training_btnANModelTrainStart
ANModel_Training_btnANModelTrainStart = lv.btn(ANModel_Training)
ANModel_Training_btnANModelTrainStart.set_pos(int(192),int(340))
ANModel_Training_btnANModelTrainStart.set_size(112,50)
ANModel_Training_btnANModelTrainStart.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
ANModel_Training_btnANModelTrainStart_label = lv.label(ANModel_Training_btnANModelTrainStart)
ANModel_Training_btnANModelTrainStart_label.set_text("Start")
ANModel_Training_btnANModelTrainStart.set_style_pad_all(0, lv.STATE.DEFAULT)
ANModel_Training_btnANModelTrainStart_label.align(lv.ALIGN.CENTER,0,0)
# create style style_anmodel_training_btnanmodeltrainstart_main_main_default
style_anmodel_training_btnanmodeltrainstart_main_main_default = lv.style_t()
style_anmodel_training_btnanmodeltrainstart_main_main_default.init()
style_anmodel_training_btnanmodeltrainstart_main_main_default.set_radius(10)
style_anmodel_training_btnanmodeltrainstart_main_main_default.set_bg_color(lv.color_make(0x2F,0x92,0xDA))
style_anmodel_training_btnanmodeltrainstart_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_anmodel_training_btnanmodeltrainstart_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_anmodel_training_btnanmodeltrainstart_main_main_default.set_bg_opa(255)
style_anmodel_training_btnanmodeltrainstart_main_main_default.set_border_color(lv.color_make(0x21,0x95,0xf6))
style_anmodel_training_btnanmodeltrainstart_main_main_default.set_border_width(0)
style_anmodel_training_btnanmodeltrainstart_main_main_default.set_border_opa(255)
style_anmodel_training_btnanmodeltrainstart_main_main_default.set_text_color(lv.color_make(0xff,0xff,0xff))
try:
    style_anmodel_training_btnanmodeltrainstart_main_main_default.set_text_font(lv.font_montserratMedium_16)
except AttributeError:
    try:
        style_anmodel_training_btnanmodeltrainstart_main_main_default.set_text_font(lv.font_montserrat_16)
    except AttributeError:
        style_anmodel_training_btnanmodeltrainstart_main_main_default.set_text_font(lv.font_montserrat_16)
style_anmodel_training_btnanmodeltrainstart_main_main_default.set_text_align(lv.TEXT_ALIGN.CENTER)

# add style for ANModel_Training_btnANModelTrainStart
ANModel_Training_btnANModelTrainStart.add_style(style_anmodel_training_btnanmodeltrainstart_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create Model_Training
Model_Training = lv.obj()
Model_Training.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# create style style_model_training_main_main_default
style_model_training_main_main_default = lv.style_t()
style_model_training_main_main_default.init()
style_model_training_main_main_default.set_bg_color(lv.color_make(0xff,0xff,0xff))
style_model_training_main_main_default.set_bg_grad_color(lv.color_make(0x2F,0x92,0xDA))
style_model_training_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.VER)
style_model_training_main_main_default.set_bg_opa(255)

# add style for Model_Training
Model_Training.add_style(style_model_training_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create Model_Training_imgModelTrainNXP
Model_Training_imgModelTrainNXP = lv.img(Model_Training)
Model_Training_imgModelTrainNXP.set_pos(int(678),int(419))
Model_Training_imgModelTrainNXP.set_size(100,37)
Model_Training_imgModelTrainNXP.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
Model_Training_imgModelTrainNXP.add_flag(lv.obj.FLAG.CLICKABLE)
try:
    with open('C:\\LocalData\\gui\\anormaly_detect_mcx\\HMI_MCX947_GG151\\generated\\mPythonImages\\mp-284701175.png','rb') as f:
        Model_Training_imgModelTrainNXP_img_data = f.read()
except:
    print('Could not open C:\\LocalData\\gui\\anormaly_detect_mcx\\HMI_MCX947_GG151\\generated\\mPythonImages\\mp-284701175.png')
    sys.exit()

Model_Training_imgModelTrainNXP_img = lv.img_dsc_t({
  'data_size': len(Model_Training_imgModelTrainNXP_img_data),
  'header': {'always_zero': 0, 'w': 100, 'h': 37, 'cf': lv.img.CF.TRUE_COLOR_ALPHA},
  'data': Model_Training_imgModelTrainNXP_img_data
})

Model_Training_imgModelTrainNXP.set_src(Model_Training_imgModelTrainNXP_img)
Model_Training_imgModelTrainNXP.set_pivot(50,50)
Model_Training_imgModelTrainNXP.set_angle(0)
# create style style_model_training_imgmodeltrainnxp_main_main_default
style_model_training_imgmodeltrainnxp_main_main_default = lv.style_t()
style_model_training_imgmodeltrainnxp_main_main_default.init()
style_model_training_imgmodeltrainnxp_main_main_default.set_img_recolor(lv.color_make(0xff,0xff,0xff))
style_model_training_imgmodeltrainnxp_main_main_default.set_img_recolor_opa(0)
style_model_training_imgmodeltrainnxp_main_main_default.set_img_opa(255)

# add style for Model_Training_imgModelTrainNXP
Model_Training_imgModelTrainNXP.add_style(style_model_training_imgmodeltrainnxp_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create Model_Training_labelModelTrainCurFunc
Model_Training_labelModelTrainCurFunc = lv.label(Model_Training)
Model_Training_labelModelTrainCurFunc.set_pos(int(38),int(43))
Model_Training_labelModelTrainCurFunc.set_size(216,43)
Model_Training_labelModelTrainCurFunc.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
Model_Training_labelModelTrainCurFunc.set_text("States Monitor")
Model_Training_labelModelTrainCurFunc.set_long_mode(lv.label.LONG.WRAP)
# create style style_model_training_labelmodeltraincurfunc_main_main_default
style_model_training_labelmodeltraincurfunc_main_main_default = lv.style_t()
style_model_training_labelmodeltraincurfunc_main_main_default.init()
style_model_training_labelmodeltraincurfunc_main_main_default.set_radius(0)
style_model_training_labelmodeltraincurfunc_main_main_default.set_bg_color(lv.color_make(0x52,0xe3,0x24))
style_model_training_labelmodeltraincurfunc_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_model_training_labelmodeltraincurfunc_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_model_training_labelmodeltraincurfunc_main_main_default.set_bg_opa(0)
style_model_training_labelmodeltraincurfunc_main_main_default.set_text_color(lv.color_make(0x0d,0x30,0x08))
try:
    style_model_training_labelmodeltraincurfunc_main_main_default.set_text_font(lv.font_montserratMedium_20)
except AttributeError:
    try:
        style_model_training_labelmodeltraincurfunc_main_main_default.set_text_font(lv.font_montserrat_20)
    except AttributeError:
        style_model_training_labelmodeltraincurfunc_main_main_default.set_text_font(lv.font_montserrat_16)
style_model_training_labelmodeltraincurfunc_main_main_default.set_text_letter_space(2)
style_model_training_labelmodeltraincurfunc_main_main_default.set_text_line_space(0)
style_model_training_labelmodeltraincurfunc_main_main_default.set_text_align(lv.TEXT_ALIGN.LEFT)
style_model_training_labelmodeltraincurfunc_main_main_default.set_pad_left(0)
style_model_training_labelmodeltraincurfunc_main_main_default.set_pad_right(0)
style_model_training_labelmodeltraincurfunc_main_main_default.set_pad_top(8)
style_model_training_labelmodeltraincurfunc_main_main_default.set_pad_bottom(0)

# add style for Model_Training_labelModelTrainCurFunc
Model_Training_labelModelTrainCurFunc.add_style(style_model_training_labelmodeltraincurfunc_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create Model_Training_btnModelTrainReturn
Model_Training_btnModelTrainReturn = lv.btn(Model_Training)
Model_Training_btnModelTrainReturn.set_pos(int(496),int(340))
Model_Training_btnModelTrainReturn.set_size(112,50)
Model_Training_btnModelTrainReturn.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
Model_Training_btnModelTrainReturn_label = lv.label(Model_Training_btnModelTrainReturn)
Model_Training_btnModelTrainReturn_label.set_text("Return")
Model_Training_btnModelTrainReturn.set_style_pad_all(0, lv.STATE.DEFAULT)
Model_Training_btnModelTrainReturn_label.align(lv.ALIGN.CENTER,0,0)
# create style style_model_training_btnmodeltrainreturn_main_main_default
style_model_training_btnmodeltrainreturn_main_main_default = lv.style_t()
style_model_training_btnmodeltrainreturn_main_main_default.init()
style_model_training_btnmodeltrainreturn_main_main_default.set_radius(10)
style_model_training_btnmodeltrainreturn_main_main_default.set_bg_color(lv.color_make(0x2F,0x92,0xDA))
style_model_training_btnmodeltrainreturn_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_model_training_btnmodeltrainreturn_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_model_training_btnmodeltrainreturn_main_main_default.set_bg_opa(255)
style_model_training_btnmodeltrainreturn_main_main_default.set_border_color(lv.color_make(0x21,0x95,0xf6))
style_model_training_btnmodeltrainreturn_main_main_default.set_border_width(0)
style_model_training_btnmodeltrainreturn_main_main_default.set_border_opa(255)
style_model_training_btnmodeltrainreturn_main_main_default.set_text_color(lv.color_make(0xff,0xff,0xff))
try:
    style_model_training_btnmodeltrainreturn_main_main_default.set_text_font(lv.font_montserratMedium_16)
except AttributeError:
    try:
        style_model_training_btnmodeltrainreturn_main_main_default.set_text_font(lv.font_montserrat_16)
    except AttributeError:
        style_model_training_btnmodeltrainreturn_main_main_default.set_text_font(lv.font_montserrat_16)
style_model_training_btnmodeltrainreturn_main_main_default.set_text_align(lv.TEXT_ALIGN.CENTER)

# add style for Model_Training_btnModelTrainReturn
Model_Training_btnModelTrainReturn.add_style(style_model_training_btnmodeltrainreturn_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create Model_Training_labelModelTrainInfo
Model_Training_labelModelTrainInfo = lv.label(Model_Training)
Model_Training_labelModelTrainInfo.set_pos(int(38),int(117))
Model_Training_labelModelTrainInfo.set_size(216,43)
Model_Training_labelModelTrainInfo.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
Model_Training_labelModelTrainInfo.set_text("Information")
Model_Training_labelModelTrainInfo.set_long_mode(lv.label.LONG.WRAP)
# create style style_model_training_labelmodeltraininfo_main_main_default
style_model_training_labelmodeltraininfo_main_main_default = lv.style_t()
style_model_training_labelmodeltraininfo_main_main_default.init()
style_model_training_labelmodeltraininfo_main_main_default.set_radius(0)
style_model_training_labelmodeltraininfo_main_main_default.set_bg_color(lv.color_make(0x52,0xe3,0x24))
style_model_training_labelmodeltraininfo_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_model_training_labelmodeltraininfo_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_model_training_labelmodeltraininfo_main_main_default.set_bg_opa(0)
style_model_training_labelmodeltraininfo_main_main_default.set_text_color(lv.color_make(0x0d,0x30,0x08))
try:
    style_model_training_labelmodeltraininfo_main_main_default.set_text_font(lv.font_montserratMedium_20)
except AttributeError:
    try:
        style_model_training_labelmodeltraininfo_main_main_default.set_text_font(lv.font_montserrat_20)
    except AttributeError:
        style_model_training_labelmodeltraininfo_main_main_default.set_text_font(lv.font_montserrat_16)
style_model_training_labelmodeltraininfo_main_main_default.set_text_letter_space(2)
style_model_training_labelmodeltraininfo_main_main_default.set_text_line_space(0)
style_model_training_labelmodeltraininfo_main_main_default.set_text_align(lv.TEXT_ALIGN.LEFT)
style_model_training_labelmodeltraininfo_main_main_default.set_pad_left(0)
style_model_training_labelmodeltraininfo_main_main_default.set_pad_right(0)
style_model_training_labelmodeltraininfo_main_main_default.set_pad_top(8)
style_model_training_labelmodeltraininfo_main_main_default.set_pad_bottom(0)

# add style for Model_Training_labelModelTrainInfo
Model_Training_labelModelTrainInfo.add_style(style_model_training_labelmodeltraininfo_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create Model_Training_labelModelTrainStat
Model_Training_labelModelTrainStat = lv.label(Model_Training)
Model_Training_labelModelTrainStat.set_pos(int(419),int(74))
Model_Training_labelModelTrainStat.set_size(216,43)
Model_Training_labelModelTrainStat.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
Model_Training_labelModelTrainStat.set_text("Status")
Model_Training_labelModelTrainStat.set_long_mode(lv.label.LONG.WRAP)
# create style style_model_training_labelmodeltrainstat_main_main_default
style_model_training_labelmodeltrainstat_main_main_default = lv.style_t()
style_model_training_labelmodeltrainstat_main_main_default.init()
style_model_training_labelmodeltrainstat_main_main_default.set_radius(0)
style_model_training_labelmodeltrainstat_main_main_default.set_bg_color(lv.color_make(0x52,0xe3,0x24))
style_model_training_labelmodeltrainstat_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_model_training_labelmodeltrainstat_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_model_training_labelmodeltrainstat_main_main_default.set_bg_opa(0)
style_model_training_labelmodeltrainstat_main_main_default.set_text_color(lv.color_make(0x0d,0x30,0x08))
try:
    style_model_training_labelmodeltrainstat_main_main_default.set_text_font(lv.font_montserratMedium_20)
except AttributeError:
    try:
        style_model_training_labelmodeltrainstat_main_main_default.set_text_font(lv.font_montserrat_20)
    except AttributeError:
        style_model_training_labelmodeltrainstat_main_main_default.set_text_font(lv.font_montserrat_16)
style_model_training_labelmodeltrainstat_main_main_default.set_text_letter_space(2)
style_model_training_labelmodeltrainstat_main_main_default.set_text_line_space(0)
style_model_training_labelmodeltrainstat_main_main_default.set_text_align(lv.TEXT_ALIGN.CENTER)
style_model_training_labelmodeltrainstat_main_main_default.set_pad_left(0)
style_model_training_labelmodeltrainstat_main_main_default.set_pad_right(0)
style_model_training_labelmodeltrainstat_main_main_default.set_pad_top(8)
style_model_training_labelmodeltrainstat_main_main_default.set_pad_bottom(0)

# add style for Model_Training_labelModelTrainStat
Model_Training_labelModelTrainStat.add_style(style_model_training_labelmodeltrainstat_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create Model_Training_barModelTrainProgress
Model_Training_barModelTrainProgress = lv.bar(Model_Training)
Model_Training_barModelTrainProgress.set_pos(int(138),int(227))
Model_Training_barModelTrainProgress.set_size(525,26)
Model_Training_barModelTrainProgress.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
Model_Training_barModelTrainProgress.set_style_anim_time(1000, 0)
Model_Training_barModelTrainProgress.set_mode(lv.bar.MODE.NORMAL)
Model_Training_barModelTrainProgress.set_value(50, lv.ANIM.OFF)
# create style style_model_training_barmodeltrainprogress_main_main_default
style_model_training_barmodeltrainprogress_main_main_default = lv.style_t()
style_model_training_barmodeltrainprogress_main_main_default.init()
style_model_training_barmodeltrainprogress_main_main_default.set_radius(10)
style_model_training_barmodeltrainprogress_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_model_training_barmodeltrainprogress_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_model_training_barmodeltrainprogress_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_model_training_barmodeltrainprogress_main_main_default.set_bg_opa(60)

# add style for Model_Training_barModelTrainProgress
Model_Training_barModelTrainProgress.add_style(style_model_training_barmodeltrainprogress_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

# create style style_model_training_barmodeltrainprogress_main_indicator_default
style_model_training_barmodeltrainprogress_main_indicator_default = lv.style_t()
style_model_training_barmodeltrainprogress_main_indicator_default.init()
style_model_training_barmodeltrainprogress_main_indicator_default.set_radius(10)
style_model_training_barmodeltrainprogress_main_indicator_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_model_training_barmodeltrainprogress_main_indicator_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_model_training_barmodeltrainprogress_main_indicator_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_model_training_barmodeltrainprogress_main_indicator_default.set_bg_opa(255)

# add style for Model_Training_barModelTrainProgress
Model_Training_barModelTrainProgress.add_style(style_model_training_barmodeltrainprogress_main_indicator_default, lv.PART.INDICATOR|lv.STATE.DEFAULT)


# create Model_Training_btnModelTrainStart
Model_Training_btnModelTrainStart = lv.btn(Model_Training)
Model_Training_btnModelTrainStart.set_pos(int(192),int(340))
Model_Training_btnModelTrainStart.set_size(112,50)
Model_Training_btnModelTrainStart.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
Model_Training_btnModelTrainStart_label = lv.label(Model_Training_btnModelTrainStart)
Model_Training_btnModelTrainStart_label.set_text("Start")
Model_Training_btnModelTrainStart.set_style_pad_all(0, lv.STATE.DEFAULT)
Model_Training_btnModelTrainStart_label.align(lv.ALIGN.CENTER,0,0)
# create style style_model_training_btnmodeltrainstart_main_main_default
style_model_training_btnmodeltrainstart_main_main_default = lv.style_t()
style_model_training_btnmodeltrainstart_main_main_default.init()
style_model_training_btnmodeltrainstart_main_main_default.set_radius(10)
style_model_training_btnmodeltrainstart_main_main_default.set_bg_color(lv.color_make(0x2F,0x92,0xDA))
style_model_training_btnmodeltrainstart_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_model_training_btnmodeltrainstart_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_model_training_btnmodeltrainstart_main_main_default.set_bg_opa(255)
style_model_training_btnmodeltrainstart_main_main_default.set_border_color(lv.color_make(0x21,0x95,0xf6))
style_model_training_btnmodeltrainstart_main_main_default.set_border_width(0)
style_model_training_btnmodeltrainstart_main_main_default.set_border_opa(255)
style_model_training_btnmodeltrainstart_main_main_default.set_text_color(lv.color_make(0xff,0xff,0xff))
try:
    style_model_training_btnmodeltrainstart_main_main_default.set_text_font(lv.font_montserratMedium_16)
except AttributeError:
    try:
        style_model_training_btnmodeltrainstart_main_main_default.set_text_font(lv.font_montserrat_16)
    except AttributeError:
        style_model_training_btnmodeltrainstart_main_main_default.set_text_font(lv.font_montserrat_16)
style_model_training_btnmodeltrainstart_main_main_default.set_text_align(lv.TEXT_ALIGN.CENTER)

# add style for Model_Training_btnModelTrainStart
Model_Training_btnModelTrainStart.add_style(style_model_training_btnmodeltrainstart_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create screen_1
screen_1 = lv.obj()
screen_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# create style style_screen_1_main_main_default
style_screen_1_main_main_default = lv.style_t()
style_screen_1_main_main_default.init()
style_screen_1_main_main_default.set_bg_color(lv.color_make(0xff,0xff,0xff))
style_screen_1_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_1_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_1_main_main_default.set_bg_opa(0)

# add style for screen_1
screen_1.add_style(style_screen_1_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create screen_1_cont_1
screen_1_cont_1 = lv.obj(screen_1)
screen_1_cont_1.set_pos(int(37),int(101))
screen_1_cont_1.set_size(254,129)
screen_1_cont_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)

# create screen_1_cb_2
screen_1_cb_2 = lv.checkbox(screen_1_cont_1)
screen_1_cb_2.set_pos(int(14),int(73))
screen_1_cb_2.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_1_cb_2.set_text("State Monitor")
# create style style_screen_1_cb_2_main_main_default
style_screen_1_cb_2_main_main_default = lv.style_t()
style_screen_1_cb_2_main_main_default.init()
style_screen_1_cb_2_main_main_default.set_radius(6)
style_screen_1_cb_2_main_main_default.set_bg_color(lv.color_make(0x52,0xe3,0x24))
style_screen_1_cb_2_main_main_default.set_bg_grad_color(lv.color_make(0xff,0xff,0xff))
style_screen_1_cb_2_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_1_cb_2_main_main_default.set_bg_opa(0)
style_screen_1_cb_2_main_main_default.set_border_color(lv.color_make(0xe1,0xe6,0xee))
style_screen_1_cb_2_main_main_default.set_border_width(0)
style_screen_1_cb_2_main_main_default.set_border_opa(0)
style_screen_1_cb_2_main_main_default.set_text_color(lv.color_make(0x0D,0x30,0x55))
try:
    style_screen_1_cb_2_main_main_default.set_text_font(lv.font_montserratMedium_20)
except AttributeError:
    try:
        style_screen_1_cb_2_main_main_default.set_text_font(lv.font_montserrat_20)
    except AttributeError:
        style_screen_1_cb_2_main_main_default.set_text_font(lv.font_montserrat_16)
style_screen_1_cb_2_main_main_default.set_text_letter_space(2)

# add style for screen_1_cb_2
screen_1_cb_2.add_style(style_screen_1_cb_2_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

# create style style_screen_1_cb_2_main_indicator_default
style_screen_1_cb_2_main_indicator_default = lv.style_t()
style_screen_1_cb_2_main_indicator_default.init()
style_screen_1_cb_2_main_indicator_default.set_radius(6)
style_screen_1_cb_2_main_indicator_default.set_bg_color(lv.color_make(0xff,0xff,0xff))
style_screen_1_cb_2_main_indicator_default.set_bg_grad_color(lv.color_make(0xff,0xff,0xff))
style_screen_1_cb_2_main_indicator_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_1_cb_2_main_indicator_default.set_bg_opa(255)
style_screen_1_cb_2_main_indicator_default.set_border_color(lv.color_make(0x21,0x95,0xf6))
style_screen_1_cb_2_main_indicator_default.set_border_width(2)
style_screen_1_cb_2_main_indicator_default.set_border_opa(255)

# add style for screen_1_cb_2
screen_1_cb_2.add_style(style_screen_1_cb_2_main_indicator_default, lv.PART.INDICATOR|lv.STATE.DEFAULT)


# create screen_1_cb_1
screen_1_cb_1 = lv.checkbox(screen_1_cont_1)
screen_1_cb_1.set_pos(int(13),int(22))
screen_1_cb_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_1_cb_1.set_text("Anomaly Detect")
# create style style_screen_1_cb_1_main_main_default
style_screen_1_cb_1_main_main_default = lv.style_t()
style_screen_1_cb_1_main_main_default.init()
style_screen_1_cb_1_main_main_default.set_radius(6)
style_screen_1_cb_1_main_main_default.set_bg_color(lv.color_make(0x52,0xe3,0x24))
style_screen_1_cb_1_main_main_default.set_bg_grad_color(lv.color_make(0xff,0xff,0xff))
style_screen_1_cb_1_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_1_cb_1_main_main_default.set_bg_opa(0)
style_screen_1_cb_1_main_main_default.set_border_color(lv.color_make(0xe1,0xe6,0xee))
style_screen_1_cb_1_main_main_default.set_border_width(0)
style_screen_1_cb_1_main_main_default.set_border_opa(0)
style_screen_1_cb_1_main_main_default.set_text_color(lv.color_make(0x0D,0x30,0x55))
try:
    style_screen_1_cb_1_main_main_default.set_text_font(lv.font_montserratMedium_20)
except AttributeError:
    try:
        style_screen_1_cb_1_main_main_default.set_text_font(lv.font_montserrat_20)
    except AttributeError:
        style_screen_1_cb_1_main_main_default.set_text_font(lv.font_montserrat_16)
style_screen_1_cb_1_main_main_default.set_text_letter_space(2)

# add style for screen_1_cb_1
screen_1_cb_1.add_style(style_screen_1_cb_1_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

# create style style_screen_1_cb_1_main_indicator_default
style_screen_1_cb_1_main_indicator_default = lv.style_t()
style_screen_1_cb_1_main_indicator_default.init()
style_screen_1_cb_1_main_indicator_default.set_radius(6)
style_screen_1_cb_1_main_indicator_default.set_bg_color(lv.color_make(0xff,0xff,0xff))
style_screen_1_cb_1_main_indicator_default.set_bg_grad_color(lv.color_make(0xff,0xff,0xff))
style_screen_1_cb_1_main_indicator_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_1_cb_1_main_indicator_default.set_bg_opa(255)
style_screen_1_cb_1_main_indicator_default.set_border_color(lv.color_make(0x21,0x95,0xf6))
style_screen_1_cb_1_main_indicator_default.set_border_width(2)
style_screen_1_cb_1_main_indicator_default.set_border_opa(255)

# add style for screen_1_cb_1
screen_1_cb_1.add_style(style_screen_1_cb_1_main_indicator_default, lv.PART.INDICATOR|lv.STATE.DEFAULT)

# create style style_screen_1_cont_1_main_main_default
style_screen_1_cont_1_main_main_default = lv.style_t()
style_screen_1_cont_1_main_main_default.init()
style_screen_1_cont_1_main_main_default.set_radius(0)
style_screen_1_cont_1_main_main_default.set_bg_color(lv.color_make(0x2F,0x35,0xDA))
style_screen_1_cont_1_main_main_default.set_bg_grad_color(lv.color_make(0x2F,0xCA,0xDA))
style_screen_1_cont_1_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.VER)
style_screen_1_cont_1_main_main_default.set_bg_opa(40)
style_screen_1_cont_1_main_main_default.set_border_color(lv.color_make(0xae,0xb3,0xfe))
style_screen_1_cont_1_main_main_default.set_border_width(2)
style_screen_1_cont_1_main_main_default.set_border_opa(128)
style_screen_1_cont_1_main_main_default.set_pad_left(0)
style_screen_1_cont_1_main_main_default.set_pad_right(0)
style_screen_1_cont_1_main_main_default.set_pad_top(0)
style_screen_1_cont_1_main_main_default.set_pad_bottom(0)

# add style for screen_1_cont_1
screen_1_cont_1.add_style(style_screen_1_cont_1_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)


# create screen_1_img_1
screen_1_img_1 = lv.img(screen_1)
screen_1_img_1.set_pos(int(378),int(110))
screen_1_img_1.set_size(160,160)
screen_1_img_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_1_img_1.add_flag(lv.obj.FLAG.CLICKABLE)
try:
    with open('C:\\LocalData\\gui\\anormaly_detect_mcx\\HMI_MCX947_GG151\\generated\\mPythonImages\\mp966077848.png','rb') as f:
        screen_1_img_1_img_data = f.read()
except:
    print('Could not open C:\\LocalData\\gui\\anormaly_detect_mcx\\HMI_MCX947_GG151\\generated\\mPythonImages\\mp966077848.png')
    sys.exit()

screen_1_img_1_img = lv.img_dsc_t({
  'data_size': len(screen_1_img_1_img_data),
  'header': {'always_zero': 0, 'w': 160, 'h': 160, 'cf': lv.img.CF.TRUE_COLOR_ALPHA},
  'data': screen_1_img_1_img_data
})

screen_1_img_1.set_src(screen_1_img_1_img)
screen_1_img_1.set_pivot(50,50)
screen_1_img_1.set_angle(0)
# create style style_screen_1_img_1_main_main_default
style_screen_1_img_1_main_main_default = lv.style_t()
style_screen_1_img_1_main_main_default.init()
style_screen_1_img_1_main_main_default.set_img_recolor(lv.color_make(0xff,0xff,0xff))
style_screen_1_img_1_main_main_default.set_img_recolor_opa(0)
style_screen_1_img_1_main_main_default.set_img_opa(255)

# add style for screen_1_img_1
screen_1_img_1.add_style(style_screen_1_img_1_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)



def ANModel_Training_btnANModelTrainReturn_clicked_1_event_cb(e,Main):
    src = e.get_target()
    code = e.get_code()
    lv.scr_load_anim(Main, lv.SCR_LOAD_ANIM.NONE, 100, 100, False)
ANModel_Training_btnANModelTrainReturn.add_event_cb(lambda e: ANModel_Training_btnANModelTrainReturn_clicked_1_event_cb(e,Main), lv.EVENT.CLICKED, None)


def Main_btnMainSwScr_clicked_1_event_cb(e,Model_Training):
    src = e.get_target()
    code = e.get_code()
    lv.scr_load_anim(Model_Training, lv.SCR_LOAD_ANIM.NONE, 100, 100, False)
Main_btnMainSwScr.add_event_cb(lambda e: Main_btnMainSwScr_clicked_1_event_cb(e,Model_Training), lv.EVENT.CLICKED, None)


def Model_Training_btnModelTrainReturn_clicked_1_event_cb(e,Main):
    src = e.get_target()
    code = e.get_code()
    lv.scr_load_anim(Main, lv.SCR_LOAD_ANIM.NONE, 100, 100, False)
Model_Training_btnModelTrainReturn.add_event_cb(lambda e: Model_Training_btnModelTrainReturn_clicked_1_event_cb(e,Main), lv.EVENT.CLICKED, None)



# content from custom.py
def widgets_move():
    switch_widget_img_guider_event_move_x = lv.anim_t()
    switch_widget_img_guider_event_move_x.init()
    switch_widget_img_guider_event_move_x.set_var(switch_widget_img_guider)
    switch_widget_img_guider_event_move_x.set_values(switch_widget_img_guider.get_x(), int(40 * disp_drv.ver_res / 272))
    switch_widget_img_guider_event_move_x.set_time(1000)
    switch_widget_img_guider_event_move_x.set_path_cb(lv.anim_t.path_bounce)
    switch_widget_img_guider_event_move_x.set_custom_exec_cb(lambda a,val: anim_x_cb(switch_widget_img_guider,val))
    lv.anim_t.start(switch_widget_img_guider_event_move_x)
    switch_widget_img_guider_event_move_y = lv.anim_t()
    switch_widget_img_guider_event_move_y.init()
    switch_widget_img_guider_event_move_y.set_var(switch_widget_img_guider)
    switch_widget_img_guider_event_move_y.set_values(switch_widget_img_guider.get_y(), int(62 * disp_drv.hor_res / 480))
    switch_widget_img_guider_event_move_y.set_time(1000)
    switch_widget_img_guider_event_move_y.set_path_cb(lv.anim_t.path_bounce)
    switch_widget_img_guider_event_move_y.set_custom_exec_cb(lambda a,val: anim_y_cb(switch_widget_img_guider,val))
    lv.anim_t.start(switch_widget_img_guider_event_move_y)

    switch_widget_qrcode_community.clear_flag(lv.obj.FLAG.HIDDEN)

    switch_widget_community_event_move_x = lv.anim_t()
    switch_widget_community_event_move_x.init()
    switch_widget_community_event_move_x.set_var(switch_widget_qrcode_community)
    if(disp_drv.ver_res < 480):
        switch_widget_community_event_move_x.set_values(switch_widget_qrcode_community.get_x(), int(220 * disp_drv.ver_res /272))
    else:
        switch_widget_community_event_move_x.set_values(switch_widget_qrcode_community.get_x(), int(300 * disp_drv.ver_res /272))
    switch_widget_community_event_move_x.set_time(1000)
    switch_widget_community_event_move_x.set_path_cb(lv.anim_t.path_overshoot)
    switch_widget_community_event_move_x.set_custom_exec_cb(lambda a,val: anim_x_cb(switch_widget_qrcode_community,val))
    lv.anim_t.start(switch_widget_community_event_move_x)
    switch_widget_community_event_move_y = lv.anim_t()
    switch_widget_community_event_move_y.init()
    switch_widget_community_event_move_y.set_var(switch_widget_qrcode_community)
    switch_widget_community_event_move_y.set_values(switch_widget_qrcode_community.get_y(), int(62 * disp_drv.hor_res / 480))
    switch_widget_community_event_move_y.set_time(1000)
    switch_widget_community_event_move_y.set_path_cb(lv.anim_t.path_overshoot)
    switch_widget_community_event_move_y.set_custom_exec_cb(lambda a,val: anim_y_cb(switch_widget_qrcode_community,val))
    lv.anim_t.start(switch_widget_community_event_move_y)

    if(not switch_widget_sw_switch.has_state(lv.STATE.CHECKED)):
        switch_widget_img_guider_event_move_x = lv.anim_t()
        switch_widget_img_guider_event_move_x.init()
        switch_widget_img_guider_event_move_x.set_var(switch_widget_img_guider)
        switch_widget_img_guider_event_move_x.set_values(switch_widget_img_guider.get_x(), int(130 * disp_drv.ver_res /272))
        switch_widget_img_guider_event_move_x.set_time(1000)
        switch_widget_img_guider_event_move_x.set_path_cb(lv.anim_t.path_bounce)
        switch_widget_img_guider_event_move_x.set_custom_exec_cb(lambda a,val: anim_x_cb(switch_widget_img_guider,val))
        lv.anim_t.start(switch_widget_img_guider_event_move_x)

        switch_widget_community_event_move_x = lv.anim_t()
        switch_widget_community_event_move_x.init()
        switch_widget_community_event_move_x.set_var(switch_widget_qrcode_community)
        switch_widget_community_event_move_x.set_values(switch_widget_qrcode_community.get_x(), int(102 * disp_drv.ver_res /272))
        switch_widget_community_event_move_x.set_time(1000)
        switch_widget_community_event_move_x.set_path_cb(lv.anim_t.path_overshoot)
        switch_widget_community_event_move_x.set_custom_exec_cb(lambda a,val: anim_x_cb(switch_widget_qrcode_community,val))
        lv.anim_t.start(switch_widget_community_event_move_x)

        switch_widget_qrcode_community.add_flag(lv.obj.FLAG.HIDDEN)

# Load the default screen
lv.scr_load(Main)

while SDL.check():
    time.sleep_ms(5)
