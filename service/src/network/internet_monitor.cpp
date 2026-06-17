#include "internet_monitor.hpp"

#include <sdbus-c++/sdbus-c++.h>

bool InternetMonitor::hasInternetConnection() const {
    auto proxy = sdbus::createProxy(sdbus::ServiceName{"org.airalarm.InternetMonitor"},
                                    sdbus::ObjectPath{"/org/airalarm/InternetMonitor"});

    bool result{};

    proxy->callMethod("HasInternetConnection")
        .onInterface("org.airalarm.InternetMonitor")
        .storeResultsTo(result);

    return result;
}