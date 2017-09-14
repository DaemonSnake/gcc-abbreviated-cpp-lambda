RED=\033[0;31m
NC=\033[0m
ROOT=$(PWD)/gcc/
GCC=$(ROOT)/gcc-7.2.0/
NPROC=$(nproc --all)

all: rebuild test

g++:
	@ln -s $(ROOT)/install/bin/g++ g++

test: g++
	@./g++ -std=c++17 test.cpp && echo "$(RED)compilation successfull!$(NC)";

update_patches:
	@rm -f patch/* && cd $(GCC) && \
	git format-patch ec5ab5b3c6ec0c219be9f1160f5b601fcd10a703 && \
	cd - && mv $(GCC)/*.patch patch/;

gcc:
	@echo "$(RED)creating gcc/...$(NC)";
	@mkdir gcc;

gcc/gcc-7.2.0: gcc/
	@echo "$(RED)downloading gcc-7.2.0...$(NC)";
	@cd gcc && \
	wget -O gcc-7.2.0.tar.gz ftp://ftp.uvsq.fr/pub/gcc/releases/gcc-7.2.0/gcc-7.2.0.tar.gz && \
	gunzip gcc-7.2.0.tar.gz && \
	tar -xf gcc-7.2.0.tar && \
	rm -f gcc-7.2.0.tar;

gcc/build: gcc/gcc-7.2.0/
	@echo "$(RED)configuring gcc...$(NC)";
	@mkdir -p gcc/build;
	@cd gcc/build && \
	$(GCC)/configure --prefix=$(ROOT)/install/ \
		--disable-bootstrap --disable-multilib --disable-shared --enable-languages=c++;

rebuild: apply_patches
	@echo "$(RED)building gcc...$(NC)";
	@cd gcc/build && $(MAKE) -j$(NPROC) && $(MAKE) install -j$(NPROC);

apply_patches: gcc/build/
	@echo "$(RED)applying patches...$(NC)";
	@for patch in $(ls $(PWD)/patch/*.patch); \
	do patch -d $(GCC) -p1 < $(PWD)/patch/$(patch); \
	done;
