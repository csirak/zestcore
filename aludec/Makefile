MODULE=aludec

SOURCES = ../alu/alu.sv $(MODULE).sv 

.PHONY:sim
sim: waveform.vcd

.PHONY:verilate
verilate: .stamp.verilate

.PHONY:build
build: obj_dir/Valudec

.PHONY:waves waveform.vcd
waves: waveform.vcd
	@echo
	@echo "### WAVES ###"
	gtkwave waveform.vcd

waveform.vcd: ./obj_dir/V$(MODULE)
	@echo
	@echo "### SIMULATING ###"
	@./obj_dir/V$(MODULE) +verilator+rand+reset+2

./obj_dir/V$(MODULE): .stamp.verilate
	@echo
	@echo "### BUILDING SIM ###"
	make -C obj_dir -f V$(MODULE).mk V$(MODULE)

.stamp.verilate: $(SOURCES) tb.cpp
	@echo
	@echo "### VERILATING ###"
	@verilator  -Wall --trace --x-assign unique --x-initial unique -cc $(SOURCES) --exe tb.cpp --top-module $(MODULE)
	@touch .stamp.verilate

.PHONY:lint
lint: $(SOURCES)
	verilator --lint-only $(SOURCES)

.PHONY: clean
clean:
	rm -rf .stamp.*;
	rm -rf ./obj_dir
	rm -rf waveform.vcd

