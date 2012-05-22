BIN := sf2dump
SOURCES := Main.cpp Dump.cpp StructuralDump.cpp
SOURCES += RIFF.cpp SF2.cpp SF2Generator.cpp 
SOURCES += Indent.cpp
OBJECT_DIR := objects

-include Makefile.local

# ----

OBJECTS = $(SOURCES:%.cpp=$(OBJECT_DIR)/%.o)

# ----

all: $(BIN)

clean:
	rm -f $(BIN) $(OBJECTS)

$(OBJECT_DIR):
	mkdir -p $(OBJECT_DIR)

$(BIN): $(OBJECT_DIR) $(OBJECTS)
	$(CXX) $(LINK_FLAGS) -o $@ $(OBJECTS)

$(OBJECT_DIR)/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $^


