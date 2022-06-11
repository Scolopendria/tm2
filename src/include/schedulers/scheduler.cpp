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
    bool scheduled{};
    int autoStart{}, loopCount{};
    calTime cal{};

    for (int i{}; i < 14; i++){
        cal.init(i);
        loopCount = 0;
        
        //auto item = containerize(account.get("Goals"));
        /*
        while(!itemList.empty()){
            for (auto item: itemList[0]){ // go through the list of items to schedule
                // initialize all the variables
                //scheduleBook = initAttributes(sPath->sortAttributes()->getAttributesList());
                //print(scheduleBook);
                scheduled = false;
                autoStart = std::stoi(item.attributes.get("start"));
                for (auto t : scheduleBook){
                    // condition checks.. behaviour and dependancies
                    // check if scheduling before the next task's start time
                    // if true, that means autoStart is in open time
                    if (autoStart > caltime.minute_t && autoStart < t.getStart()){
                        scheduled = collide(
                            gRoot,
                            sPath,
                            caltime.minute_t + 1,
                            item,
                            scheduleBook,
                            autoStart
                        );
                        if (scheduled) break;
                    }
                    autoStart = std::max(t.getEnd(), autoStart);
                }
                if (!scheduled){
                    scheduled = collide( //scheduled unused here
                        gRoot,
                        sPath,
                        caltime.minute_t + 1,
                        item,
                        scheduleBook,
                        autoStart
                    );
                }
            }
            // reinitialize itemList -list of tasks left to be scheduled
            //loopcount and sPath needs to be oncorporated into a giant object to be passed around
            //itemList = smartPass(purge(demote(itemList[0], loopCount++), sPath), itemList[1]);
        }
        decontainerize(itemList[1]);
        */
    }

    return account;
}

#endif