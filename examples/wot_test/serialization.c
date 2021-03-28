#include "wot.h"
#include "serialization.h"
#include "wot_internal.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void _reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void _itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    _reverse(s);
}

char * _wot_td_fill_json_buffer(char *buffer, char *string, uint32_t length){
    memcpy(buffer, string, length);
    return &(buffer[length]);
}

char * _wot_td_fill_json_string(char *buffer, char *string, uint32_t length){
    buffer = _wot_td_fill_json_buffer(buffer, "\"", 1);
    buffer = _wot_td_fill_json_buffer(buffer, string, length);
    return _wot_td_fill_json_buffer(buffer, "\"", 1);
}

char * _wot_td_fill_json_uri(char *buffer, wot_td_uri_t *uri){
    buffer = _wot_td_fill_json_buffer(buffer, "\"", 1);
    buffer = _wot_td_fill_json_buffer(buffer, uri->schema, strlen(uri->schema));
    buffer = _wot_td_fill_json_buffer(buffer, uri->value, strlen(uri->value));
    return _wot_td_fill_json_buffer(buffer, "\"", 1);
}

char * _wot_td_fill_json_date(char *buffer, wot_td_date_time_t *date){
    buffer = _wot_td_fill_json_buffer(buffer, "\"", 1);
    char s[11];
    _itoa(date->year, s);
    buffer = _wot_td_fill_json_buffer(buffer, s, strlen(s));
    buffer = _wot_td_fill_json_buffer(buffer, "-", 1);
    _itoa(date->month, s);
    buffer = _wot_td_fill_json_buffer(buffer, s, strlen(s));
    buffer = _wot_td_fill_json_buffer(buffer, "-", 1);
    _itoa(date->day, s);
    buffer = _wot_td_fill_json_buffer(buffer, s, strlen(s));
    buffer = _wot_td_fill_json_buffer(buffer, "T", 1);
    _itoa(date->hour, s);
    buffer = _wot_td_fill_json_buffer(buffer, s, strlen(s));
    buffer = _wot_td_fill_json_buffer(buffer, ":", 1);
    _itoa(date->minute, s);
    buffer = _wot_td_fill_json_buffer(buffer, s, strlen(s));
    buffer = _wot_td_fill_json_buffer(buffer, ":", 1);
    _itoa(date->second, s);
    buffer = _wot_td_fill_json_buffer(buffer, s, strlen(s));
    _itoa(date->timezone_offset, s);
    buffer = _wot_td_fill_json_buffer(buffer, s, strlen(s));

    return _wot_td_fill_json_buffer(buffer, "\"", 1);
}

char * _wot_td_fill_json_obj_key(char *buffer, char *string, uint32_t length){
    buffer = _wot_td_fill_json_string(buffer, string, length);
    return _wot_td_fill_json_buffer(buffer, ":", 1);
}

char * _wot_td_fill_json_bool(char *buffer, bool value){
    if(value){
        char c[] = "true";
        buffer = _wot_td_fill_json_buffer(buffer, c, sizeof(c)-1);
    }else{
        char c[] = "false";
        buffer = _wot_td_fill_json_buffer(buffer, c, sizeof(c)-1);
    }

    return buffer;
}

char * _previous_prop_check(char *buffer, uint32_t max_length, bool has_previous_prop){
    (void)max_length;
    if(has_previous_prop){
        return _wot_td_fill_json_buffer(buffer, ",", 1);
    }else{
        return buffer;
    }
}

char * _serialize_context(char *buffer, uint32_t max_length, json_ld_context_t *context){
    (void)max_length;

    if(context->key != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, "{", 1);

        buffer = _wot_td_fill_json_obj_key(buffer, context->key, strlen(context->key));
    }

    buffer = _wot_td_fill_json_string(buffer, context->value, strlen(context->value));

    if(context->key != NULL) {
        buffer = _wot_td_fill_json_buffer(buffer, "}", 1);
    }
    return buffer;
}

char * _serialize_context_array(char *buffer, uint32_t max_length, json_ld_context_t *context){
    buffer = _wot_td_fill_json_buffer(buffer, "[", 1);
    json_ld_context_t *tmp_ctx = context;
    max_length = max_length-2;

    while(tmp_ctx != NULL){
        buffer = _serialize_context(buffer, max_length, tmp_ctx);
        if(tmp_ctx != NULL && tmp_ctx->next != NULL){
            buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        }
        tmp_ctx = tmp_ctx->next;
    }
    return _wot_td_fill_json_buffer(buffer, "]", 1);
}

char * _serialize_type_array(char *buffer, uint32_t max_length, wot_td_type_t *type){
    (void)max_length;
    char type_obj_key[] = "@type";
    buffer = _wot_td_fill_json_obj_key(buffer, type_obj_key, sizeof(type_obj_key)-1);
    buffer = _wot_td_fill_json_buffer(buffer, "[", 1);
    wot_td_type_t *tmp_type = type;
    while(tmp_type != NULL){
        buffer = _wot_td_fill_json_string(buffer, tmp_type->value, strlen(tmp_type->value));
        if(tmp_type != NULL && tmp_type->next != NULL){
            buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        }
        tmp_type = tmp_type->next;
    }
    return _wot_td_fill_json_buffer(buffer, "]", 1);
}

char * _serialize_lang(char *buffer, uint32_t max_length, wot_td_multi_lang_t *lang){
    (void)max_length;
    buffer = _wot_td_fill_json_buffer(buffer, "{", 1);
    buffer = _wot_td_fill_json_obj_key(buffer, lang->tag, strlen(lang->tag));
    buffer = _wot_td_fill_json_string(buffer, lang->value, strlen(lang->value));
    return _wot_td_fill_json_buffer(buffer, "}", 1);
}

