

class EPD {
  public:
    EPD();
    ~EPD();
    void begin();
    void setSize(int size);
    void reset();
    void CMD(unsigned char command);
    void DATA(unsigned char command);
    void chkstatus();
    void init();
    void load(unsigned char value);
    void next();
    void refresh();
    void sleep();
    void clear();
    

  private:
    int size;
    int width;
    int height;
    int bits;
    void _clear(unsigned char c);
};
