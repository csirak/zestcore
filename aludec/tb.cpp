#include "Valudec.h"
#include "Valudec_alu_pkg.h"
#include <cassert>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#define MAX_SIM_TIME 300
#define VERIF_START_TIME 7


typedef Valudec_alu_pkg::alu_op_e alu_op_e;

vluint64_t sim_time = 0;
vluint64_t posedge_cnt = 0;

int main(int argc, char **argv, char **env) {
  srand(time(NULL));
  Verilated::commandArgs(argc, argv);

  Valudec *dut = new Valudec;

  Verilated::traceEverOn(true);
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);
  m_trace->open("waveform.vcd");

  while (sim_time < MAX_SIM_TIME) {
    uint8_t f3 = rand() % 8;
    uint8_t f7 = rand() % 2;
    dut->funct3 = f3;
    dut->funct7b5 = f7;

    dut->eval();

    switch (f3) {
      case 0:
        assert(dut->aluop == f7? alu_op_e::SUB : alu_op_e::ADD);
        break;
      case 2:
        assert(dut->aluop == alu_op_e::SLT);
        break;
      case 6:
        assert(dut->aluop == alu_op_e::OR);
        break;
      case 7:
        assert(dut->aluop == alu_op_e::AND);
        break;
      default:
        assert(dut->aluop == alu_op_e::NOP);
        break;
    }

    m_trace->dump(sim_time);
    sim_time++;
  }

  m_trace->close();
  delete dut;
  exit(EXIT_SUCCESS);
}
