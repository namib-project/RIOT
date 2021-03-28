#include "wot.h"
#include <stddef.h>
#include <string.h>
#include "error.h"

#define WOT_TD_ADD_CONTENT(entity, prop, var, type) ({          \
    if(entity == NULL || var == NULL){                          \
        return -EINVAL;                                         \
    }                                                           \
                                                                \
    type *tmp = entity->prop;                                   \
                                                                \
    var->next = NULL;                                           \
                                                                \
    if(tmp == NULL){                                            \
        entity->prop = var;                                     \
    }else{                                                      \
        while(tmp->next != NULL && (tmp->next != var)){         \
            tmp = tmp->next;                                    \
        }                                                       \
        tmp->next = var;                                        \
    }                                                           \
    return 0;                                                   \
})

#define WOT_TD_RM_CONTENT(entity, prop, var, type) ({           \
    if(entity == NULL || var == NULL){                          \
        return -EINVAL;                                         \
    }                                                           \
                                                                \
    type *tmp = entity->prop;                                   \
                                                                \
    if(tmp == NULL){                                            \
        return -EINVAL;                                         \
    }                                                           \
                                                                \
    if(tmp == var){                                             \
        entity->prop = var->next;                               \
        return 0;                                               \
    }                                                           \
    while(tmp->next != NULL && (tmp->next != var)){             \
        tmp = tmp->next;                                        \
    }                                                           \
    if(tmp->next == var){                                       \
        tmp->next = var->next;                                  \
    }else{                                                      \
        return -EFAULT;                                         \
    }                                                           \
                                                                \
    return 1;                                                   \
})

#define WOT_TD_NTH_CONTENT(entity, prop, type) ({               \
    type *tmp = entity->prop;                                   \
                                                                \
    for (int i = 0; (i < pos) && tmp; i++) {                    \
        tmp = tmp->next;                                        \
    }                                                           \
    return tmp;                                                 \
})

#define WOT_TD_FIND_x(entity, prop, type, x) ({                 \
    type *tmp = entity->prop;                                   \
                                                                \
    while(tmp != NULL && strcmp(tmp->x, x) != 0){               \
        tmp = tmp->next;                                        \
    }                                                           \
                                                                \
    return tmp;                                                 \
})

int wot_td_thing_context_add(wot_td_thing_t *thing, json_ld_context_t *context){
    WOT_TD_ADD_CONTENT(thing, context, context, json_ld_context_t);
}

int wot_td_thing_context_rm(wot_td_thing_t *thing, json_ld_context_t *context){
    WOT_TD_RM_CONTENT(thing, context, context, json_ld_context_t);
}

json_ld_context_t * wot_td_thing_context_find_nth(wot_td_thing_t *thing, uint8_t pos){
    WOT_TD_NTH_CONTENT(thing, context, json_ld_context_t);
}

json_ld_context_t * wot_td_thing_context_find_key(wot_td_thing_t *thing, char *key){
    WOT_TD_FIND_x(thing, context, json_ld_context_t, key);
}

json_ld_context_t * wot_td_thing_context_find_value(wot_td_thing_t *thing, char *value){
    WOT_TD_FIND_x(thing, context, json_ld_context_t, value);
}

int wot_td_thing_title_add(wot_td_thing_t *thing, wot_td_multi_lang_t *title){
    WOT_TD_ADD_CONTENT(thing, titles, title, wot_td_multi_lang_t);
}

int wot_td_thing_title_rm(wot_td_thing_t *thing, wot_td_multi_lang_t *title){
    WOT_TD_RM_CONTENT(thing, titles, title, wot_td_multi_lang_t);
}

wot_td_multi_lang_t * wot_td_thing_titles_find_nth(wot_td_thing_t *thing, uint8_t pos){
    WOT_TD_NTH_CONTENT(thing, titles, wot_td_multi_lang_t);
}

wot_td_multi_lang_t * wot_td_thing_titles_find_tag(wot_td_thing_t *thing, char *tag){
    WOT_TD_FIND_x(thing, titles, wot_td_multi_lang_t, tag);
}

