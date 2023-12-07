#ifndef D_EPD

#define D_EPD     42 //42//75//102//29
#define D_EPDI    1  //1//2//3//0
#define RES_Pin       2
#define DC_Pin        4
#define CS_Pin        0
#define BUSY_Pin      5

#define EPD_CS_0 digitalWrite(CS_Pin,LOW)
#define EPD_CS_1 digitalWrite(CS_Pin,HIGH)
#define EPD_DC_0  digitalWrite(DC_Pin,LOW)
#define EPD_DC_1  digitalWrite(DC_Pin,HIGH)
#define EPD_RST_0 digitalWrite(RES_Pin,LOW)
#define EPD_RST_1 digitalWrite(RES_Pin,HIGH)

#endif
