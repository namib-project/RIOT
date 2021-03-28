#ifndef NET_WOT_COAP_H
#define NET_WOT_COAP_H

#include "net/wot.h"
#include "net/gcoap.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct wot_td_coap_form {
    const coap_resource_t *coap_resource;
    wot_td_form_t *form;
    struct wot_td_coap_form *next;
} wot_td_coap_form_t;

void wot_td_coap_add_forms(wot_td_coap_form_t *coap_forms);
void wot_td_coap_server_init(void);

#ifdef __cplusplus
}
#endif

#endif /* NET_WOT_COAP_H */
