#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "xtimer.h"
#include "net/wot.h"
#include "net/wot/serialization.h"

#define COUNT_x(var, type) ({           \
    uint8_t counter = 0;                \
    type *tmp = var;                    \
    if(var != NULL){                    \
        counter++;                      \
        while(tmp->next != NULL){       \
            tmp = tmp->next;            \
            counter++;                  \
        }                               \
    }                                   \
    return counter;                     \
})

uint8_t _count_titles(wot_td_multi_lang_t *titles){
    COUNT_x(titles, wot_td_multi_lang_t);
}

uint8_t _count_descriptions(wot_td_multi_lang_t *descriptions){
    COUNT_x(descriptions, wot_td_multi_lang_t);
}

void _test_util_features(char *text){
    printf("\n%s\n", text);
}

void _test_util_scenario(char *text){
    printf("    %s\n", text);
}

void _test_util_given(char *text){
    printf("        %s\n", text);
}

void _test_util_when(char *text){
    printf("            %s\n", text);
}

void _test_util_then(char *text, bool result){
    if(result){
        printf("                \e[42;97m%s: true\e[0m\n", text);
    }else{
        printf("                \e[41;97m%s: false\e[0m\n", text);
    }
}

wot_td_uri_t wot_thing_id = {
        .schema = "coap://",
        .value = "some_thing"
};

wot_td_thing_t wot_thing = {
        .id = &wot_thing_id,
};

void _string_receiver(const char * string){
    printf("%c", string[i]);
}

int _thing_full_json(void){
    wot_td_content_type_t json_content_type = CONTENT_TYPE_JSON;

    _test_util_given("Given: A thing description without descriptions");
    _test_util_when("When adding one context items to it");

    const char first_context_key[] = "some_key1";
    const char first_context_value[] = "some_value1";
    json_ld_context_t first_test_context = {
            .key = first_context_key,
            .value = first_context_value,
    };

    _test_util_when("When adding second context items to it");
    const char second_context_value[] = "some_value2";
    json_ld_context_t second_test_context = {
            .value = second_context_value,
    };

    _test_util_then("Then there must be two context item",
                    wot_td_thing_context_add(&wot_thing, &first_test_context) == 0 &&
                    wot_td_thing_context_add(&wot_thing, &second_test_context) == 0);

    _test_util_when("When adding two title items to it");

    const char first_test_title_tag[] = "de";
    const char first_test_title_value[] = "testtitlevalue1";
    wot_td_multi_lang_t first_title = {
            .tag = first_test_title_tag,
            .value = first_test_title_value
    };

    const char second_test_title_tag[] = "en";
    const char second_test_title_value[] = "this is an english title";
    wot_td_multi_lang_t second_title = {
            .tag = second_test_title_tag,
            .value = second_test_title_value
    };

    _test_util_then("Then there must be two items in the correct order",
                    wot_td_thing_title_add(&wot_thing, &first_title) == 0 &&
                    wot_td_thing_title_add(&wot_thing, &second_title) == 0 &&
                    _count_titles(wot_thing.titles) == 2 &&
                    wot_td_thing_titles_find_nth(&wot_thing, 1)->value == second_test_title_value
    );

    _test_util_when("When adding one type to it");

    const char saref_type_s[] = "saref:LightSwitch";
    wot_td_type_t saref_type = {
            .value = saref_type_s
    };

    _test_util_then("Then there must be one type",
                    wot_td_thing_type_add(&wot_thing, &saref_type) == 0
    );

    _test_util_when("When adding descriptions");
    const char test_description_tag_de[] = "de";
    const char test_description_value_de[] = "dasisteintext";
    wot_td_multi_lang_t test_description_de = {
            .tag = test_description_tag_de,
            .value = test_description_value_de
    };

    const char test_description_tag_en[] = "en";
    const char test_description_value_en[] = "this is an english description";
    wot_td_multi_lang_t test_description_en = {
            .tag = test_description_tag_en,
            .value = test_description_value_en
    };

    _test_util_then("Then the thing must contain the correct descriptions",
                    wot_td_thing_desc_add(&wot_thing, &test_description_de) == 0 &&
                    wot_td_thing_desc_add(&wot_thing, &test_description_en) == 0 &&
                    _count_descriptions(wot_thing.descriptions) == 2 &&
                    strcmp(wot_thing.descriptions->tag, test_description_tag_de) == 0 &&
                    strcmp(wot_thing.descriptions->value, test_description_value_de) == 0);

    _test_util_when("When adding the default lang tag");

    char lang_tag[] = "en";
    wot_thing.default_language_tag = lang_tag;
    _test_util_then("Then the thing must contain the default tag",
                    strcmp(wot_thing.default_language_tag, lang_tag));


    _test_util_when("When adding a nosec security");

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

    _test_util_then("Then there must be one security",
                    wot_td_thing_security_add(&wot_thing, &security) == 0
    );

    _test_util_when("When adding a property affordance");
    wot_td_form_op_t property_form_op = {
            .op_type = FORM_OP_READ_PROPERTY
    };

    wot_td_uri_t property_form_href = {
            .schema = "",
            .value = "/some_property_endpoint"
    };

    wot_td_form_t property_aff_form = {
            .op = &property_form_op,
            .href = &property_form_href,
            .content_type = &json_content_type,
    };

    wot_td_int_affordance_t property_int_affordance = {
            .forms = &property_aff_form
    };
    wot_td_prop_affordance_t property_affordance = {
            .observable = true,
            .key = "someproperty",
            .int_affordance = &property_int_affordance
    };

    _test_util_then("Then there must be one property affordance",
                    wot_td_thing_prop_add(&wot_thing, &property_affordance) == 0
    );

    _test_util_when("When adding an action affordance");

    wot_td_form_op_t action_form_op = {
            .op_type = FORM_OP_INVOKE_ACTION
    };

    wot_td_uri_t action_form_href = {
            .schema = "",
            .value = "/some_action_endpoint"
    };

    wot_td_form_t action_aff_form = {
            .op = &action_form_op,
            .href = &action_form_href,
            .content_type = &json_content_type,
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

    _test_util_then("Then there must be one action affordance",
                    wot_td_thing_action_add(&wot_thing, &test_action_aff) == 0
    );

    _test_util_when("When adding an event affordance");

    wot_td_form_op_t test_event_form_op = {
            .op_type = FORM_OP_SUBSCRIBE_EVENT
    };

    wot_td_uri_t test_event_form_href = {
            .schema = "coap://",
            .value = "some_event_endpoint"
    };

    wot_td_form_t test_event_form = {
            .op = &test_event_form_op,
            .href = &test_event_form_href,
            .content_type = &json_content_type,

    };

    wot_td_int_affordance_t test_event_int_aff = {
            .forms = &test_event_form
    };

    wot_td_event_affordance_t test_event_aff = {
            .key = "some_event_affordance",
            .int_affordance = &test_event_int_aff
    };

    _test_util_then("Then there must be one event affordance",
                    wot_td_thing_event_add(&wot_thing, &test_event_aff) == 0
    );

    puts("JSON: ");
    wot_td_serialize_thing((wot_td_serialize_receiver_t) &_string_receiver, &wot_thing);
    printf("%c\n", '\0');

    return 0;
}

int main(void) {
    xtimer_sleep(3);
    puts("------ Starting tests -----\n");

    _test_util_features("Feature: WoT TD serialization");
    _thing_full_json();

    return 0;
}
