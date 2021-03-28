#include "net/wot.h"
#include "net/wot/coap.h"
#include "net/wot/coap/config.h"

#ifndef IS_NATIVE
    extern void toggle_led(void);
    extern bool is_led_on(void);
    extern char* get_led_status(void);
#else
    bool led_on;

    bool is_led_on(void) {
        return led_on;
    }

    void toggle_led(void) {
        led_on = !led_on;
    }

    char* get_led_status(void) {
        if (led_on) {
            return "on";
        }
        else {
            return "off";
        }
    }
#endif

static ssize_t _led_status_handler(coap_pkt_t* pdu, uint8_t *buf, size_t len, void *ctx);
static ssize_t _led_toggle_handler(coap_pkt_t* pdu, uint8_t *buf, size_t len, void *ctx);

static ssize_t _echo_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len, void *context)
{
    (void)context;
    char uri[CONFIG_NANOCOAP_URI_MAX];

    if (coap_get_uri_path(pkt, (uint8_t *)uri) <= 0) {
        return coap_reply_simple(pkt, COAP_CODE_INTERNAL_SERVER_ERROR, buf,
                                 len, COAP_FORMAT_JSON, NULL, 0);
    }
    char *sub_uri = uri + strlen("/echo/");
    size_t sub_uri_len = strlen(sub_uri);
    return coap_reply_simple(pkt, COAP_CODE_CONTENT, buf, len, COAP_FORMAT_JSON,
                             (uint8_t *)sub_uri, sub_uri_len);
}

static ssize_t _led_status_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx)
{
    (void)ctx;
    char* led_status = get_led_status();
    gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
    coap_opt_add_format(pdu, COAP_FORMAT_TEXT);
    size_t resp_len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);

    /* write the RIOT board name in the response buffer */
    if (pdu->payload_len >= strlen(led_status)) {
            memcpy(pdu->payload, led_status, strlen(led_status));
            return resp_len + strlen(led_status);
    }
    else {
            puts("gcoap_cli: msg buffer too small");
            return gcoap_response(pdu, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
    }
}

static ssize_t _led_toggle_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx)
{
    (void)ctx;

    /* read coap method type in packet */
    unsigned method_flag = coap_method2flag(coap_get_code_detail(pdu));

    if (method_flag == COAP_POST) {
            toggle_led();
            return gcoap_response(pdu, buf, len, COAP_CODE_CHANGED);
    }

    return 0;
}

const coap_resource_t _coap_resources[] = {
        { "/echo", COAP_GET | COAP_MATCH_SUBTREE, _echo_handler, NULL },
        { "/led/status", COAP_GET, _led_status_handler, NULL },
        { "/led/toggle", COAP_POST, _led_toggle_handler, NULL },
};

static gcoap_listener_t _coap_listener = {
        &_coap_resources[0],
        ARRAY_SIZE(_coap_resources),
        NULL,
        NULL,
        NULL
};

wot_td_form_op_t wot_td_echo_form_op = {
        .op_type = FORM_OP_READ_PROPERTY,
        .next = NULL,
};

wot_td_form_op_t wot_td_led_status_form_op = {
        .op_type = FORM_OP_READ_PROPERTY,
        .next = NULL,
};

wot_td_form_op_t wot_td_led_toggle_form_op = {
        .op_type = FORM_OP_INVOKE_ACTION,
        .next = NULL,
};

wot_td_uri_t wot_td_echo_aff_form_href = {0};
wot_td_extension_t wot_td_echo_form_coap = {0};

wot_td_uri_t wot_td_led_status_aff_form_href = {0};
wot_td_extension_t wot_td_led_status_form_coap = {0};

wot_td_uri_t wot_td_led_toggle_aff_form_href = {0};
wot_td_extension_t wot_td_led_toggle_form_coap = {0};

wot_td_content_type_t wot_td_echo_content_type = {
        .media_type = CONTENT_TYPE_JSON,
        .media_type_paramter = NULL
};

