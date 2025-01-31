`timescale 1ns / 1ps

module regfile (
    input logic clk,
    input logic we3,
    input logic [4:0] r1,
    input logic [4:0] r2,
    input logic [4:0] r3,
    input logic [31:0] wd,
    output logic [31:0] rs1,
    output logic [31:0] rs2
);
  logic [31:0] regs[32]  /*verilator public*/;

  always_ff @(posedge clk) begin
    if (we3) begin
      regs[r3] <= wd;
    end
  end

  assign rs1 = r1 != 0 ? regs[r1] : 0;
  assign rs2 = r2 != 0 ? regs[r2] : 0;
endmodule
