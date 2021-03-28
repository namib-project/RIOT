/*
 * Copyright (C) 2016 Kaspar Schleiser <kaspar@schleiser.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       CoAP example server application (using nanocoap)
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @}
 */

#include <stdio.h>
#include <string.h>

#include "net/nanocoap_sock.h"
#include "xtimer.h"
#include "shell_commands.h"
#include "shell.h"

#include "net/wot/wot.h"
#include "net/wot/serialization.h"

#define COAP_INBUF_SIZE (256U)

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

/* import "ifconfig" shell command, used for printing addresses */
//extern int _gnrc_netif_config(int argc, char **argv);

static int start_coap_server(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    puts("Start nanocoap server");

    /* initialize nanocoap server instance */
    uint8_t buf[COAP_INBUF_SIZE];
    sock_udp_ep_t local = { .port=COAP_PORT, .family=AF_INET6 };
    nanocoap_server(&local, buf, sizeof(buf));

    return 0;
}

static const shell_command_t shell_commands[] = {
        { "server", "start the nanocoap server", start_coap_server },
        { NULL, NULL, NULL }
};

wot_td_uri_t wot_thing_id = {
        .schema = "coap://",
        .value = "some_thing"
};

wot_td_thing_t wot_thing = {
        .id = &wot_thing_id,
};

char output[1000];

int _thing_full_json(void){

    char first_context_key[] = "some_key1";
    char first_context_value[] = "some_value1";
    json_ld_context_t first_test_context = {
            .key = first_context_key,
            .value = first_context_value,
    };

    char second_context_value[] = "some_value2";
    json_ld_context_t second_test_context = {
            .value = second_context_value,
    };

    wot_td_thing_context_add(&wot_thing, &first_test_context);
    wot_td_thing_context_add(&wot_thing, &second_test_context);

    char first_test_title_tag[] = "testtitletag1";
    char first_test_title_value[] = "testtitlevalue1";
    wot_td_multi_lang_t first_title = {
            .tag = first_test_title_tag,
            .value = first_test_title_value
    };

    char second_test_title_tag[] = "testtitletag2";
    char second_test_title_value[] = "testtitlevalue2";
    wot_td_multi_lang_t second_title = {
            .tag = second_test_title_tag,
            .value = second_test_title_value
    };

    wot_td_thing_title_add(&wot_thing, &first_title);
    wot_td_thing_title_add(&wot_thing, &second_title);

    char saref_type_s[] = "saref:LightSwitch";
    wot_td_type_t saref_type = {
            .value = saref_type_s
    };

    wot_td_thing_type_add(&wot_thing, &saref_type);

    char test_description_tag[] = "DE";
    char test_description_value[] = "dasisteintext";
    wot_td_multi_lang_t test_description = {
            .tag = test_description_tag,
            .value = test_description_value
    };

    wot_td_thing_desc_add(&wot_thing, &test_description);

    wot_td_multi_lang_t scheme_desc = {
            .tag = "DE",
            .value = "Keine Sicherheit"
    };

    wot_td_basic_sec_scheme_t basic_security_scheme = {
            .name = "querykey",
            .in = SECURITY_SCHEME_IN_QUERY
    };

    wot_td_sec_scheme_t scheme = {
            .scheme_type = SECURITY_SCHEME_BASIC,
            .descriptions = &scheme_desc,
            .scheme = &basic_security_scheme
    };

    wot_td_security_t security = {
            .key = "nosec",
            .value = &scheme,
    };

    wot_td_thing_security_add(&wot_thing, &security);

    wot_td_form_op_t property_form_op = {
            .op_type = FORM_OP_READ_PROPERTY
    };

    wot_td_uri_t property_form_href = {
            .schema = "",
            .value = "/some_property_endpoint"
    };

    wot_td_form_t property_aff_form = {
            .op = &property_form_op,
            .href = &property_form_href
    };

    wot_td_int_affordance_t property_int_affordance = {
            .forms = &property_aff_form
    };
    wot_td_prop_affordance_t property_affordance = {
            .observable = true,
            .key = "someproperty",
            .int_affordance = &property_int_affordance
    };

    wot_td_thing_prop_add(&wot_thing, &property_affordance);

    wot_td_form_op_t action_form_op = {
            .op_type = FORM_OP_INVOKE_ACTION
    };

    wot_td_uri_t action_form_href = {
            .schema = "",
            .value = "/some_action_endpoint"
    };

    wot_td_form_t action_aff_form = {
            .op = &action_form_op,
            .href = &action_form_href
    };

    wot_td_int_affordance_t action_int_affordance = {
            .forms = &action_aff_form
    };

    wot_td_multi_lang_t action_aff_input_de_title = {
            .tag = "DE",
            .value = "Test Aktion Input"
    };

    wot_td_multi_lang_t action_aff_input_de_desc = {
            .tag = "DE",
            .value = "Dies ist nur eine Test Aktion. Nicht nutzen."
    };

    wot_td_data_schema_t action_aff_input_data_schema = {
            .titles = &action_aff_input_de_title,
            .descriptions = &action_aff_input_de_desc
    };

    wot_td_multi_lang_t action_aff_output_de_title = {
            .tag = "DE",
            .value = "Test Aktion Output"
    };

    wot_td_multi_lang_t action_aff_output_de_desc = {
            .tag = "DE",
            .value = "Dies ist nur eine Test Aktion. Nicht nutzen."
    };

    wot_td_data_schema_t action_aff_output_data_schema = {
            .titles = &action_aff_output_de_title,
            .descriptions = &action_aff_output_de_desc
    };

    wot_td_action_affordance_t test_action_aff = {
            .key = "testaction",
            .input = &action_aff_input_data_schema,
            .output = &action_aff_output_data_schema,
            .safe = true,
            .idempotent = false,
            .int_affordance = &action_int_affordance
    };

    wot_td_thing_action_add(&wot_thing, &test_action_aff);

    wot_td_form_op_t test_event_form_op = {
            .op_type = FORM_OP_SUBSCRIBE_EVENT
    };

    wot_td_uri_t test_event_form_href = {
            .schema = "coap://",
            .value = "some_event_endpoint"
    };

    wot_td_form_t test_event_form = {
            .op = &test_event_form_op,
            .href = &test_event_form_href
    };

    wot_td_int_affordance_t test_event_int_aff = {
            .forms = &test_event_form
    };

    wot_td_event_affordance_t test_event_aff = {
            .key = "some_event_affordance",
            .int_affordance = &test_event_int_aff
    };

    wot_td_thing_event_add(&wot_thing, &test_event_aff);

    wot_td_serialize_thing(output, 1000, &wot_thing, WOT_TD_SERIALIZE_JSON);

    printf("JSON: %s\n", output);

    return 0;
}

int main(void)
{
    puts("RIOT nanocoap example application");

    _thing_full_json();

    /* nanocoap_server uses gnrc sock which uses gnrc which needs a msg queue */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    //puts("Waiting for address autoconfiguration...");
    //xtimer_sleep(3);

    /* print network addresses */
    //puts("Configured network interfaces:");
    //_gnrc_netif_config(0, NULL);

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    /* define own shell commands */
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should be never reached */
    return 0;
}
