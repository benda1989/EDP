#include <SPI.h>
#include "epd.h"
#include "conf.h"

EPD::EPD()
{
}

EPD::~EPD()
{
}

void EPD::begin()
{
  pinMode(BUSY_Pin, INPUT);
  pinMode(RES_Pin, OUTPUT);
  pinMode(DC_Pin, OUTPUT);
  pinMode(CS_Pin, OUTPUT);
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  SPI.begin();
}

void EPD::setSize(int size)
{
  this->bits = 1;
  switch (size)
  {
    case 42:
      this->width = 400;
      this->height = 300;
      break;
    case 75:
      this->width = 640;
      this->height = 384;
      this->bits = 4;
      break;
    case 102:
      this->width = 960;
      this->height = 640;
      break;
  }
  this->size = size;
  this->bits = this->bits * this->width * this->height / 8;
  Serial.println(this->size);
}

void EPD::chkstatus(void)
{
  int s = this->size ==102;
  while (digitalRead(BUSY_Pin) == s)
  {
    delay(50);
    ESP.wdtFeed();
  }  //102: 0:BUSY  //1: busy
}

void EPD::CMD(unsigned char command)
{
  EPD_DC_0;
  EPD_CS_0;
  SPI.transfer(command);
  EPD_CS_1;
}

void EPD::DATA(unsigned char command)
{

  EPD_DC_1;
  EPD_CS_0;
  SPI.transfer(command);
  EPD_CS_1;
  //  Serial.print(command);
}

void EPD::reset()
{
  EPD_RST_0; // Module reset
  delay(10); // At least 10ms
  EPD_RST_1;
  delay(10);
}

void EPD::init()
{
  this->reset(); // Electronic paper IC reset
  switch (this->size)
  {
    case 29:
      this->CMD(0x01);  //POWER_SETTING
      this->DATA(0x07);
      this->DATA(0x00);
      this->DATA(0x08);
      this->DATA(0x00);
      this->CMD(0x06);   // boost soft start
      this->DATA(0x17);  // A
      this->DATA(0x17);  // B
      this->DATA(0x17);  // C
      this->CMD(0x04);   // Power on
      this->chkstatus();

      this->CMD(0x00);  //PANEL_SETTING
      this->DATA(0x8F);
      this->CMD(0x50);  //VCOM_AND_DATA_INTERVAL_SETTING;
      this->DATA(0x77);
      this->CMD(0x30);  //PLL_CONTROL
      this->DATA(0x39);
      this->CMD(0x61);  //TCON_RESOLUTION
      this->DATA(0x80);
      this->DATA(0x01);
      this->DATA(0x28);
      this->CMD(0x82);  //VCM_DC_SETTING_REGISTER
      this->DATA(0x0A);
      
      this->CMD(0x10);
      break;
    case 42:
      this->CMD(0x06);   // boost soft start
      this->DATA(0x17);  // A
      this->DATA(0x17);  // B
      this->DATA(0x17);  // C
      this->CMD(0x04);   // Power on
      this->chkstatus(); // waiting for the electronic paper IC to release the idle signal
      this->CMD(0x00);   // panel setting
      this->DATA(0x0f);  // LUT from OTP£¬400x300
      this->DATA(0x0d);  // VCOM to 0V fast
      this->CMD(0x50);
      this->DATA(0x77);

      this->CMD(0x10);
      break;
    case 75:
      this->CMD(0x01);
      this->DATA(0x37);
      this->DATA(0x00);

      this->CMD(0x00);
      this->DATA(0xCF);
      this->DATA(0x08);

      this->CMD(0x06);
      this->DATA(0xC7);
      this->DATA(0xCC);
      this->DATA(0x28);

      this->CMD(0x04);
      this->chkstatus();

      this->CMD(0x30);
      this->DATA(0x3A);
      this->CMD(0x41);
      this->DATA(0x00);
      this->CMD(0x60);
      this->DATA(0x22);
      this->CMD(0x50);
      this->DATA(0x77);

      this->CMD(0x61);
      this->DATA(0x02);
      this->DATA(0x80);
      this->DATA(0x01);
      this->DATA(0x80);

      this->CMD(0x82);
      this->DATA(0x1E);

      this->CMD(0xe5);
      this->DATA(0x03);

      this->CMD(0x10);
      break;
    case 102:
      this->CMD(0x12);   // SWRESET
      this->chkstatus(); // waiting for the electronic paper IC to release the idle signal

      this->CMD(0x0C); // Soft start setting
      this->DATA(0xAE);
      this->DATA(0xC7);
      this->DATA(0xC3);
      this->DATA(0xC0);
      this->DATA(0x80);

      this->CMD(0x01); // Set MUX as 639
      this->DATA(0x7F);
      this->DATA(0x02);
      this->DATA(0x00);

      this->CMD(0x11); // Data entry mode
      this->DATA(0x01);

      this->CMD(0x18);
      this->DATA(0X80);

      this->CMD(0x3C);  // border
      this->DATA(0x01); // LUT1, for white

      this->CMD(0x44);
      this->DATA(0x00); // RAM x address start at 0
      this->DATA(0x00);
      this->DATA(0xBF); // RAM x address end at 3BFh -> 959
      this->DATA(0x03);
      this->CMD(0x45);
      this->DATA(0x7F); // RAM y address start at 27Fh;
      this->DATA(0x02);
      this->DATA(0x00); // RAM y address end at 00h;
      this->DATA(0x00);

      this->CMD(0x4E);
      this->DATA(0x00);
      this->DATA(0x00);
      this->CMD(0x4F);
      this->DATA(0x7F);
      this->DATA(0x02);

      this->CMD(0x24);
      break;
  }
}