char * _serialize_title_array(char *buffer, uint32_t max_length, wot_td_multi_lang_t *titles, char *lang){
    wot_td_multi_lang_t *tmp = titles;
    wot_td_multi_lang_t *default_title = NULL;
    char title_obj_key[] = "titles";
    buffer = _wot_td_fill_json_obj_key(buffer, title_obj_key, sizeof(title_obj_key)-1);
    buffer = _wot_td_fill_json_buffer(buffer, "[", 1);
    while(tmp != NULL){
        if(lang == NULL || strcmp(tmp->tag, lang) != 0){
            buffer = _serialize_lang(buffer, max_length, tmp);
        }else{
            default_title = tmp;
        }

        if(tmp->next != NULL){
            buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        }
        tmp = tmp->next;
    }
    buffer = _wot_td_fill_json_buffer(buffer, "]", 1);

    if(default_title != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        char key[] = "title";
        buffer = _wot_td_fill_json_obj_key(buffer, key, sizeof(key)-1);
        buffer = _wot_td_fill_json_string(buffer, default_title->value, strlen(default_title->value));
    }

    return buffer;
}

char * _serialize_description_array(char *buffer, uint32_t max_length, wot_td_multi_lang_t *desc, char *lang){
    wot_td_multi_lang_t *tmp = desc;
    wot_td_multi_lang_t *default_description = NULL;
    char description_obj_key[] = "descriptions";
    buffer = _wot_td_fill_json_obj_key(buffer, description_obj_key, sizeof(description_obj_key)-1);
    buffer = _wot_td_fill_json_buffer(buffer, "[", 1);
    while(tmp != NULL){
        if(lang == NULL || strcmp(tmp->tag, lang) != 0){
            buffer = _serialize_lang(buffer, max_length, tmp);
        }else{
            default_description = tmp;
        }

        if(tmp->next != NULL){
            buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        }
        tmp = tmp->next;
    }
    buffer = _wot_td_fill_json_buffer(buffer, "]", 1);

    if(default_description != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        char key[] = "description";
        buffer = _wot_td_fill_json_obj_key(buffer, key, sizeof(key)-1);
        buffer = _wot_td_fill_json_string(buffer, default_description->value, strlen(default_description->value));
    }

    return buffer;
}

void _security_scheme_string(char *result, wot_td_sec_scheme_type_t scheme_type){
    switch (scheme_type) {
        case SECURITY_SCHEME_NONE:
            strcpy(result, "nosec");
            break;
        case SECURITY_SCHEME_BASIC:
            strcpy(result, "basic");
            break;
        case SECURITY_SCHEME_DIGEST:
            strcpy(result, "digest");
            break;
        case SECURITY_SCHEME_API_KEY:
            strcpy(result, "apikey");
            break;
        case SECURITY_SCHEME_BEARER:
            strcpy(result, "bearer");
            break;
        case SECURITY_SCHEME_PSK:
            strcpy(result, "psk");
            break;
        case SECURITY_SCHEME_OAUTH2:
            strcpy(result, "oauth2");
            break;
        default:
            strcpy(result, "nosec");
            break;
    }
}

void _security_schema_in_string(char *result, wot_td_sec_scheme_in_t in){
    switch(in){
        case SECURITY_SCHEME_IN_HEADER:
            strcpy(result, "header");
            break;
        case SECURITY_SCHEME_IN_QUERY:
            strcpy(result, "query");
            break;
        case SECURITY_SCHEME_IN_BODY:
            strcpy(result, "body");
            break;
        case SECURITY_SCHEME_IN_COOKIE:
            strcpy(result, "cookie");
            break;
        default:
            strcpy(result, "header");
    }
}

char * _serialize_sec_scheme_basic(char *buffer, uint32_t max_length, wot_td_basic_sec_scheme_t *scheme){
    (void)max_length;
    char in_obj_key[] = "in";
    char name_obj_key[] = "name";
    char sec_in_value[7];
    _security_schema_in_string(sec_in_value, scheme->in);

    buffer = _wot_td_fill_json_obj_key(buffer, in_obj_key, sizeof(in_obj_key)-1);
    buffer = _wot_td_fill_json_string(buffer, sec_in_value, strlen(sec_in_value));
    buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
    buffer = _wot_td_fill_json_obj_key(buffer, name_obj_key, sizeof(name_obj_key)-1);
    buffer = _wot_td_fill_json_string(buffer, scheme->name, strlen(scheme->name));

    return buffer;
}

void _security_digest_qop_string(char *result, wot_td_digest_qop_t qop){
    if(qop == SECURITY_DIGEST_QOP_AUTH_INT){
        strcpy(result, "auth-int");
    }else{
        strcpy(result, "auth");
    }
}

char * _serialize_sec_scheme_digest(char *buffer, uint32_t max_length, wot_td_digest_sec_scheme_t *scheme){
    (void)max_length;
    char in_obj_key[] = "in";
    char name_obj_key[] = "name";
    char qop_obj_key[] = "qop";
    char qop_value[9];
    char sec_in_value[7];
    _security_digest_qop_string(qop_value, scheme->qop);
    _security_schema_in_string(sec_in_value, scheme->in);

    buffer = _wot_td_fill_json_obj_key(buffer, qop_obj_key, sizeof(qop_obj_key)-1);
    buffer = _wot_td_fill_json_string(buffer, qop_value, strlen(qop_value));
    buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
    buffer = _wot_td_fill_json_obj_key(buffer, in_obj_key, sizeof(in_obj_key)-1);
    buffer = _wot_td_fill_json_string(buffer, sec_in_value, strlen(sec_in_value));
    buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
    buffer = _wot_td_fill_json_obj_key(buffer, name_obj_key, sizeof(name_obj_key)-1);
    buffer = _wot_td_fill_json_string(buffer, scheme->name, strlen(scheme->name));

    return buffer;
}

