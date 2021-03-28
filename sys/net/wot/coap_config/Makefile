MODULE = wot_coap_config

WOT_TD_CONFIG = $(BINDIR)/$(MODULE)/coap_config.c
GENSRC += $(WOT_TD_CONFIG)
GENOBJC  := $(GENSRC:%.c=%.o)
SRCS += $(GENSRC)

include Makefile.gensrc
include $(RIOTMAKE)/tools/wot_td_coap.inc.mk
include $(RIOTBASE)/Makefile.base