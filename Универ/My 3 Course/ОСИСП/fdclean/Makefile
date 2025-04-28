# Определение переменных
SRC_DIR = src
BUILD_DIR = build
CACHE_DIR = $(HOME)/.cache/fdclean
BIN_NAME = fdclean
BIN_PATH = $(BUILD_DIR)/$(BIN_NAME)
SYMLINK_PATH = /usr/local/bin/$(BIN_NAME)
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
ARCHIVE_NAME = $(BIN_NAME).tar.gz

# Цель по умолчанию
all: $(BIN_PATH)

# Сборка бинарника
$(BIN_PATH): $(SRC_FILES)
	@mkdir -p $(BUILD_DIR) $(CACHE_DIR)
	gcc -o $(BIN_PATH) $(SRC_FILES) -lssl -lcrypto \
		-DLOG_FILE=\"$(CACHE_DIR)/fdclean.log\" \
		-DLOCK_FILE=\"$(CACHE_DIR)/fdclean.lock\" \
		-DBIN_FILE=\"$(CACHE_DIR)/fdclean.bin\" \
		-DSCAN_DELAY_SEC=20

# Установка
install: 
	sudo cp $(BIN_PATH) $(SYMLINK_PATH)

# Удаление бинарника и символической ссылки
uninstall:
	sudo rm -f $(SYMLINK_PATH)
	
# Архивация
archive: 
	tar -czf $(ARCHIVE_NAME) $(SRC_DIR) Makefile

# Очистка
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(CACHE_DIR)

.PHONY: all clean install uninstall

