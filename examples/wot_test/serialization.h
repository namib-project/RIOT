#ifndef WOT_TESTKIT_SERIALIZATION_H
#define WOT_TESTKIT_SERIALIZATION_H

typedef enum {
    WOT_TD_SERIALIZE_JSON,
    WOT_TD_SERIALIZE_CBOR
} wot_td_serialize_type_t;

int wot_td_serialize_thing(char *buffer, uint32_t max_length, wot_td_thing_t *thing, wot_td_serialize_type_t type);

#endif //WOT_TESTKIT_SERIALIZATION_H