//Todo: Implement
char * _serialize_sec_scheme_api_key(char *buffer, uint32_t max_length, wot_td_api_key_sec_scheme_t *scheme){
    (void)max_length;
    (void)buffer;
    (void)scheme;
    return buffer;
}

//Todo: Implement
char * _serialize_sec_scheme_bearer(char *buffer, uint32_t max_length, wot_td_bearer_sec_scheme_t *scheme){
    (void)max_length;
    (void)buffer;
    (void)scheme;
    return buffer;
}

//Todo: Implement
char * _serialize_sec_scheme_psk(char *buffer, uint32_t max_length, wot_td_psk_sec_scheme_t *scheme){
    (void)max_length;
    (void)buffer;
    (void)scheme;
    return buffer;
}

//Todo: Implement
char * _serialize_sec_scheme_oauth2(char *buffer, uint32_t max_length, wot_td_oauth2_sec_scheme_t *scheme){
    (void)max_length;
    (void)buffer;
    (void)scheme;
    return buffer;
}

char * _serialize_security_schema(char *buffer, uint32_t max_length, wot_td_sec_scheme_t *security){
    switch (security->scheme_type) {
        default:
            return buffer;
        case SECURITY_SCHEME_BASIC:
            return _serialize_sec_scheme_basic(buffer, max_length, (wot_td_basic_sec_scheme_t *) security->scheme);
        case SECURITY_SCHEME_DIGEST:
            return _serialize_sec_scheme_digest(buffer, max_length, (wot_td_digest_sec_scheme_t *) security->scheme);
        case SECURITY_SCHEME_API_KEY:
            return _serialize_sec_scheme_api_key(buffer, max_length, (wot_td_api_key_sec_scheme_t *) security->scheme);
        case SECURITY_SCHEME_BEARER:
            return _serialize_sec_scheme_bearer(buffer, max_length, (wot_td_bearer_sec_scheme_t *) security->scheme);
        case SECURITY_SCHEME_PSK:
            return _serialize_sec_scheme_psk(buffer, max_length, (wot_td_psk_sec_scheme_t *) security->scheme);
        case SECURITY_SCHEME_OAUTH2:
            return _serialize_sec_scheme_oauth2(buffer, max_length, (wot_td_oauth2_sec_scheme_t *) security->scheme);
    }
}

//Todo: Full implementation
char * _serialize_security_array(char *buffer, uint32_t max_length, wot_td_security_t *security, char *lang){
    wot_td_security_t *tmp_sec = security;
    wot_td_sec_scheme_t *scheme = NULL;
    char security_name[10];
    char schema_obj_key[] = "scheme";
    char security_def_obj_key[] = "securityDefinitions";
    buffer = _wot_td_fill_json_obj_key(buffer, security_def_obj_key, sizeof(security_def_obj_key)-1);
    buffer = _wot_td_fill_json_buffer(buffer, "{", 1);
    while (tmp_sec != NULL){
        scheme = tmp_sec->value;
        buffer = _wot_td_fill_json_obj_key(buffer, tmp_sec->key, strlen(tmp_sec->key));
        buffer = _wot_td_fill_json_buffer(buffer, "{", 1);
        _security_scheme_string(security_name, scheme->scheme_type);
        buffer = _wot_td_fill_json_obj_key(buffer, schema_obj_key, sizeof(schema_obj_key)-1);
        buffer = _wot_td_fill_json_string(buffer, security_name, strlen(security_name));
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        buffer = _serialize_description_array(buffer, max_length, scheme->descriptions, lang);
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        buffer = _serialize_security_schema(buffer, max_length, scheme);
        buffer = _wot_td_fill_json_buffer(buffer, "}", 1);
        tmp_sec = tmp_sec->next;
    }
    buffer = _wot_td_fill_json_buffer(buffer, "}", 1);

    buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
    char security_obj_key[] = "security";
    buffer = _wot_td_fill_json_obj_key(buffer, security_obj_key, sizeof(security_obj_key)-1);
    buffer = _wot_td_fill_json_buffer(buffer, "[", 1);
    tmp_sec = security;
    while (tmp_sec != NULL){
        scheme = tmp_sec->value;
        _security_scheme_string(security_name, scheme->scheme_type);
        buffer = _wot_td_fill_json_string(buffer, security_name, strlen(security_name));
        tmp_sec = tmp_sec->next;
    }
    buffer = _wot_td_fill_json_buffer(buffer, "]", 1);
    return buffer;
}

