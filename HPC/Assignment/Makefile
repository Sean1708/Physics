.PHONY: all WriteUp Code
all: WriteUp Code

WriteUp:
	$(MAKE) -C WriteUp
	$(CURDIR)/rsync-ignore-missing.sh WriteUp/Y0071170.pdf Y0071170/

Code:
	$(MAKE) -C Code
	$(CURDIR)/rsync-ignore-missing.sh Code/*.c Code/*.h Code/Makefile Y0071170/