wot_td_multi_lang_t * wot_td_thing_titles_find_value(wot_td_thing_t *thing, char *value){
    WOT_TD_FIND_x(thing, titles, wot_td_multi_lang_t, value);
}

int wot_td_thing_desc_add(wot_td_thing_t *thing, wot_td_multi_lang_t *desc){
    WOT_TD_ADD_CONTENT(thing, descriptions, desc, wot_td_multi_lang_t);
}

int wot_td_thing_desc_rm(wot_td_thing_t *thing, wot_td_multi_lang_t *desc){
    WOT_TD_RM_CONTENT(thing, descriptions, desc, wot_td_multi_lang_t);
}

wot_td_multi_lang_t * wot_td_thing_desc_find_nth(wot_td_thing_t *thing, uint8_t pos){
    WOT_TD_NTH_CONTENT(thing, descriptions, wot_td_multi_lang_t);
}

wot_td_multi_lang_t * wot_td_thing_desc_find_tag(wot_td_thing_t *thing, char *tag){
    WOT_TD_FIND_x(thing, descriptions, wot_td_multi_lang_t, tag);
}

wot_td_multi_lang_t * wot_td_thing_desc_find_value(wot_td_thing_t *thing, char *value){
    WOT_TD_FIND_x(thing, descriptions, wot_td_multi_lang_t, value);
}

int wot_td_thing_type_add(wot_td_thing_t *thing, wot_td_type_t *type){
    WOT_TD_ADD_CONTENT(thing, type, type, wot_td_type_t);
}

int wot_td_thing_type_rm(wot_td_thing_t *thing, wot_td_type_t *type){
    WOT_TD_RM_CONTENT(thing, type, type, wot_td_type_t);
}

wot_td_type_t * wot_td_thing_type_find_nth(wot_td_thing_t *thing, uint8_t pos){
    WOT_TD_NTH_CONTENT(thing, type, wot_td_type_t);
}

wot_td_type_t * wot_td_thing_type_find_value(wot_td_thing_t *thing, char *value){
    WOT_TD_FIND_x(thing, type, wot_td_type_t, value);
}

int wot_td_thing_prop_add(wot_td_thing_t *thing, wot_td_prop_affordance_t *property){
    WOT_TD_ADD_CONTENT(thing, properties, property, wot_td_prop_affordance_t);
}

int wot_td_thing_prop_rm(wot_td_thing_t *thing, wot_td_prop_affordance_t *property){
    WOT_TD_RM_CONTENT(thing, properties, property, wot_td_prop_affordance_t);
}

wot_td_prop_affordance_t * wot_td_thing_prop_find_nth(wot_td_thing_t *thing, uint8_t pos){
    WOT_TD_NTH_CONTENT(thing, properties, wot_td_prop_affordance_t);
}

wot_td_prop_affordance_t * wot_td_thing_prop_find_key(wot_td_thing_t *thing, char *key){
    WOT_TD_FIND_x(thing, properties, wot_td_prop_affordance_t, key);
}

int wot_td_thing_action_add(wot_td_thing_t *thing, wot_td_action_affordance_t *action){
    WOT_TD_ADD_CONTENT(thing, actions, action, wot_td_action_affordance_t);
}

int wot_td_thing_action_rm(wot_td_thing_t *thing, wot_td_action_affordance_t *action){
    WOT_TD_RM_CONTENT(thing, actions, action, wot_td_action_affordance_t);
}

wot_td_action_affordance_t * wot_td_thing_action_find_nth(wot_td_thing_t *thing, uint8_t pos){
    WOT_TD_NTH_CONTENT(thing, actions, wot_td_action_affordance_t);
}

wot_td_action_affordance_t * wot_td_thing_action_find_key(wot_td_thing_t *thing, char *key){
    WOT_TD_FIND_x(thing, actions, wot_td_action_affordance_t, key);
}

int wot_td_thing_event_add(wot_td_thing_t *thing, wot_td_event_affordance_t *event){
    WOT_TD_ADD_CONTENT(thing, events, event, wot_td_event_affordance_t);
}