void _form_op_type_string(char *result, wot_td_form_op_type_t op_type){
    switch (op_type) {
        case FORM_OP_READ_PROPERTY:
            strcpy(result, "readproperty");
            break;
        case FORM_OP_WRITE_PROPERTY:
            strcpy(result, "writeproperty");
            break;
        case FORM_OP_OBSERVE_PROPERTY:
            strcpy(result, "observeproperty");
            break;
        case FORM_OP_UNOBSERVE_PROPERTY:
            strcpy(result, "unobserveproperty");
            break;
        case FORM_OP_INVOKE_ACTION:
            strcpy(result, "invokeaction");
            break;
        case FORM_OP_SUBSCRIBE_EVENT:
            strcpy(result, "subscribeevent");
            break;
        case FORM_OP_UNSUBSCRIBE_EVENT:
            strcpy(result, "unsubscribeevent");
            break;
        case FORM_OP_READ_ALL_PROPERTIES:
            strcpy(result, "readallproperties");
            break;
        case FORM_OP_WRITE_ALL_PROPERTIES:
            strcpy(result, "writeallproperties");
            break;
        case FORM_OP_READ_MULTIPLE_PROPERTIES:
            strcpy(result, "readmultipleproperties");
            break;
        case FORM_OP_WRITE_MULTIPLE_PROPERTIES:
            strcpy(result, "writemultipleproperties");
            break;
        default:
            strcpy(result, "");
            break;
    }
}

char * _serialize_op_array(char *buffer, uint32_t max_length, wot_td_form_op_t *op){
    (void)max_length;
    wot_td_form_op_t *tmp = op;
    char op_obj_name[25];
    buffer = _wot_td_fill_json_buffer(buffer, "[", 1);
    while(tmp != NULL){
        _form_op_type_string(op_obj_name, tmp->op_type);
        buffer = _wot_td_fill_json_string(buffer, op_obj_name, strlen(op_obj_name));
        tmp = tmp->next;
    }
    buffer = _wot_td_fill_json_buffer(buffer, "]", 1);

    return buffer;
}

void _content_type_string(char *result, wot_td_content_type_t content_type){
    switch (content_type) {
        case CONTENT_TYPE_JSON:
            strcpy(result, "application/json");
            break;
        default:
            strcpy(result, "");
            break;
    }
}

void _content_encoding_string(char *result, wot_td_content_encoding_type_t encoding){
    switch (encoding) {
        case CONTENT_ENCODING_GZIP:
            strcpy(result, "gzip");
            break;
        default:
            strcpy(result, "");
            break;
    }
}

char * _serialize_form_array(char *buffer, uint32_t max_length, wot_td_form_t *form){
    wot_td_form_t *tmp = form;
    char op_obj_key[] = "op";
    char href_obj_key[] = "href";

    buffer = _wot_td_fill_json_buffer(buffer, "[", 1);
    while (tmp != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, "{", 1);

        if(tmp->op != NULL){
            buffer = _wot_td_fill_json_obj_key(buffer, op_obj_key, sizeof(op_obj_key)-1);
            buffer = _serialize_op_array(buffer, max_length, tmp->op);
            buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        }
        buffer = _wot_td_fill_json_obj_key(buffer, href_obj_key, sizeof(href_obj_key)-1);
        buffer = _wot_td_fill_json_uri(buffer, tmp->href);

        if(tmp->content_type != NULL){
            char content_type[20];
            char content_type_obj_key[] = "contentType";
            _content_type_string(content_type, *tmp->content_type);
            buffer = _wot_td_fill_json_obj_key(buffer, content_type_obj_key, sizeof(content_type_obj_key)-1);
            buffer = _wot_td_fill_json_string(buffer, content_type, strlen(content_type));
        }

        if(tmp->content_encoding != NULL){
            char content_encoding[10];
            char content_encoding_obj_key[] = "contentCoding";
            _content_encoding_string(content_encoding, *tmp->content_encoding);
            buffer = _wot_td_fill_json_obj_key(buffer, content_encoding_obj_key, sizeof(content_encoding_obj_key)-1);
            buffer = _wot_td_fill_json_string(buffer, content_encoding, strlen(content_encoding));
        }

        if(tmp->sub_protocol != NULL){
            char sub_protocol_obj_key[] = "subprotocol";
            buffer = _wot_td_fill_json_obj_key(buffer, sub_protocol_obj_key, sizeof(sub_protocol_obj_key)-1);
            buffer = _wot_td_fill_json_string(buffer, tmp->sub_protocol, strlen(tmp->sub_protocol));
        }

        if(tmp->security != NULL){
            char security_obj_key[] = "security";
            buffer = _wot_td_fill_json_obj_key(buffer, security_obj_key, sizeof(security_obj_key)-1);
            wot_td_security_t *sec = tmp->security;
            buffer = _wot_td_fill_json_buffer(buffer, "[", 1);
            while (sec != NULL){
                buffer = _wot_td_fill_json_string(buffer, sec->key, strlen(sec->key));
                sec = sec->next;
            }
            buffer = _wot_td_fill_json_buffer(buffer, "]", 1);
        }

        if(tmp->scopes != NULL){
            char scopes_obj_key[] = "scopes";
            buffer = _wot_td_fill_json_obj_key(buffer, scopes_obj_key, sizeof(scopes_obj_key)-1);
            wot_td_auth_scopes_t *scope = tmp->scopes;
            buffer = _wot_td_fill_json_buffer(buffer, "[", 1);
            while(scope != NULL){
                buffer = _wot_td_fill_json_string(buffer, scope->value, strlen(scope->value));
                scope = scope->next;
            }
            buffer = _wot_td_fill_json_buffer(buffer, "]", 1);
        }

        //Todo: Continue

        buffer = _wot_td_fill_json_buffer(buffer, "}", 1);
        tmp = tmp->next;
    }
    buffer = _wot_td_fill_json_buffer(buffer, "]", 1);

    return buffer;
}

