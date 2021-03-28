#include "net/wot.h"
#include "net/wot/coap.h"
#include "net/wot/coap/config.h"

#ifndef IS_NATIVE
extern void toggle_led(void);
extern bool is_led_on(void);
extern char *get_led_status(void);
#else
bool led_on;

bool is_led_on(void)
{
    return led_on;
}

void toggle_led(void)
{
    led_on = !led_on;
}

char *get_led_status(void)
{
    if (led_on)
    {
        return "on";
    }
    else
    {
        return "off";
    }
}
#endif

ssize_t _echo_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len, void *context)
{
    (void)context;
    char uri[CONFIG_NANOCOAP_URI_MAX];

    if (coap_get_uri_path(pkt, (uint8_t *)uri) <= 0)
    {
        return coap_reply_simple(pkt, COAP_CODE_INTERNAL_SERVER_ERROR, buf,
                                 len, COAP_FORMAT_JSON, NULL, 0);
    }
    char *sub_uri = uri + strlen("/echo/");
    size_t sub_uri_len = strlen(sub_uri);
    return coap_reply_simple(pkt, COAP_CODE_CONTENT, buf, len, COAP_FORMAT_JSON,
                             (uint8_t *)sub_uri, sub_uri_len);
}

ssize_t _led_status_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx)
{
    (void)ctx;
    char *led_status = get_led_status();
    gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
    coap_opt_add_format(pdu, COAP_FORMAT_TEXT);
    size_t resp_len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);

    if (pdu->payload_len >= strlen(led_status))
    {
        memcpy(pdu->payload, led_status, strlen(led_status));
        return resp_len + strlen(led_status);
    }
    else
    {
        puts("wot_coap: msg buffer too small");
        return gcoap_response(pdu, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
    }
}

ssize_t _led_toggle_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx)
{
    (void)ctx;

    /* read coap method type in packet */
    unsigned method_flag = coap_method2flag(coap_get_code_detail(pdu));

    if (method_flag == COAP_POST)
    {
        toggle_led();
        return gcoap_response(pdu, buf, len, COAP_CODE_CHANGED);
    }

    return 0;
}