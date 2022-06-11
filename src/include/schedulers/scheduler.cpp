#ifndef SCHEDULER_CPP
#define SCHEDULER_CPP

#include "schedulers.h"

megaString scheduler(megaString tm2);
node schedule(node account);

megaString scheduler(megaString tm2){
    for (auto&& child : tm2.child.getChildren()){
        tm2.child.forge(schedule(child));
    }
    return tm2;
}

node schedule(node account){
    return account;
}

#endif