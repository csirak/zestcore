#include "Vmem.h"
#include "Vmem_mem.h"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#define MAX_SIM_TIME 300
#define VERIF_START_TIME 7

vluint64_t sim_time = 0;
vluint64_t posedge_cnt = 0;

typedef struct {
  bool we;
  uint8_t addr;
  uint32_t wdata;
} mem_tx_t;

uint32_t mem[64] = {0};
std::queue<mem_tx_t *> tx_queue;

void queue_random_tx(Vmem *dut) {
  mem_tx_t *tx = new mem_tx_t;
  tx->we = rand() % 2;
  tx->addr = rand() % 64;
  tx->wdata = rand();
  tx_queue.push(tx);

  dut->we = tx->we;
  dut->addr = tx->addr;
  dut->wdata = tx->wdata;
}

void valid_output(Vmem *dut) {
  if (tx_queue.empty()) {
    throw std::runtime_error("No transactions in queue");
  }
  mem_tx_t *tx = tx_queue.front();
  tx_queue.pop();

  if (dut->we) {
    mem[dut->addr] = dut->wdata;
  } else {
    assert(mem[dut->addr] == dut->rdata);
  }
  delete tx; // Free the memory after use
}

int main(int argc, char **argv, char **env) {
  srand(time(NULL));
  Verilated::commandArgs(argc, argv);

  Vmem *dut = new Vmem;

  Verilated::traceEverOn(true);
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);
  m_trace->open("waveform.vcd");

  for (int i = 0; i < 64; i++) {
    mem[i] = dut->mem->RAM[i];
  }

  while (sim_time < MAX_SIM_TIME) {
    // Pre eval
    dut->clk ^= 1;
    dut->eval();

    if (dut->clk) {
      queue_random_tx(dut);
    } else {
      valid_output(dut);
    }

    m_trace->dump(sim_time);
    sim_time++;
  }

  m_trace->close();
  delete dut;
  exit(EXIT_SUCCESS);
}