wot_td_content_type_t wot_td_led_status_content_type = {
        .media_type = CONTENT_TYPE_TEXT_PLAIN,
        .media_type_paramter = NULL
};

wot_td_form_t wot_td_echo_aff_form = {
        .op = &wot_td_echo_form_op,
        .content_type = &wot_td_echo_content_type,
        .href = &wot_td_echo_aff_form_href,
        .extensions = &wot_td_echo_form_coap,
        .next = NULL,
};

wot_td_form_t wot_td_led_status_aff_form = {
        .op = &wot_td_led_status_form_op,
        .content_type = &wot_td_led_status_content_type,
        .href = &wot_td_led_status_aff_form_href,
        .extensions = &wot_td_led_status_form_coap,
        .next = NULL,
};

wot_td_form_t wot_td_led_toggle_aff_form = {
        .op = &wot_td_led_toggle_form_op,
        .href = &wot_td_led_toggle_aff_form_href,
        .extensions = &wot_td_led_toggle_form_coap,
        .next = NULL,
};

wot_td_int_affordance_t wot_echo_int_affordance = {
        .forms = &wot_td_echo_aff_form
};

wot_td_int_affordance_t wot_led_status_int_affordance = {
        .forms = &wot_td_led_status_aff_form
};

wot_td_int_affordance_t wot_led_toggle_int_affordance = {
        .forms = &wot_td_led_toggle_aff_form
};

wot_td_data_schema_t wot_hello_prop_data = {
        .read_only = true,
        .write_only = false,
        .json_type = JSON_TYPE_STRING
};

wot_td_data_schema_map_t wot_hello_prop_map = {
        .key = "hello",
        .value = &wot_hello_prop_data
};

wot_td_object_required_t wot_echo_required = {
        .value = "hello"
};

wot_td_object_schema_t wot_echo_data_schema_obj = {
        .properties = &wot_hello_prop_map,
        .required = &wot_echo_required
};

wot_td_data_schema_t wot_echo_data_schema = {
        .json_type = JSON_TYPE_OBJECT,
        .read_only = true,
        .write_only = false,
        .schema = &wot_echo_data_schema_obj
};

wot_td_data_schema_t wot_led_status_data_schema = {
        .json_type = JSON_TYPE_STRING,
        .read_only = true,
        .write_only = false
};

wot_td_prop_affordance_t wot_echo_affordance = {
        .observable = false,
        .key = "echo",
        .int_affordance = &wot_echo_int_affordance,
        .data_schema = &wot_echo_data_schema,
        .next = NULL,
};

wot_td_prop_affordance_t wot_led_status_affordance = {
        .observable = false,
        .key = "status",
        .int_affordance = &wot_led_status_int_affordance,
        .data_schema = &wot_led_status_data_schema,
        .next = NULL,
};

wot_td_action_affordance_t wot_led_toggle_affordance = {
        .key = "toggle",
        .int_affordance = &wot_led_toggle_int_affordance,
        .next = NULL,
};

wot_td_coap_prop_affordance_t wot_coap_echo_affordance = {
        .coap_resource = &_coap_resources[0],
        .affordance = &wot_echo_affordance,
};

wot_td_coap_prop_affordance_t wot_coap_led_status_affordance = {
        .coap_resource = &_coap_resources[1],
        .affordance = &wot_led_status_affordance,
};

wot_td_coap_action_affordance_t wot_coap_led_toggle_affordance = {
        .coap_resource = &_coap_resources[2],
        .affordance = &wot_led_toggle_affordance,
};

int wot_td_coap_config_init(wot_td_thing_t *thing){
    gcoap_register_listener(&_coap_listener);
    wot_td_coap_prop_add(thing, &wot_coap_echo_affordance);
    wot_td_coap_prop_add(thing, &wot_coap_led_status_affordance);
    wot_td_coap_action_add(thing, &wot_coap_led_toggle_affordance);
    return 0;
}
