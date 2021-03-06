/*
 * =====================================================================================
 *
 *       Filename:  xOSCApp.cpp
 *
 *    Description:  osc send / receive test
 *
 *        Version:  1.0
 *        Created:  03/18/2014 13:20:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "horo_OSCApp.h"

struct MyApp : public OSCApp {
  virtual void onDraw(){
    SharedData::SendToMain( SharedData::TestPacket() );
  }
};

MyApp app;

int main(){
  SharedData::print();
  app.start();
}
