#include "net/wot.h"
#include "net/wot/coap.h"
#include "net/wot/coap/config.h"

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

const coap_resource_t _coap_resources[] = {
        { "/echo", COAP_GET | COAP_MATCH_SUBTREE, _echo_handler, NULL },
};

static gcoap_listener_t _coap_listener = {
        &_coap_resources[0],
        sizeof(_coap_resources) / sizeof(_coap_resources[0]),
        NULL,
        NULL
};

wot_td_form_op_t wot_td_echo_form_op = {
        .op_type = FORM_OP_READ_PROPERTY,
        .next = NULL,
};

wot_td_uri_t wot_td_echo_aff_form_href = {0};
wot_td_extension_t wot_td_echo_form_coap = {0};

wot_td_content_type_t wot_td_echo_content_type = {
        .media_type = CONTENT_TYPE_JSON,
        .media_type_paramter = NULL
};

wot_td_form_t wot_td_echo_aff_form = {
        .op = &wot_td_echo_form_op,
        .content_type = &wot_td_echo_content_type,
        .href = &wot_td_echo_aff_form_href,
        .extensions = &wot_td_echo_form_coap,
        .next = NULL,
};

wot_td_int_affordance_t wot_echo_int_affordance = {
        .forms = &wot_td_echo_aff_form
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

wot_td_prop_affordance_t wot_echo_affordance = {
        .observable = false,
        .key = "echo",
        .int_affordance = &wot_echo_int_affordance,
        .data_schema = &wot_echo_data_schema,
        .next = NULL,
};

wot_td_coap_prop_affordance_t wot_coap_echo_affordance = {
        .coap_resource = &_coap_listener,
        .affordance = &wot_echo_affordance,
};

int wot_td_coap_config_init(wot_td_thing_t *thing){
    wot_td_coap_prop_add(thing, &wot_coap_echo_affordance);
    return 0;
}