int wot_td_thing_event_rm(wot_td_thing_t *thing, wot_td_event_affordance_t *event){
    WOT_TD_RM_CONTENT(thing, events, event, wot_td_event_affordance_t);
}

wot_td_event_affordance_t * wot_td_thing_event_find_nth(wot_td_thing_t *thing, uint8_t pos){
    WOT_TD_NTH_CONTENT(thing, events, wot_td_event_affordance_t);
}

wot_td_event_affordance_t * wot_td_thing_event_find_key(wot_td_thing_t *thing, char *key){
    WOT_TD_FIND_x(thing, events, wot_td_event_affordance_t, key);
}

int wot_td_thing_security_add(wot_td_thing_t *thing, wot_td_security_t *security){
    WOT_TD_ADD_CONTENT(thing, security, security, wot_td_security_t);
}

int wot_td_thing_security_rm(wot_td_thing_t *thing, wot_td_security_t *security){
    WOT_TD_RM_CONTENT(thing, security, security, wot_td_security_t);
}

wot_td_security_t * wot_td_thing_security_find_nth(wot_td_thing_t *thing, uint8_t pos){
    WOT_TD_NTH_CONTENT(thing, security, wot_td_security_t);
}

wot_td_security_t * wot_td_thing_security_find_key(wot_td_thing_t *thing, char *key){
    WOT_TD_FIND_x(thing, security, wot_td_security_t, key);
}

int wot_td_thing_link_add(wot_td_thing_t *thing, wot_td_link_t *link){
    WOT_TD_ADD_CONTENT(thing, links, link, wot_td_link_t);
}

int wot_td_thing_link_rm(wot_td_thing_t *thing, wot_td_link_t *link){
    WOT_TD_RM_CONTENT(thing, links, link, wot_td_link_t);
}

wot_td_link_t * wot_td_thing_link_find_nth(wot_td_thing_t *thing, uint8_t pos){
    WOT_TD_NTH_CONTENT(thing, links, wot_td_link_t);
}

int wot_td_thing_form_add(wot_td_thing_t *thing, wot_td_form_t *form){
    WOT_TD_ADD_CONTENT(thing, forms, form, wot_td_form_t);
}

int wot_td_thing_form_rm(wot_td_thing_t *thing, wot_td_form_t *form){
    WOT_TD_RM_CONTENT(thing, forms, form, wot_td_form_t);
}

wot_td_form_t * wot_td_thing_form_find_nth(wot_td_thing_t *thing, uint8_t pos){
    WOT_TD_NTH_CONTENT(thing, forms, wot_td_form_t);
}

int wot_td_form_security_add(wot_td_form_t *form, wot_td_security_t *security){
    WOT_TD_ADD_CONTENT(form, security, security, wot_td_security_t);
}

int wot_td_form_security_rm(wot_td_form_t *form, wot_td_security_t *security){
    WOT_TD_RM_CONTENT(form, security, security, wot_td_security_t);
}

wot_td_security_t * wot_td_form_security_find_nth(wot_td_form_t *form, uint8_t pos){
    WOT_TD_NTH_CONTENT(form, security, wot_td_security_t);
}

wot_td_security_t * wot_td_form_security_find_key(wot_td_form_t *form, char *key){
    WOT_TD_FIND_x(form, security, wot_td_security_t, key);
}

int wot_td_form_auth_scope_add(wot_td_form_t *form, wot_td_auth_scopes_t *scope){
    WOT_TD_ADD_CONTENT(form, scopes, scope, wot_td_auth_scopes_t);
}

int wot_td_form_auth_scope_rm(wot_td_form_t *form, wot_td_auth_scopes_t *scope){
    WOT_TD_RM_CONTENT(form, scopes, scope, wot_td_auth_scopes_t);
}

wot_td_auth_scopes_t * wot_td_form_auth_scope_find_nth(wot_td_form_t *form, uint8_t pos){
    WOT_TD_NTH_CONTENT(form, scopes, wot_td_auth_scopes_t);
}

