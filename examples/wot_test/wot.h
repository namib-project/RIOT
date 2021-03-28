#ifndef WOT_TESTKIT_WOT_H
#define WOT_TESTKIT_WOT_H

#include "wot_internal.h"
#include <stddef.h>

int wot_td_thing_context_add(wot_td_thing_t *thing, json_ld_context_t *context);
int wot_td_thing_context_rm(wot_td_thing_t *thing, json_ld_context_t *context);
json_ld_context_t * wot_td_thing_context_find_nth(wot_td_thing_t *thing, uint8_t pos);
json_ld_context_t * wot_td_thing_context_find_key(wot_td_thing_t *thing, char *key);
json_ld_context_t * wot_td_thing_context_find_value(wot_td_thing_t *thing, char *value);


int wot_td_thing_title_add(wot_td_thing_t *thing, wot_td_multi_lang_t *title);
int wot_td_thing_title_rm(wot_td_thing_t *thing, wot_td_multi_lang_t *title);
wot_td_multi_lang_t * wot_td_thing_titles_find_nth(wot_td_thing_t *thing, uint8_t pos);
wot_td_multi_lang_t * wot_td_thing_titles_find_tag(wot_td_thing_t *thing, char *tag);
wot_td_multi_lang_t * wot_td_thing_titles_find_value(wot_td_thing_t *thing, char *value);


int wot_td_thing_desc_add(wot_td_thing_t *thing, wot_td_multi_lang_t *desc);
int wot_td_thing_desc_rm(wot_td_thing_t *thing, wot_td_multi_lang_t *desc);
wot_td_multi_lang_t * wot_td_thing_desc_find_nth(wot_td_thing_t *thing, uint8_t pos);
wot_td_multi_lang_t * wot_td_thing_desc_find_tag(wot_td_thing_t *thing, char *tag);
wot_td_multi_lang_t * wot_td_thing_desc_find_value(wot_td_thing_t *thing, char *value);

int wot_td_thing_type_add(wot_td_thing_t *thing, wot_td_type_t *type);
int wot_td_thing_type_rm(wot_td_thing_t *thing, wot_td_type_t *type);
wot_td_type_t * wot_td_thing_type_find_nth(wot_td_thing_t *thing, uint8_t pos);
wot_td_type_t * wot_td_thing_type_find_value(wot_td_thing_t *thing, char *value);


int wot_td_thing_prop_add(wot_td_thing_t *thing, wot_td_prop_affordance_t *property);
int wot_td_thing_prop_rm(wot_td_thing_t *thing, wot_td_prop_affordance_t *property);
wot_td_prop_affordance_t * wot_td_thing_prop_find_nth(wot_td_thing_t *thing, uint8_t pos);
wot_td_prop_affordance_t * wot_td_thing_prop_find_key(wot_td_thing_t *thing, char *key);


int wot_td_thing_action_add(wot_td_thing_t *thing, wot_td_action_affordance_t *action);
int wot_td_thing_action_rm(wot_td_thing_t *thing, wot_td_action_affordance_t *action);
wot_td_action_affordance_t * wot_td_thing_action_find_nth(wot_td_thing_t *thing, uint8_t pos);
wot_td_action_affordance_t * wot_td_thing_action_find_key(wot_td_thing_t *thing, char *key);


int wot_td_thing_event_add(wot_td_thing_t *thing, wot_td_event_affordance_t *event);
int wot_td_thing_event_rm(wot_td_thing_t *thing, wot_td_event_affordance_t *event);
wot_td_event_affordance_t * wot_td_thing_event_find_nth(wot_td_thing_t *thing, uint8_t pos);
wot_td_event_affordance_t * wot_td_thing_event_find_key(wot_td_thing_t *thing, char *key);


int wot_td_thing_security_add(wot_td_thing_t *thing, wot_td_security_t *security);
int wot_td_thing_security_rm(wot_td_thing_t *thing, wot_td_security_t *security);
wot_td_security_t * wot_td_thing_security_find_nth(wot_td_thing_t *thing, uint8_t pos);
wot_td_security_t * wot_td_thing_security_find_key(wot_td_thing_t *thing, char *key);


int wot_td_thing_link_add(wot_td_thing_t *thing, wot_td_link_t *link);
int wot_td_thing_link_rm(wot_td_thing_t *thing, wot_td_link_t *link);
wot_td_link_t * wot_td_thing_link_find_nth(wot_td_thing_t *thing, uint8_t pos);


int wot_td_thing_form_add(wot_td_thing_t *thing, wot_td_form_t *form);
int wot_td_thing_form_rm(wot_td_thing_t *thing, wot_td_form_t *form);
wot_td_form_t * wot_td_thing_form_find_nth(wot_td_thing_t *thing, uint8_t pos);


int wot_td_form_security_add(wot_td_form_t *form, wot_td_security_t *security);
int wot_td_form_security_rm(wot_td_form_t *form, wot_td_security_t *security);
wot_td_security_t * wot_td_form_security_find_nth(wot_td_form_t *form, uint8_t pos);
wot_td_security_t * wot_td_form_security_find_key(wot_td_form_t *form, char *key);


