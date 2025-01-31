/* verilator lint_off IMPORTSTAR */
import alu_pkg::*;

module aludec (
    input logic [14:12] funct3,
    input logic funct7b5,
    output alu_op_e aluop
);

  always_comb
    case (funct3)
      3'b000:  aluop = funct7b5 ? SUB : ADD;
      3'b010:  aluop = SLT;
      3'b110:  aluop = OR;
      3'b111:  aluop = AND;
      default: aluop = NOP;
    endcase
endmodule
