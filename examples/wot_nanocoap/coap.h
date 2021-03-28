#ifndef WOT_COAP_H
#define WOT_COAP_H

#include "net/wot/wot.h"
#include "net/nanocoap.h"

typedef struct {
    coap_resource_t *coap_resource;
    wot_td_prop_affordance_t *affordance;
} wot_td_coap_prop_affordance_t;

typedef struct {
    coap_resource_t *coap_resource;
    wot_td_action_affordance_t *affordance;
} wot_td_coap_action_affordance_t;

typedef struct {
    coap_resource_t *coap_resource;
    wot_td_event_affordance_t *affordance;
} wot_td_coap_event_affordance_t;

int wot_td_coap_prop_add(wot_td_thing_t *thing, wot_td_coap_prop_affordance_t *property);
int wot_td_coap_action_add(wot_td_thing_t *thing, wot_td_action_affordance_t *action);
int wot_td_coap_event_add(wot_td_thing_t *thing, wot_td_coap_event_affordance_t *event);

#endif //WOT_COAP_H