wot_td_auth_scopes_t * wot_td_form_auth_scope_find_value(wot_td_form_t *form, char *value){
    WOT_TD_FIND_x(form, scopes, wot_td_auth_scopes_t, value);
}

int wot_td_form_op_add(wot_td_form_t *form, wot_td_form_op_t *op){
    WOT_TD_ADD_CONTENT(form, op, op, wot_td_form_op_t);
}

int wot_td_form_op_rm(wot_td_form_t *form, wot_td_form_op_t *op){
    WOT_TD_RM_CONTENT(form, op, op, wot_td_form_op_t);
}

wot_td_form_op_t * wot_td_form_op_find_nth(wot_td_form_t *form, uint8_t pos){
    WOT_TD_NTH_CONTENT(form, op, wot_td_form_op_t);
}

int wot_td_affordance_title_add(wot_td_int_affordance_t *affordance, wot_td_multi_lang_t *title){
    WOT_TD_ADD_CONTENT(affordance, titles, title, wot_td_multi_lang_t);
}

int wot_td_affordance_title_rm(wot_td_int_affordance_t *affordance, wot_td_multi_lang_t *title){
    WOT_TD_RM_CONTENT(affordance, titles, title, wot_td_multi_lang_t);
}

wot_td_multi_lang_t * wot_td_affordance_title_find_nth(wot_td_int_affordance_t *affordance, uint8_t pos){
    WOT_TD_NTH_CONTENT(affordance, titles, wot_td_multi_lang_t);
}

wot_td_multi_lang_t * wot_td_affordance_title_find_tag(wot_td_int_affordance_t *affordance, char *tag){
    WOT_TD_FIND_x(affordance, titles, wot_td_multi_lang_t, tag);
}

wot_td_multi_lang_t * wot_td_affordance_title_find_value(wot_td_int_affordance_t *affordance, char *value){
    WOT_TD_FIND_x(affordance, titles, wot_td_multi_lang_t, value);
}

int wot_td_security_desc_add(wot_td_sec_scheme_t *security, wot_td_multi_lang_t *desc){
    WOT_TD_ADD_CONTENT(security, descriptions, desc, wot_td_multi_lang_t);
}

int wot_td_security_desc_rm(wot_td_sec_scheme_t *security, wot_td_multi_lang_t *desc){
    WOT_TD_RM_CONTENT(security, descriptions, desc, wot_td_multi_lang_t);
}

wot_td_multi_lang_t * wot_td_security_desc_find_nth(wot_td_sec_scheme_t *security, uint8_t pos){
    WOT_TD_NTH_CONTENT(security, descriptions, wot_td_multi_lang_t);
}

wot_td_multi_lang_t * wot_td_security_desc_find_tag(wot_td_sec_scheme_t *security, char *tag){
    WOT_TD_FIND_x(security, descriptions, wot_td_multi_lang_t, tag);
}

wot_td_multi_lang_t * wot_td_security_desc_find_value(wot_td_sec_scheme_t *security, char *value){
    WOT_TD_FIND_x(security, descriptions, wot_td_multi_lang_t, value);
}

int wot_td_oauth_auth_scope_add(wot_td_oauth2_sec_scheme_t *scheme, wot_td_auth_scopes_t *scope){
    WOT_TD_ADD_CONTENT(scheme, scopes, scope, wot_td_auth_scopes_t);
}

int wot_td_oauth_auth_scope_rm(wot_td_oauth2_sec_scheme_t *scheme, wot_td_auth_scopes_t *scope){
    WOT_TD_RM_CONTENT(scheme, scopes, scope, wot_td_auth_scopes_t);
}

wot_td_auth_scopes_t * wot_td_oauth_scope_find_nth(wot_td_oauth2_sec_scheme_t *scheme, uint8_t pos){
    WOT_TD_NTH_CONTENT(scheme, scopes, wot_td_auth_scopes_t);
}

wot_td_auth_scopes_t * wot_td_oauth_scope_find_value(wot_td_oauth2_sec_scheme_t *scheme, char *value){
    WOT_TD_FIND_x(scheme, scopes, wot_td_auth_scopes_t, value);
}

