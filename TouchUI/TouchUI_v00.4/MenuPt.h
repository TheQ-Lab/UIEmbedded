//MenuPt.h
#ifndef MenuPt_h
#define MenuPt_h

class MenuPt
{
  public:
    int x,y,w,h;
    virtual void draw();
    virtual void onClick();
    virtual void onClick(byte);
};

#endif