//Todo: Implement other properties
char * _serialize_int_aff(char *buffer, uint32_t max_length, wot_td_int_affordance_t *int_aff){
    char forms_obj_key[] = "forms";
    buffer = _wot_td_fill_json_obj_key(buffer, forms_obj_key, sizeof(forms_obj_key)-1);
    buffer = _serialize_form_array(buffer, max_length, int_aff->forms);

    return buffer;
}

char * _serialize_prop_aff_array(char *buffer, uint32_t max_length, wot_td_prop_affordance_t *prop_aff){
    wot_td_prop_affordance_t *tmp = prop_aff;
    char prop_aff_obj_key[] = "properties";
    char observable_obj_key[] = "observable";

    buffer = _wot_td_fill_json_obj_key(buffer, prop_aff_obj_key, sizeof(prop_aff_obj_key)-1);
    buffer = _wot_td_fill_json_buffer(buffer, "{", 1);
    while(tmp != NULL){
        buffer = _wot_td_fill_json_obj_key(buffer, tmp->key, strlen(tmp->key));
        buffer = _wot_td_fill_json_buffer(buffer, "{", 1);
        buffer = _wot_td_fill_json_obj_key(buffer, observable_obj_key, sizeof(observable_obj_key)-1);
        buffer = _wot_td_fill_json_bool(buffer, tmp->observable);
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        buffer = _serialize_int_aff(buffer, max_length, tmp->int_affordance);
        buffer = _wot_td_fill_json_buffer(buffer, "}", 1);
        tmp = tmp->next;
    }

    return _wot_td_fill_json_buffer(buffer, "}", 1);
}

char * _serialize_data_schema(char *buffer, uint32_t max_length, wot_td_data_schema_t *data_schema, char *lang);

char * _serialize_data_schema_object(char *buffer, uint32_t max_length, wot_td_object_schema_t *schema, char *lang){
    char properties_obj_key[] = "properties";
    buffer = _wot_td_fill_json_obj_key(buffer, properties_obj_key, sizeof(properties_obj_key)-1);
    wot_td_data_schema_map_t *property = schema->properties;
    buffer = _wot_td_fill_json_buffer(buffer, "{", 1);
    while (property != NULL){
        buffer = _wot_td_fill_json_obj_key(buffer, property->key, strlen(property->key));
        buffer = _serialize_data_schema(buffer, max_length, property->value, lang);
        property = property->next;
    }
    buffer = _wot_td_fill_json_buffer(buffer, "}", 1);
    buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
    char required_obj_key[] = "required";
    buffer = _wot_td_fill_json_obj_key(buffer, required_obj_key, sizeof(required_obj_key)-1);
    wot_td_object_required_t *required = schema->required;
    buffer = _wot_td_fill_json_buffer(buffer, "[", 1);
    while (required != NULL){
        buffer = _wot_td_fill_json_string(buffer, required->value, strlen(required->value));
        required = required->next;
    }
    buffer = _wot_td_fill_json_buffer(buffer, "]", 1);
    return buffer;
}

char * _serialize_data_schema_array(char *buffer, uint32_t max_length, wot_td_array_schema_t *schema, char *lang){
    if(schema->items != NULL){
        char item_obj_key[] = "items";
        buffer = _wot_td_fill_json_obj_key(buffer, item_obj_key, sizeof(item_obj_key)-1);
        wot_td_data_schemas_t *item = schema->items;
        buffer = _wot_td_fill_json_buffer(buffer, "[", 1);
        while (item != NULL){
            buffer = _serialize_data_schema(buffer, max_length, item->value, lang);
            item = item->next;
        }
        buffer = _wot_td_fill_json_buffer(buffer, "]", 1);
    }

    if(schema->min_items != NULL){
        if(schema->items != NULL){
            buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        }
        char min_items_obj_key[] = "minItems";
        buffer = _wot_td_fill_json_obj_key(buffer, min_items_obj_key, sizeof(min_items_obj_key) - 1);
        char min_items_output[32];
        _itoa(*schema->min_items, min_items_output);
        buffer = _wot_td_fill_json_string(buffer, min_items_output, strlen(min_items_output));
    }

    if(schema->max_items != NULL){
        if(schema->min_items != NULL || schema->items != NULL){
            buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        }
        char max_items_obj_key[] = "maxItems";
        buffer = _wot_td_fill_json_obj_key(buffer, max_items_obj_key, sizeof(max_items_obj_key)-1);
        char max_items_output[32];
        _itoa(*schema->max_items, max_items_output);
        buffer = _wot_td_fill_json_string(buffer, max_items_output, strlen(max_items_output));
    }
    return buffer;
}

char * _serialize_data_schema_number(char *buffer, uint32_t max_length, wot_td_number_schema_t *schema){
    (void)max_length;
    if(schema->minimum != NULL){
        char min_obj_key[] = "minimum";
        buffer = _wot_td_fill_json_obj_key(buffer, min_obj_key, sizeof(min_obj_key)-1);
        //Todo: Implement double to string conversion
    }

    return buffer;
}

char * _serialize_data_schema_int(char *buffer, uint32_t max_length, wot_td_integer_schema_t *schema){
    (void)max_length;
    if(schema->minimum != NULL){
        char min_obj_key[] = "minimum";
        buffer = _wot_td_fill_json_obj_key(buffer, min_obj_key, sizeof(min_obj_key)-1);
        char min_output[23];
        _itoa(*schema->minimum, min_output);
        buffer = _wot_td_fill_json_string(buffer, min_output, strlen(min_output));
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);

        char max_obj_key[] = "maximum";
        buffer = _wot_td_fill_json_obj_key(buffer, max_obj_key, sizeof(max_obj_key)-1);
        char max_output[23];
        _itoa(*schema->minimum, max_output);
        buffer = _wot_td_fill_json_string(buffer, max_output, strlen(max_output));
    }
    return buffer;
}

