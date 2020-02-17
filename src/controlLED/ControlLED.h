class ControlLED {
  public:
    ControlLED(int rPin, int gPin, int bPin);

    void init();

    void setRGB(int r, int g, int b);
    // void fadeToRGB(int r, int g, int b, int msDuration);

  private:
    int _rPin;
    int _gPin;
    int _bPin;

    int _rValue;
    int _gValue;
    int _blueValue;
};