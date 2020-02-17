class ControlRelay {
  public:
    ControlRelay(int _pin, bool activeLow=true);

    void init();

    int getPin();
    void setPin(int pin);

    bool powerEnabled();
    bool powerDisabled();

    void enablePower();
    void disablePower();

    void setPowerState(bool enablePower);

  private:
    bool _activeLow;
    bool _powerEnabled;
    int _pin;
};