# NXP Application Code Hub

[<img src="https://mcuxpresso.nxp.com/static/icon/nxp-logo-color.svg" width="100"/>](https://www.nxp.com)

## On-device training fan anomaly detection on MCXN947

On-device trainable anomaly detection based on the MCX platform. Train the SVM model on-device with normal accelerometer data
and inference SVM model on-device too. 

Because data patterns are often very different or vary among the device life cycle.

During the whole life cycle of a device, learning may need to be done multiple times.

The GUI is implemented by LVGL.

![](image/diagram.png)

![](image/data_flow.png)

#### Boards: FRDM-MCXN947

#### Categories: AI/ML, Anomaly Detection

#### Peripherals: DISPLAY, I2C

#### Toolchains: MCUXpresso IDE, VS Code

## Table of Contents

1. [Software](#step1)
2. [Hardware](#step2)
3. [Setup](#step3)
4. [Results](#step4)
5. [FAQs](#step5)
6. [Support](#step6)
7. [Release Notes](#step7)

## 1. Software <a name="step1"></a>

- Download [SDK_2_14_0_FRDM-MCXN947](https://mcuxpresso.nxp.com/en/welcome)
- Download and install [MCUXpresso IDE V11.9.0 or later](https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE).

## 2. Hardware <a name="step2"></a>

* 3.5" TFT LCD module by NXP (P/N PAR-LCD-S035)
* FRDM-MCXN947(SCH-90818_REV B) board
* IMU(Inertial Measurement Unit) Sensor: MPU6050 module or FXLS8974 [ACCEL-4-CLICK](https://www.mikroe.com/accel-4-click)
* FAN
* Type-C USB cable

## 3. Setup <a name="step3"></a>

### 3.1 Step 1 Setup hardware

* Stick the IMU(6050) Sensor on the FAN like:

![](image/fan_with_imu.jpg)

or IMU(FXLS8974)

![](image/fan_with_fxls8974.jpg)

* Connect IMU with FRDM-MCXN947 board, connection is shown in the table:

![](image/imu_connections.png)

* Connect LCD Panel with J8 FRDM-MCXN947 board.
* The whole system is shown as:

IMU(MPU6050)

![](image/whole_system.jpg)

or IMU(FXLS8974)

![](image/whole_system_fxl8974.jpg)

Connect the debug port on board with the laptop.

![](image/debug_connection.png)

### 3.2 Step 2

#### Develop in MCUXpresso IDE

Import the project into MCUXpresso IDE, click 'Import project from Application Code Hub', search 'on-device training fan anomaly on mcxn947' example, and clone to the local workspace.

![](image/mcux%20_import_project.png)

Build the project, after compilation is complete, use the GUI Flash Tool (2 in the following figure) to write the program to the board.

![](image/build_download.png)


#### Develop in VS code

In VS code, select the 'MCUXpresso For VScode' plugin, and click 'Application Code Hub' in the QUICKSTART PANEL. 
search 'on-device training fan anomaly on mcxn947' example, clone to local workspace.

After a while, the project is shown in the 'Projects'.

![](image/vs_import_project.png)

Build the project, after compile complete flash the board.

![](image/vs_build_project.png)


## 4. Results <a name="step4"></a>

Reset board, the main windows showing as:

![](image/main_window.png)

### 4.1 Normal state training

Set the fan in the normal state, click the 'Train' button, and enter the training window. 

![](image/training_window.jpg)

Click the start button to start training. Notice that don't move or touch the fun during the training.

After training is complete, click return back to the main window.
The window shows the normal status, Green LED at the top is on.

![](image/result_normal.jpg)

### 4.2 Anomaly state detection

Flip the Fan over, then the data of IMU is changed, and then the normal status is detected. The red LED at the top is on.
  
![](image/result_anormal.jpg)

### 4.3 Other scenario

* Cover the fan then anomaly state is detected.
  
![](image/cover_anomaly.png)
  
* Retrain the model, after training complete state is normal.

![](image/other_scenario_normally.png)

* Remove the cover, then the anomaly state is detected.

![](image/other_scenario_anomaly.png)

For more details about the demo, please refer to the video. 

(**image/how_to_run.mp4**).

<video id="video" controls="" preload="none" poster="tony">
<source id="mp4" src="./image/how_to_run.mp4" type="video/mp4">
</video>

## 5. FAQs <a name="step5"></a>

*No FAQs have been identified for this project.*

## 6. Support <a name="step6"></a>

*Please contact NXP for additional support.*

#### Project Metadata

<!----- Boards ----->

[![Board badge](https://img.shields.io/badge/Board-MCX–N9XX–BRK-blue)](https://github.com/search?q=org%3Anxp-appcodehub+MCX-N9XX-BRK+in%3Areadme&type=Repositories)

<!----- Categories ----->

[![Category badge](https://img.shields.io/badge/Category-AI/ML-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+aiml+in%3Areadme&type=Repositories) [![Category badge](https://img.shields.io/badge/Category-ANOMALY%20DETECTION-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+anomaly_detection+in%3Areadme&type=Repositories)

<!----- Peripherals ----->

[![Peripheral badge](https://img.shields.io/badge/Peripheral-DISPLAY-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+display+in%3Areadme&type=Repositories) [![Peripheral badge](https://img.shields.io/badge/Peripheral-I2C-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+i2c+in%3Areadme&type=Repositories)

<!----- Toolchains ----->

[![Toolchain badge](https://img.shields.io/badge/Toolchain-MCUXPRESSO%20IDE-orange)](https://github.com/search?q=org%3Anxp-appcodehub+mcux+in%3Areadme&type=Repositories)

Questions regarding the content/correctness of this example can be entered as Issues within this GitHub repository.

> **Warning**: For more general technical questions regarding NXP Microcontrollers and the difference in expected funcionality, enter your questions on the [NXP Community Forum](https://community.nxp.com/)

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/@NXP_Semiconductors)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/nxp-semiconductors)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/nxpsemi/)
[![Follow us on Twitter](https://img.shields.io/badge/Twitter-Follow%20us%20on%20Twitter-white.svg)](https://twitter.com/NXP)

## 7. Release Notes <a name="step7"></a>

| Version | Description / Update                    |                                   Date |
| :-----: | --------------------------------------- | -------------------------------------: |
|   1.0   | Initial release on Application Code Hub | January 30 <sup>th </sup> 2024 |