char * _serialize_data_schema_subclass(char *buffer, uint32_t max_length, wot_td_data_schema_t *data_schema, char *lang){
    switch (*data_schema->json_type) {
        case JSON_TYPE_OBJECT:
            return _serialize_data_schema_object(buffer, max_length, (wot_td_object_schema_t *) data_schema->schema, lang);
        case JSON_TYPE_ARRAY:
            return _serialize_data_schema_array(buffer, max_length, (wot_td_array_schema_t *) data_schema->schema, lang);
        case JSON_TYPE_NUMBER:
            return _serialize_data_schema_number(buffer, max_length, (wot_td_number_schema_t *) data_schema->schema);
        case JSON_TYPE_INTEGER:
            return _serialize_data_schema_int(buffer, max_length, (wot_td_integer_schema_t *) data_schema->schema);
        default:
            return buffer;
    }
}

//Todo: Implement
char * _serialize_data_schema(char *buffer, uint32_t max_length, wot_td_data_schema_t *data_schema, char *lang){
    bool has_previous_prop = false;
    buffer = _wot_td_fill_json_buffer(buffer, "{", 1);

    if(data_schema->type != NULL){
        has_previous_prop = true;
        wot_td_type_t *type = data_schema->type;
        char type_obj_key[] = "@type";
        buffer = _wot_td_fill_json_obj_key(buffer, type_obj_key, sizeof(type_obj_key)-1);

        while(type != NULL){
            buffer = _wot_td_fill_json_string(buffer, type->value, strlen(type->value));
            if(type->next != NULL){
                buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
            }
            type = type->next;
        }
    }

    if(data_schema->titles != NULL){
        buffer = _previous_prop_check(buffer, max_length, has_previous_prop);
        has_previous_prop = true;
        buffer = _serialize_title_array(buffer, max_length, data_schema->titles, lang);
    }

    if(data_schema->descriptions != NULL){
        buffer = _previous_prop_check(buffer, max_length, has_previous_prop);
        has_previous_prop = true;
        buffer = _serialize_description_array(buffer, max_length, data_schema->descriptions, lang);
    }

    if(data_schema->json_type != NULL){
        buffer = _previous_prop_check(buffer, max_length, has_previous_prop);
        has_previous_prop = true;
        buffer = _serialize_data_schema_subclass(buffer, max_length, data_schema, lang);
    }

    if(data_schema->constant != NULL){
        buffer = _previous_prop_check(buffer, max_length, has_previous_prop);
        has_previous_prop = true;
        char constant_obj_key[] = "const";
        buffer = _wot_td_fill_json_obj_key(buffer, constant_obj_key, sizeof(constant_obj_key)-1);
        buffer = _wot_td_fill_json_string(buffer, data_schema->constant, strlen(data_schema->constant));
    }

    if(data_schema->unit != NULL){
        buffer = _previous_prop_check(buffer, max_length, has_previous_prop);
        has_previous_prop = true;
        char unit_obj_key[] = "unit";
        buffer = _wot_td_fill_json_obj_key(buffer, unit_obj_key, sizeof(unit_obj_key)-1);
        buffer = _wot_td_fill_json_string(buffer, data_schema->unit, strlen(data_schema->unit));
    }

    if(data_schema->one_of != NULL){
        buffer = _previous_prop_check(buffer, max_length, has_previous_prop);
        has_previous_prop = true;
        char one_of_obj_key[] = "oneOf";
        buffer = _wot_td_fill_json_obj_key(buffer, one_of_obj_key, sizeof(one_of_obj_key)-1);
        wot_td_data_schemas_t *tmp = data_schema->one_of;
        buffer = _wot_td_fill_json_string(buffer, "[", 1);
        while (tmp != NULL){
            buffer = _serialize_data_schema(buffer, max_length, tmp->value, lang);
            tmp = tmp->next;
        }
        buffer = _wot_td_fill_json_string(buffer, "]", 1);
    }

    if(data_schema->enumeration != NULL){
        buffer = _previous_prop_check(buffer, max_length, has_previous_prop);
        has_previous_prop = true;
        char enum_obj_key[] = "enum";
        buffer = _wot_td_fill_json_obj_key(buffer, enum_obj_key, sizeof(enum_obj_key)-1);
        wot_td_data_enums_t *tmp = data_schema->enumeration;
        buffer = _wot_td_fill_json_string(buffer, "[", 1);
        while (tmp != NULL){
            buffer = _wot_td_fill_json_string(buffer, tmp->value, strlen(tmp->value));
            tmp = tmp->next;
        }
        buffer = _wot_td_fill_json_string(buffer, "]", 1);
    }

    buffer = _previous_prop_check(buffer, max_length, has_previous_prop);
    char read_only_obj_key[] = "readOnly";
    buffer = _wot_td_fill_json_obj_key(buffer, read_only_obj_key, sizeof(read_only_obj_key)-1);
    buffer = _wot_td_fill_json_bool(buffer, data_schema->read_only);

    buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
    char write_only_obj_key[] = "writeOnly";
    buffer = _wot_td_fill_json_obj_key(buffer, write_only_obj_key, sizeof(write_only_obj_key)-1);
    buffer = _wot_td_fill_json_bool(buffer, data_schema->write_only);

    if(data_schema->format != NULL){
        buffer = _wot_td_fill_json_string(buffer, ",", 1);
        char format_obj_key[] = "format";
        buffer = _wot_td_fill_json_obj_key(buffer, format_obj_key, sizeof(format_obj_key)-1);
        buffer = _wot_td_fill_json_string(buffer, data_schema->format, strlen(data_schema->format));
    }

    buffer = _wot_td_fill_json_buffer(buffer, "}", 1);

    return buffer;
}