void EPD::load(unsigned char value)
{
  if (this->size == 75)
  {
    for (int j = 0; j < 2; j++)
    {
      int temp = 0;
      if ((value & 0x03) == 0x03)
        temp = 0x40;
      else if ((value & 0x03) == 0x01)
        temp = 0x30;
      value = value >> 2;

      if ((value & 0x03) == 0x03)
        temp |= 0x04;
      else if ((value & 0x03) == 0x01)
        temp |= 0x03;
      value = value >> 2;
      this->DATA((byte)temp);
    }
  }
  else
  {
    this->DATA((byte)value);
  }
}

void EPD::next(void)
{
  if (this->size == 102)
  {
    this->CMD(0x26);
  }
  else
  {
    this->CMD(0x13);
  }
}

void EPD::refresh(void)
{
  if (this->size == 102)
  {
    this->CMD(0x22); // Display Update Control
    this->DATA(0xF7);
    this->CMD(0x20); // Activate Display Update Sequence
  }
  else
  {
    this->CMD(0x12); // DISPLAY REFRESH
    delay(100);      //!!!The delay here is necessary, 200uS at least!!!
  }
  this->chkstatus();
}

void EPD::sleep(void)
{
  if (this->size == 102)
  {
    this->CMD(0x10);
    this->DATA(0x01);
    delay(100);
  }
  else
  {
    this->CMD(0X02);   // power off
    this->chkstatus(); // waiting for the electronic paper IC to release the idle signal
    this->CMD(0X07);   // deep sleep
    this->DATA(0xA5);
  }
}

void EPD::clear()
{
  this->init();
  if (this->size == 102)
  {
    this->_clear(0xff);
    this->CMD(0x26); // write RAM for black(0)/white (1)
    this->_clear(0x00);
  }
  else
  {
    if (this->size == 75)
    {
      this->_clear(0x33);
    }
    else
    {
      this->_clear(0xff);
      this->CMD(0x13);
      this->_clear(0xff);
    }
  }
  this->refresh();
  this->sleep();
}

void EPD::_clear(unsigned char c)
{
  unsigned int i;
  for (i = 0; i < this->bits; i++)
  {
    this->DATA(c);
    if (i % 20000 == 0)
      ESP.wdtFeed();
  }
}
