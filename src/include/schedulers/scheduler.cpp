#ifndef SCHEDULER_CPP
#define SCHEDULER_CPP

#include "schedulers.h"
#include <vector>
#include <iostream>

megaString scheduler(megaString tm2);
node schedule(node account);
metaContainer wheel(metaContainer day);
metaContainer collide(metaContainer day);
metaContainer pruneIrrelevantTasks(metaContainer completeList, calTime cal);
attributeContainer decontainerize(metaContainer day);

megaString scheduler(megaString tm2){
    std::vector<node> children{tm2.child.getChildren()};
    for (auto &&child : children){
        tm2.child.forge(schedule(child));
    }

    return tm2;
}

node schedule(node account){
    for (int i{}; i < 3; i++){// assignment failing
        calTime tempor{i};
        account.forge(
            *account.get("Schedule").forge(
                [tempor](node account){
                    node temp{"\"" + tempor.strDate + "\"{}"};
                    temp.attributes = decontainerize(
                        wheel(
                            pruneIrrelevantTasks(
                                // switch to decide which task gets scheduled on which day before sending it to wheel
                                // basically prube irrevlevant task before wheel for better long term management
                                // introduce 'demotions' in this meta pruner and cross-day info passing
                                metaContainer{account.get("Goals")},
                                tempor
                            ).extract()
                        )
                    );

                    std::cout << "pass: " << tempor.strDate << std::endl;

                    return temp;
                }(account)
            )
        );
    }

    return account;
}

metaContainer wheel(metaContainer day){
    // param niche of this function, wheel, and unnderling function, collide, is the same
    // they do have different functional roles, though
    // schedule children
    while (!day.children.empty()){
        day = collide(day);
        //day.children = demote(day.children);
    }
    // recursively schedule (spin through) scheduled-children
    std::vector<metaContainer> scheduledChildrenTree{};
    for (auto &&child : day.scheduledChildren){
        scheduledChildrenTree.push_back(wheel(child));
    }
    day.scheduledChildren = scheduledChildrenTree;

    return day;
}

metaContainer collide(metaContainer day){// collision error
    for (std::size_t i{day.children.size()-1}; i != SIZE_MAX; i--){
        int totalTimeUsed{day.children[i].updateTotalTime()};

        if (totalTimeUsed > day.getTask().getEnd() - day.getTask().getStart()) continue;
        
        if (day.scheduledChildren.empty()){
            day.init(i, day.getTask().getStart());
            continue;    
        }

        /////////////////////

        int time1{totalTimeUsed + day.scheduledChildren.back().getTask().getEnd()};
        int timeEnd{day.getTask().getEnd()};

        /////////////////////

        if (totalTimeUsed + day.getTask().getStart() < day.scheduledChildren.front().getTask().getStart()){
            day.init(i, day.getTask().getStart());
        } else if (totalTimeUsed + day.scheduledChildren.back().getTask().getEnd() <= day.getTask().getEnd()){
            day.init(i, day.scheduledChildren.back().getTask().getEnd());
        } else if  (day.scheduledChildren.size() != 1){
            for (std::size_t iter{}; iter < day.scheduledChildren.size()-2; iter++){
                if (totalTimeUsed + day.scheduledChildren[iter].getTask().getEnd() < day.scheduledChildren[iter+1].getTask().getStart()){
                    day.init(i, day.scheduledChildren[i].getTask().getEnd());
                    break;
                }
            }
        }
    }

    // missing optimizations
    return day;
}

metaContainer pruneIrrelevantTasks(metaContainer completeList, calTime cal){
    // simplified version
    for (std::size_t i{completeList.children.size()-1}; i != SIZE_MAX; i--){
        if (completeList.children[i].attributes.get("date") != cal.strDate){
            completeList.children.erase(completeList.children.begin() + i);
        } else {
            pruneIrrelevantTasks(completeList.children[i], cal);
        }
    }

    return completeList;
}

attributeContainer decontainerize(metaContainer day){
    attributeContainer scheduleBook{};

    if (day.getName() == "self") scheduleBook.set(day.getTask().getFullStdTime(), day.getTask().getName());// redundant

    for (auto &&child : day.scheduledChildren){
        scheduleBook.set(decontainerize(child).getList());
    }

    return scheduleBook;
}

#endif