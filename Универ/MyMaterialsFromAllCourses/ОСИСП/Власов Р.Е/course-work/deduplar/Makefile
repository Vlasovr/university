# ---------- настраиваемые переменные ----------
CC          ?= gcc
PKG_CONFIG  ?= pkg-config   # можно переопределить: PKG_CONFIG=pkgconf

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
BUILD_DIR   ?= build
SRC         := src/deduplar.c
BIN         := $(BUILD_DIR)/deduplar
SCAN_INTERVAL ?= 20                  # секунд

# Куда класть логи/lock/журнал при рантайме
CACHE_DIR   ?= $(HOME)/.cache/deduplar

# ---------- флаги сборки ----------
OPENSSL_CFLAGS  := $(shell $(PKG_CONFIG) --cflags openssl)
OPENSSL_LDFLAGS := $(shell $(PKG_CONFIG) --libs   openssl)

CFLAGS   ?= -O2 -std=c11 -pedantic -Wall -Wextra -pthread \
            $(OPENSSL_CFLAGS)

LDFLAGS  ?= $(OPENSSL_LDFLAGS) -pthread

# ---------- цели ----------
all: $(BIN)

$(BIN): $(SRC)
	@mkdir -p $(dir $@) $(CACHE_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS) \
		-DLOG_FILE=\"$(CACHE_DIR)/deduplar.log\" \
		-DLOCK_FILE=\"$(CACHE_DIR)/deduplar.lock\" \
		-DBIN_FILE=\"$(CACHE_DIR)/deduplar.bin\" \
		-DSCAN_INTERVAL=$(SCAN_INTERVAL)

install: $(BIN)
	install -d $(DESTDIR)$(BINDIR)
	install -m755 $(BIN) $(DESTDIR)$(BINDIR)/deduplar

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/deduplar

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all install uninstall clean
