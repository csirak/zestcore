#include "Valu.h"
#include "Valu_alu_pkg.h"
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#define MAX_SIM_TIME 1000

vluint64_t sim_time = 0;
vluint64_t posedge_cnt = 0;

typedef struct {
  uint32_t a;
  uint32_t b;
  Valu_alu_pkg::alu_op_e op;
} alutx_t;

void rand_alutx(alutx_t &alutx) {
  alutx.a = rand();
  alutx.b = rand();
  alutx.op = (Valu_alu_pkg::alu_op_e)(rand() % 8);
}

void drive_alutx(Valu *dut, alutx_t &alutx) {
  dut->a = alutx.a;
  dut->b = alutx.b;
  dut->op = alutx.op;
}

bool valid_alutx(alutx_t &alutx, uint32_t y) {
  // printf("ALU op: %d, a: %u, b: %u, y: %u\n", alutx.op, alutx.a, alutx.b, y);
  switch (alutx.op) {
  case Valu_alu_pkg::ADD:
    return y == alutx.a + alutx.b;
  case Valu_alu_pkg::SUB:
    return y == alutx.a - alutx.b;
  case Valu_alu_pkg::AND:
    return y == (alutx.a & alutx.b);
  case Valu_alu_pkg::OR:
    return y == (alutx.a | alutx.b);
  case Valu_alu_pkg::SLT:
    return y == (alutx.a < alutx.b);
  default:
    return y == 0;
  }
}

int main(int argc, char **argv, char **env) {
  srand(time(NULL));
  Verilated::commandArgs(argc, argv);

  Valu *dut = new Valu;

  Verilated::traceEverOn(true);
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);
  m_trace->open("waveform.vcd");

  while (sim_time < MAX_SIM_TIME) {
    // Pre eval
    alutx_t tx;
    rand_alutx(tx);
    drive_alutx(dut, tx);
    dut->eval();

    assert(valid_alutx(tx, dut->y));

    m_trace->dump(sim_time);
    sim_time++;
  }

  m_trace->close();
  delete dut;
  exit(EXIT_SUCCESS);
}
