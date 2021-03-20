class Hitbox
{
  public:
    int x, y, w, h;
    String desc;
    //Option reference;
    Hitbox (int _x, int _y, int _w, int _h, String _desc) {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      desc = _desc;
    }
    /*
    void addOption(Option& o) {
      reference = o;
    }*/

    void trigger() {
      Serial.println(desc);
    }
};
