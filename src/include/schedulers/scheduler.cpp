#ifndef SCHEDULER_CPP
#define SCHEDULER_CPP

#include "schedulers.h"

megaString scheduler(megaString tm2);
node schedule(node account);
metaContainer pruneIrrelevantTasks(metaContainer completeList, calTime cal);

megaString scheduler(megaString tm2){
    for (auto&& child : tm2.child.getChildren()){
        tm2.child.forge(schedule(child));
    }
    return tm2;
}

node schedule(node account){
    calTime cal{};

    for (int i{}; i < 14; i++){
        cal.init(i);
        int loopCount{};
        auto itemList = pruneIrrelevantTasks(metaContainer{account.get("Goals")}, cal).extract();

        while(!itemList.empty()){
            for (auto item: itemList){// go through the list of items to schedule
                // initialize all the variables
                bool scheduled{false};
                int autoStart{std::stoi(item.attributes.get("start"))};// missing error handling
                /*auto scheduleBook = initAttributes();
                //Something along the lines of initAttributes(), but adjusted for containers
                
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
                }*/
            }
            loopCount++;
        }
    }

    return account;
}

metaContainer pruneIrrelevantTasks(metaContainer completeList, calTime cal){
    // simplified version
    for (int i{completeList.children.size()}; i != 0; i--){
        if (completeList.children[i].attributes.get("date") != cal.strDate){
            completeList.children.erase(completeList.children.begin() + i);
        } else {
            pruneIrrelevantTasks(completeList.children[i], cal);
        }
    }

    return completeList;
}

#endif