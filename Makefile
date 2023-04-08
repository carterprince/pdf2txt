CC = gcc
CFLAGS = -Wall -O2 -fopenmp `pkg-config --cflags poppler-glib`
LDFLAGS = `pkg-config --libs poppler-glib`
TARGET = pdf2txt
PREFIX = /usr/local

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LDFLAGS)

install: $(TARGET)
	install -m 755 $(TARGET) $(PREFIX)/bin/$(TARGET)

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all install uninstall clean
