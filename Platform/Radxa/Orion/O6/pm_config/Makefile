CC:=gcc
CFLAGS:=-Wall -Wno-overflow

all: clean csupm_bin_config

csupm_bin_config: csupm_bin_config.c $(wildcard *.h)
	$(CC) $(CFLAGS) -o $@ $(firstword $^)
	./$@

clean:
	rm -f csupm_bin_config csu_pm_config.bin

.PHONY: all clean