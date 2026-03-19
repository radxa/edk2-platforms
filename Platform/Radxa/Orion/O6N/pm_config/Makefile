CC:=gcc
CFLAGS:=-Wall -Wno-overflow

all: csupm_bin_config

.PHONY: csupm_bin_config
csupm_bin_config: csupm_bin_config.c $(wildcard *.h) clean
	$(CC) $(CFLAGS) -o $@ $(firstword $^)
	./$@

clean:
	rm -f csupm_bin_config csu_pm_config.bin

.PHONY: all clean
