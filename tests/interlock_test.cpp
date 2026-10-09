#include "../firmware/interlock.h"
#include <cassert>
int main(){
 Interlock x; assert(!x.arm());
 x.tick(0,true,600,true); x.tick(2999,true,600,true);assert(!x.arm());
 x.tick(3000,true,600,true);assert(x.arm());
 x.tick(3100,true,499,true);assert(!x.armed&&!x.ready);
 x.tick(3200,true,600,true);x.tick(6200,true,600,true);assert(x.ready&&!x.armed);assert(x.arm());
 x.tick(6300,true,600,false);assert(!x.armed&&!x.ready);
 x.tick(6400,true,4095,true);assert(!x.valid);
 x.tick(6500,false,600,true);assert(!x.valid);
 x.tick(6600,true,600,true);x.tick(9600,true,600,true);assert(x.arm());x.stop();assert(!x.arm());
 Interlock w;w.tick(0xfffffff0u,true,600,true);w.tick(2984,true,600,true);assert(w.ready);
}
