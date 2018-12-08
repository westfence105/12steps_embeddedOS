.PHONY: all write clean

all:
	cd boot; $(MAKE); cd -
	cd os;   $(MAKE); cd -

clean:
	cd boot; $(MAKE) clean; cd -
	cd os;   $(MAKE) clean; cd -

write:
	cd boot; $(MAKE) write; cd -