char * _serialize_action_aff_array(char *buffer, uint32_t max_length, wot_td_action_affordance_t *action_aff, char *lang){
    wot_td_action_affordance_t *tmp = action_aff;
    char action_aff_obj_key[] = "actions";
    char input_obj_key[] = "input";
    char output_obj_key[] = "output";
    char safe_obj_key[] = "safe";
    char idempotent_obj_key[] = "idempotent";

    buffer = _wot_td_fill_json_obj_key(buffer, action_aff_obj_key, sizeof(action_aff_obj_key)-1);
    buffer = _wot_td_fill_json_buffer(buffer, "{", 1);
    while (tmp != NULL){
        buffer = _wot_td_fill_json_obj_key(buffer, tmp->key, strlen(tmp->key));
        buffer = _wot_td_fill_json_buffer(buffer, "{", 1);
        buffer = _wot_td_fill_json_obj_key(buffer, input_obj_key, sizeof(input_obj_key)-1);
        buffer = _serialize_data_schema(buffer, max_length, tmp->input, lang);
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        buffer = _wot_td_fill_json_obj_key(buffer, output_obj_key, sizeof(output_obj_key)-1);
        buffer = _serialize_data_schema(buffer, max_length, tmp->output, lang);
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        buffer = _wot_td_fill_json_obj_key(buffer, safe_obj_key, sizeof(safe_obj_key)-1);
        buffer = _wot_td_fill_json_bool(buffer, tmp->safe);
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        buffer = _wot_td_fill_json_obj_key(buffer, idempotent_obj_key, sizeof(idempotent_obj_key)-1);
        buffer = _wot_td_fill_json_bool(buffer, tmp->idempotent);
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        buffer = _serialize_int_aff(buffer, max_length, tmp->int_affordance);
        buffer = _wot_td_fill_json_buffer(buffer, "}", 1);
        tmp = tmp->next;
    }

    return _wot_td_fill_json_buffer(buffer, "}", 1);
}

char * _serialize_event_aff_array(char *buffer, uint32_t max_length, wot_td_event_affordance_t *event_aff, char *lang){
    wot_td_event_affordance_t *tmp = event_aff;
    char events_obj_key[] = "events";
    char subscription_obj_key[] = "subscription";
    char data_obj_key[] = "data";
    char cancellation_obj_key[] = "cancellation";
    bool has_previous_prop = false;

    buffer = _wot_td_fill_json_obj_key(buffer, events_obj_key, sizeof(events_obj_key)-1);
    buffer = _wot_td_fill_json_buffer(buffer, "{", 1);
    while (tmp != NULL){
        buffer = _wot_td_fill_json_obj_key(buffer, tmp->key, strlen(tmp->key));
        buffer = _wot_td_fill_json_buffer(buffer, "{", 1);
        if(tmp->subscription != NULL){
            has_previous_prop = true;
            buffer = _wot_td_fill_json_obj_key(buffer, subscription_obj_key, sizeof(subscription_obj_key)-1);
            buffer = _serialize_data_schema(buffer, max_length, tmp->subscription, lang);
        }
        if(tmp->data != NULL){
            buffer = _previous_prop_check(buffer, max_length, has_previous_prop);
            has_previous_prop = true;
            buffer = _wot_td_fill_json_obj_key(buffer, data_obj_key, sizeof(data_obj_key)-1);
            buffer = _serialize_data_schema(buffer, max_length, tmp->data, lang);
        }
        if(tmp->cancellation != NULL){
            buffer = _previous_prop_check(buffer, max_length, has_previous_prop);
            has_previous_prop = true;
            buffer = _wot_td_fill_json_obj_key(buffer, cancellation_obj_key, sizeof(cancellation_obj_key)-1);
            buffer = _serialize_data_schema(buffer, max_length, tmp->cancellation, lang);
        }
        buffer = _previous_prop_check(buffer, max_length, has_previous_prop);
        buffer = _serialize_int_aff(buffer, max_length, tmp->int_affordance);
        buffer = _wot_td_fill_json_buffer(buffer, "}", 1);
        if(tmp->next != NULL){
            buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        }
        tmp = tmp->next;
    }
    return _wot_td_fill_json_buffer(buffer, "}", 1);
}

char * _serialize_link_array(char *buffer, uint32_t max_length, wot_td_link_t *links){
    (void)max_length;
    wot_td_link_t *tmp = links;
    char href_obj_key[] = "href";
    char type_obj_key[] = "type";
    char rel_obj_key[] = "rel";
    char anchor_obj_key[] = "anchor";
    buffer = _wot_td_fill_json_buffer(buffer, "[", 1);

    while(tmp != NULL){
        _wot_td_fill_json_buffer(buffer, "{", 1);
        buffer = _wot_td_fill_json_obj_key(buffer, href_obj_key, sizeof(href_obj_key)-1);
        buffer = _wot_td_fill_json_uri(buffer, tmp->href);

        if(tmp->type != NULL){
            buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
            buffer = _wot_td_fill_json_obj_key(buffer, type_obj_key, sizeof(type_obj_key)-1);
            buffer = _wot_td_fill_json_string(buffer, tmp->type, strlen(tmp->type));
        }

        if(tmp->rel != NULL){
            buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
            buffer = _wot_td_fill_json_obj_key(buffer, rel_obj_key, sizeof(rel_obj_key)-1);
            buffer = _wot_td_fill_json_string(buffer, tmp->rel, strlen(tmp->rel));
        }

        if(tmp->anchor != NULL){
            buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
            buffer = _wot_td_fill_json_obj_key(buffer, anchor_obj_key, sizeof(anchor_obj_key)-1);
            buffer = _wot_td_fill_json_uri(buffer, tmp->anchor);
        }

        _wot_td_fill_json_buffer(buffer, "}", 1);
        if(tmp->next != NULL){
            buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        }
        tmp = tmp->next;
    }

    return _wot_td_fill_json_buffer(buffer, "]", 1);
}

