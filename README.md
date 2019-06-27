# libyuv-wrapper

  YUV的I420的压缩，缩放，裁剪，缩放</br>
  i420和nv21,rgba之间转换的工具库</br>
  nv21:一般android的相机数据格式</br>
  rgba:android的jni绘制数据格式</br>

** YUV格式 **

| 格式       | Mode | 通道 | 比特 | 示例                                                                    |
| --------- |------|------|------|-------------------------------------------------------------------------|
| GRAY      |Planar|1|8 |400. Y0Y1...Y15|
| I420      |Planar|3|12|420. Y0Y1..Y15  U0U1U2U3 V0V1V2V3   * 常用 理解为YU12|
| IYUV      |Planar|3|12|同I420|
| YUV420P   |Planar|3|12|同I420|
| YUVJ420P  |Planar|3|12|同I420,不同的是色彩转换公式，YUVJ使用  JPEG 公式， 输出数据范围[0..255]， I420的范围是 [16-240]|
| YV12      |Planar|3|12|420. Y0Y1..Y15  V0V1V2V3 U0U1U2U3   * 常用 很多编解码器都用这个格式输入输出|
| YVU420P   |Planar|3|12|同YV12|
| NV12      |Planar|2|12|420. Y0Y1..Y15  U0V0U1V1..U3V3 * 常用 |
| NV21      |Planar|2|12|420. Y0Y1..Y15  V0U0V1U1..V3U3|
| YV16      |Planar|3|16|422. Y0Y1..Y15  V0V1..V7 U0U1..U7|
| YUV422P   |Planar|3|16|同YV16|
| I422      |Planar|3|16|同YV16|
| UYVY      |Packed|1|16|422. U0Y0V0Y1/U1Y2V1Y3/...|
| Y422      |Packed|1|16|同UYVY|
| YUY2      |Packed|1|16|422. Y0U0Y1V0/Y2U1Y3V1/...  * 很多摄像机的输出格式|
| YUV422    |Packed|1|16|同YUY2|
| YUYV      |Packed|1|16|同YUY2|
| YVYU      |Packed|1|16|422. Y0V0Y1U0/Y2V1Y3U1/..
| YU16      |Planar|3|16|422. Y0Y1..Y15 U0U1..U7 V0V1..V7|
| NV16      |Planar|2|16|422. Y0Y1..Y15  U0V0U1V1..U7V7|
| YV24      |Planar|3|24|444. Y0Y1..Y15  V0V1..V15 U0U1..U15|
| I444      |Planar|3|24|同YV24|
| IYU2      |Packed|1|24|444. U0Y0V0/U1Y1V1/...|