int wot_td_form_auth_scope_add(wot_td_form_t *form, wot_td_auth_scopes_t *scope);
int wot_td_form_auth_scope_rm(wot_td_form_t *form, wot_td_auth_scopes_t *scope);
wot_td_auth_scopes_t * wot_td_form_auth_scope_find_nth(wot_td_form_t *form, uint8_t pos);
wot_td_auth_scopes_t * wot_td_form_auth_scope_find_value(wot_td_form_t *form, char *value);

int wot_td_form_op_add(wot_td_form_t *form, wot_td_form_op_t *op);
int wot_td_form_op_rm(wot_td_form_t *form, wot_td_form_op_t *op);
wot_td_form_op_t * wot_td_form_op_find_nth(wot_td_form_t *form, uint8_t pos);


int wot_td_affordance_title_add(wot_td_int_affordance_t *affordance, wot_td_multi_lang_t *title);
int wot_td_affordance_title_rm(wot_td_int_affordance_t *affordance, wot_td_multi_lang_t *title);
wot_td_multi_lang_t * wot_td_affordance_title_find_nth(wot_td_int_affordance_t *affordance, uint8_t pos);
wot_td_multi_lang_t * wot_td_affordance_title_find_tag(wot_td_int_affordance_t *affordance, char *tag);
wot_td_multi_lang_t * wot_td_affordance_title_find_value(wot_td_int_affordance_t *affordance, char *value);


int wot_td_security_desc_add(wot_td_sec_scheme_t *security, wot_td_multi_lang_t *desc);
int wot_td_security_desc_rm(wot_td_sec_scheme_t *security, wot_td_multi_lang_t *desc);
wot_td_multi_lang_t * wot_td_security_desc_find_nth(wot_td_sec_scheme_t *security, uint8_t pos);
wot_td_multi_lang_t * wot_td_security_desc_find_tag(wot_td_sec_scheme_t *security, char *tag);
wot_td_multi_lang_t * wot_td_security_desc_find_value(wot_td_sec_scheme_t *security, char *value);


int wot_td_oauth_auth_scope_add(wot_td_oauth2_sec_scheme_t *scheme, wot_td_auth_scopes_t *scope);
int wot_td_oauth_auth_scope_rm(wot_td_oauth2_sec_scheme_t *scheme, wot_td_auth_scopes_t *scope);
wot_td_auth_scopes_t * wot_td_oauth_scope_find_nth(wot_td_oauth2_sec_scheme_t *scheme, uint8_t pos);
wot_td_auth_scopes_t * wot_td_oauth_scope_find_value(wot_td_oauth2_sec_scheme_t *scheme, char *value);


int wot_td_data_schema_desc_add(wot_td_data_schema_t *schema, wot_td_multi_lang_t *desc);
int wot_td_data_schema_desc_rm(wot_td_data_schema_t *schema, wot_td_multi_lang_t *desc);
wot_td_multi_lang_t * wot_td_data_schema_desc_find_nth(wot_td_data_schema_t *schema, uint8_t pos);
wot_td_multi_lang_t * wot_td_data_schema_desc_find_tag(wot_td_data_schema_t *schema, char *tag);
wot_td_multi_lang_t * wot_td_data_schema_desc_find_value(wot_td_data_schema_t *schema, char *value);


int wot_td_data_schema_title_add(wot_td_data_schema_t *schema, wot_td_multi_lang_t *title);
int wot_td_data_schema_title_rm(wot_td_data_schema_t *schema, wot_td_multi_lang_t *title);
wot_td_multi_lang_t * wot_td_data_data_schema_title_find_nth(wot_td_data_schema_t *schema, uint8_t pos);
wot_td_multi_lang_t * wot_td_data_schema_title_find_tag(wot_td_data_schema_t *schema, char *tag);
wot_td_multi_lang_t * wot_td_data_schema_title_find_value(wot_td_data_schema_t *schema, char *value);


int wot_td_data_schema_validation_add(wot_td_data_schema_t *schema, wot_td_data_schemas_t *validator);
int wot_td_data_schema_validation_rm(wot_td_data_schema_t *schema, wot_td_data_schemas_t *validator);
wot_td_data_schemas_t * wot_td_data_data_schema_validator_find_nth(wot_td_data_schema_t *schema, uint8_t pos);


int wot_td_data_schema_enum_add(wot_td_data_schema_t *schema, wot_td_data_enums_t *enumeration);
int wot_td_data_schema_enum_rm(wot_td_data_schema_t *schema, wot_td_data_enums_t *enumeration);
wot_td_data_enums_t * wot_td_data_schema_enum_find_nth(wot_td_data_schema_t *schema, uint8_t pos);
wot_td_data_enums_t * wot_td_data_schema_enum_find_value(wot_td_data_schema_t *schema, char *value);

#endif //WOT_TESTKIT_WOT_H
