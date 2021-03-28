#ifndef WOT_CONFIG_H
#define WOT_CONFIG_H

#include "net/wot/wot.h"

wot_td_thing_t wot_thing;

int wot_td_config_init(wot_td_thing_t *thing);

wot_td_prop_affordance_t wot_echo_affordance;

#endif //WOT_CONFIG_H
