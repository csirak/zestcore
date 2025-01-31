#include "Vregfile.h"
#include "Vregfile_regfile.h"
#include <cstdio>
#include <cstdlib>
#include <verilated.h>
#include <verilated_vcd_c.h>

#define NUM_TESTS 100
#define NUM_REGS 32

uint32_t reg_values[NUM_REGS] = {0}; // Expected register values

vluint64_t sim_time = 0;

void clock_tick(Vregfile *dut, VerilatedVcdC *m_trace) {
  dut->clk ^= 1;
  dut->eval();
  m_trace->dump(sim_time++);
}

int main(int argc, char **argv) {
  srand(time(NULL));
  Vregfile *dut = new Vregfile;

  // Setup waveform dumping
  Verilated::traceEverOn(true);
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);
  m_trace->open("waveform.vcd");

  // Initialize
  dut->clk = 0;
  dut->we3 = 0;

  printf("Starting random testing...\n");
  int failures = 0;

  for (int i = 0; i < NUM_REGS; i++) {
    reg_values[i] = dut->regfile->regs[i];
  }

  for (int i = 0; i < NUM_TESTS; i++) {
    // Random write
    int wr_reg = rand() % NUM_REGS;
    uint32_t wr_data = rand();

    dut->we3 = 1;
    dut->r3 = wr_reg;
    dut->wd = wr_data;
    if (wr_reg)
      reg_values[wr_reg] = wr_data;

    clock_tick(dut, m_trace);
    clock_tick(dut, m_trace);
    dut->we3 = 0;

    // Random read
    int rd_reg1 = rand() % NUM_REGS;
    int rd_reg2 = rand() % NUM_REGS;
    dut->r1 = rd_reg1;
    dut->r2 = rd_reg2;
    dut->eval();
    m_trace->dump(sim_time);

    // Verify - R0 should always read as 0, other registers should match
    // expected values
    uint32_t expected_rs1 = reg_values[rd_reg1];
    uint32_t expected_rs2 = reg_values[rd_reg2];

    if (dut->rs1 != expected_rs1 || dut->rs2 != expected_rs2) {
      printf("Error at test %d:\n", i);
      printf("R%d: Expected: %08x, Got: %08x\n", rd_reg1, expected_rs1,
             dut->rs1);
      printf("R%d: Expected: %08x, Got: %08x\n", rd_reg2, expected_rs2,
             dut->rs2);
      failures++;
    }
  }

  printf("Testing complete: %d failures\n", failures);
  m_trace->close();
  delete dut;
  return failures ? EXIT_FAILURE : EXIT_SUCCESS;
}