int wot_td_data_schema_desc_add(wot_td_data_schema_t *schema, wot_td_multi_lang_t *desc){
    WOT_TD_ADD_CONTENT(schema, descriptions, desc, wot_td_multi_lang_t);
}

int wot_td_data_schema_desc_rm(wot_td_data_schema_t *schema, wot_td_multi_lang_t *desc){
    WOT_TD_RM_CONTENT(schema, descriptions, desc, wot_td_multi_lang_t);
}

wot_td_multi_lang_t * wot_td_data_schema_desc_find_nth(wot_td_data_schema_t *schema, uint8_t pos){
    WOT_TD_NTH_CONTENT(schema, descriptions, wot_td_multi_lang_t);
}

wot_td_multi_lang_t * wot_td_data_schema_desc_find_tag(wot_td_data_schema_t *schema, char *tag){
    WOT_TD_FIND_x(schema, descriptions, wot_td_multi_lang_t, tag);
}

wot_td_multi_lang_t * wot_td_data_schema_desc_find_value(wot_td_data_schema_t *schema, char *value){
    WOT_TD_FIND_x(schema, descriptions, wot_td_multi_lang_t, value);
}

int wot_td_data_schema_title_add(wot_td_data_schema_t *schema, wot_td_multi_lang_t *title){
    WOT_TD_ADD_CONTENT(schema, titles, title, wot_td_multi_lang_t);
}

int wot_td_data_schema_title_rm(wot_td_data_schema_t *schema, wot_td_multi_lang_t *title){
    WOT_TD_RM_CONTENT(schema, titles, title, wot_td_multi_lang_t);
}

wot_td_multi_lang_t * wot_td_data_data_schema_title_find_nth(wot_td_data_schema_t *schema, uint8_t pos){
    WOT_TD_NTH_CONTENT(schema, titles, wot_td_multi_lang_t);
}

wot_td_multi_lang_t * wot_td_data_schema_title_find_tag(wot_td_data_schema_t *schema, char *tag){
    WOT_TD_FIND_x(schema, titles, wot_td_multi_lang_t, tag);
}

wot_td_multi_lang_t * wot_td_data_schema_title_find_value(wot_td_data_schema_t *schema, char *value){
    WOT_TD_FIND_x(schema, titles, wot_td_multi_lang_t, value);
}

int wot_td_data_schema_validation_add(wot_td_data_schema_t *schema, wot_td_data_schemas_t *validator){
    WOT_TD_ADD_CONTENT(schema, one_of, validator, wot_td_data_schemas_t);
}

int wot_td_data_schema_validation_rm(wot_td_data_schema_t *schema, wot_td_data_schemas_t *validator){
    WOT_TD_RM_CONTENT(schema, one_of, validator, wot_td_data_schemas_t);
}

wot_td_data_schemas_t * wot_td_data_data_schema_validator_find_nth(wot_td_data_schema_t *schema, uint8_t pos){
    WOT_TD_NTH_CONTENT(schema, one_of, wot_td_data_schemas_t);
}

int wot_td_data_schema_enum_add(wot_td_data_schema_t *schema, wot_td_data_enums_t *enumeration){
    WOT_TD_ADD_CONTENT(schema, enumeration, enumeration, wot_td_data_enums_t);
}

int wot_td_data_schema_enum_rm(wot_td_data_schema_t *schema, wot_td_data_enums_t *enumeration){
    WOT_TD_RM_CONTENT(schema, enumeration, enumeration, wot_td_data_enums_t);
}

wot_td_data_enums_t * wot_td_data_schema_enum_find_nth(wot_td_data_schema_t *schema, uint8_t pos){
    WOT_TD_NTH_CONTENT(schema, enumeration, wot_td_data_enums_t);
}

wot_td_data_enums_t * wot_td_data_schema_enum_find_value(wot_td_data_schema_t *schema, char *value){
    WOT_TD_FIND_x(schema, enumeration, wot_td_data_enums_t, value);
}
