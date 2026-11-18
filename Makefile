CC = gcc
CFLAGS = -g -O0 -Wall -Werror
LDLIBS = -lssl -lcrypto -lcurl

SOURCES = client.c smtp.c tcp.c tls.c
OBJECTS = $(SOURCES:.c=.o)
DEPS = $(OBJECTS:.o=.d)

.PHONY: all clean

all: client

client: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LDLIBS)
	$(RM) $(OBJECTS) $(DEPS)

# Generate header dependencies while building the objects.
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	$(RM) client $(OBJECTS) $(DEPS)