int _wot_td_serialize_thing_json(char *buffer, uint32_t max_length, wot_td_thing_t *thing){
    bool has_previous_prop = false;
    buffer = _wot_td_fill_json_buffer(buffer, "{", 1);

    if(thing->context != NULL){
        has_previous_prop = true;
        char thing_context_key[] = "@context";
        buffer = _wot_td_fill_json_obj_key(buffer, thing_context_key, sizeof(thing_context_key)-1);
        buffer = _serialize_context_array(buffer, max_length-2, thing->context);
    }else{
        return 1;
    }

    if(thing->security != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        buffer = _serialize_security_array(buffer, max_length, thing->security, thing->default_language_tag);
    }else{
        return 1;
    }

    if(thing->type != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        buffer = _serialize_type_array(buffer, max_length, thing->type);
    }

    if(thing->id != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        char id_obj_key[] = "id";
        buffer = _wot_td_fill_json_obj_key(buffer, id_obj_key, sizeof(id_obj_key)-1);
        buffer = _wot_td_fill_json_uri(buffer, thing->id);
    }

    if(thing->titles != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        has_previous_prop = true;
        buffer = _serialize_title_array(buffer, max_length, thing->titles, thing->default_language_tag);
    }

    if(thing->descriptions != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        has_previous_prop = true;
        buffer = _serialize_description_array(buffer, max_length, thing->descriptions, thing->default_language_tag);
    }

    if(thing->properties != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        buffer = _serialize_prop_aff_array(buffer, max_length, thing->properties);
    }

    if(thing->actions != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        buffer = _serialize_action_aff_array(buffer, max_length, thing->actions, thing->default_language_tag);
    }

    if(thing->events != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        buffer = _serialize_event_aff_array(buffer, max_length, thing->events, thing->default_language_tag);
    }

    if(thing->links != NULL){
        buffer = _previous_prop_check(buffer, max_length, has_previous_prop);
        char links_obj_key[] = "links";
        buffer = _wot_td_fill_json_obj_key(buffer, links_obj_key, sizeof(links_obj_key)-1);
        buffer = _serialize_link_array(buffer, max_length, thing->links);
    }

    if(thing->base != NULL){
        buffer = _previous_prop_check(buffer, max_length, has_previous_prop);
        char base_obj_key[] = "base";
        buffer = _wot_td_fill_json_obj_key(buffer, base_obj_key, sizeof(base_obj_key)-1);
        buffer = _wot_td_fill_json_uri(buffer, thing->base);
    }

    if(thing->support != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        char support_obj_key[] = "support";
        buffer = _wot_td_fill_json_obj_key(buffer, support_obj_key, sizeof(support_obj_key) - 1);
        buffer = _wot_td_fill_json_uri(buffer, thing->support);
    }

    if(thing->version != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        char version_obj_key[] = "version";
        buffer = _wot_td_fill_json_obj_key(buffer, version_obj_key, sizeof(version_obj_key)-1);
        char instance_obj_key[] = "instance";
        buffer = _wot_td_fill_json_buffer(buffer, "{", 1);
        buffer = _wot_td_fill_json_obj_key(buffer, instance_obj_key, sizeof(instance_obj_key)-1);
        buffer = _wot_td_fill_json_string(buffer, thing->version->instance, strlen(thing->version->instance));
        buffer = _wot_td_fill_json_buffer(buffer, "}", 1);
    }

    if(thing->forms != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        char form_obj_key[] = "forms";
        buffer = _wot_td_fill_json_obj_key(buffer, form_obj_key, sizeof(form_obj_key)-1);
        buffer = _serialize_form_array(buffer, max_length, thing->forms);
    }

    if(thing->created != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        char created_obj_key[] = "created";
        buffer = _wot_td_fill_json_obj_key(buffer, created_obj_key, sizeof(created_obj_key)-1);
        buffer = _wot_td_fill_json_date(buffer, thing->created);
    }

    if(thing->modified != NULL){
        buffer = _wot_td_fill_json_buffer(buffer, ",", 1);
        char modified_obj_key[] = "modified";
        buffer = _wot_td_fill_json_obj_key(buffer, modified_obj_key, sizeof(modified_obj_key)-1);
        buffer = _wot_td_fill_json_date(buffer, thing->modified);
    }

    buffer = _wot_td_fill_json_buffer(buffer, "}", 1);
    buffer = _wot_td_fill_json_buffer(buffer, "\0", 1);

    return 0;
}

int wot_td_serialize_thing(char *buffer, uint32_t max_length, wot_td_thing_t *thing, wot_td_serialize_type_t type){
    if (type == WOT_TD_SERIALIZE_JSON){
        return _wot_td_serialize_thing_json(buffer, max_length, thing);
    }

    return 0;
}
