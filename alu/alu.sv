typedef enum logic [2:0] {
  ADD,
  SUB,
  AND,
  OR,
  SLT
} alu_op_e  /* verilator public */;

module alu #(
    parameter unsigned WIDTH = 32
) (
    input logic [WIDTH-1:0] a,
    input logic [WIDTH-1:0] b,
    input alu_op_e op,
    output logic [WIDTH-1:0] y
);

  always_comb begin
    case (op)
      ADD: y = a + b;
      SUB: y = a - b;
      AND: y = a & b;
      OR: y = a | b;
      SLT: y = {{(WIDTH - 1) {'0}}, a < b};
      default: y = '0;
    endcase
  end

endmodule

