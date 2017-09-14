ROOT= $(PWD)/gcc/
GCC= $(ROOT)/gcc-7.2.0/

all: test

g++:
	@ln -s $(ROOT)/install/bin/g++ g++

test: g++
	@./g++ -std=c++17 test.cpp

update_patches:
	@rm -f patch/* && cd $(GCC) && \
	git format-patch ec5ab5b3c6ec0c219be9f1160f5b601fcd10a703 && \
	cd - && mv $(GCC)/*.patch patch/;

gcc/:
	@mkdir gcc;

gcc/gcc-7.2.0/: gcc/
	@cd gcc && \
	wget -O gcc-7.2.0.tar.gz ftp://ftp.uvsq.fr/pub/gcc/releases/gcc-7.2.0/gcc-7.2.0.tar.gz && \
	gunzip gcc-7.2.0.tar.gz && \
	tar -xf gcc-7.2.0.tar && \
	rm -f gcc-7.2.0.tar;

gcc/build/: gcc/gcc-7.2.0/
	@mkdir -p gcc/build;
	@cd gcc/build && \
	$(GCC)/configure --prefix=$(ROOT)/install/ \
		--disable-bootstrap --disable-multilib --disable-shared --enable-languages=c++;

rebuild: gcc/build/
	@cd gcc/build && $(MAKE) && $(MAKE) install;
