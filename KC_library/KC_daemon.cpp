#include "KC_daemon.h"

#define D_FUNCTION(type) template <typename FunctionalityTn> type Daemon<FunctionalityTn>::

D_FUNCTION(int) Start() {
}

#undef D_FUNCTION
