#include <stddef.h>
#include "net/wot/config.h"

json_ld_context_t wot_td_saref_context = {
        .key = "saref",
        .value = "https://w3id.org/saref#",
        .next = NULL,
};

wot_td_type_t wot_td_saref_type = {
        .value = "saref:LightSwitch",
        .next = NULL,
};

wot_td_multi_lang_t wot_td_title_de = {
        .tag = "de",
        .value = "Deutscher Titel",
        .next = NULL,
};

wot_td_multi_lang_t wot_td_title_en = {
        .tag = "en",
        .value = "English title",
        .next = NULL,
};

wot_td_multi_lang_t wot_td_description_de = {
        .tag = "de",
        .value = "Deutsche Beschreibung",
        .next = NULL,
};

wot_td_multi_lang_t wot_td_description_en = {
        .tag = "en",
        .value = "English description",
        .next = NULL,
};

char wot_td_default_lang_tag[] = "en";

wot_td_multi_lang_t wot_td_sec_scheme_desc_en = {
        .tag = "en",
        .value = "Basic sec schema",
        .next = NULL,
};

/*wot_td_basic_sec_scheme_t wot_td_basic_security_scheme = {
        .name = "querykey",
        .in = SECURITY_SCHEME_IN_QUERY
};
*/

wot_td_sec_scheme_t wot_td_sec_scheme = {
        .scheme_type = SECURITY_SCHEME_NONE,
        .descriptions = &wot_td_sec_scheme_desc_en,
        .scheme = NULL
};

wot_td_security_t wot_td_security_schema = {
        .key = "noschema",
        .value = &wot_td_sec_scheme,
        .next = NULL,
};

int wot_td_config_init(wot_td_thing_t *thing){
    wot_td_thing_context_add(thing, &wot_td_saref_context);

    wot_td_thing_type_add(thing, &wot_td_saref_type);
    wot_td_thing_title_add(thing, &wot_td_title_en);
    wot_td_thing_title_add(thing, &wot_td_title_de);
    wot_td_thing_desc_add(thing, &wot_td_description_en);
    wot_td_thing_desc_add(thing, &wot_td_description_de);

    thing->default_language_tag = wot_td_default_lang_tag;

    wot_td_thing_security_add(thing, &wot_td_security_schema);

    return 0;
}
