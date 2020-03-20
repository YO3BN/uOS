# tool marcros

ARCH_ID := atmega1284
ARCH_CPU := avr8
ARCH_CC_FLAGS := -mmcu=$(ARCH_ID)
ARCH_INCLUDE := arch/$(ARCH_CPU)/$(ARCH_ID)

CC := avr-gcc  $(ARCH_CC_FLAGS)
AR := avr-ar
CCFLAG := -Os
DBGFLAG := -g3 -Og
CCOBJFLAG := $(CCFLAG) -I$(ARCH_INCLUDE) -c

# path marcros
BIN_PATH := ../bin
OBJ_PATH := ../obj
SRC_PATH := kernel
INC_PATH := $(SRC_PATH)/include
DBG_PATH := ./

# compile marcros
TARGET_NAME := uOS.a
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
DISTCLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG)
CLEAN_LIST := $(TARGET) \
			  $(TARGET_DEBUG) \
			  $(DISTCLEAN_LIST)

# default rule
default: all

# non-phony targets
$(TARGET): $(OBJ)
#	$(CC) $(CCFLAG) -o $@ $?
	$(AR) rcs $(BIN_PATH)/$(TARGET_NAME) $(OBJ_PATH)/*.o

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAG) -I$(INC_PATH) -o $@ $<

$(DBG_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAG) $(DBGFLAG) -I$(INC_PATH) -o $@ $<

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CC) $(CCFLAG) $(DBGFLAG) $? -o $@

# phony rules
.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)