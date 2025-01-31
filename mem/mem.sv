module mem #(
    parameter unsigned MEMSZ = 64,
    parameter string MEMFILE = "",
    parameter unsigned LOAD_MEMFILE = 0
) (
    input logic clk,
    input logic we,
    input logic [$clog2(MEMSZ)-1:0] addr,
    input logic [31:0] wdata,
    output logic [31:0] rdata
);
  logic [31:0] RAM[MEMSZ]  /*verilator public*/;

  initial begin
    if (LOAD_MEMFILE) begin
      $readmemh(MEMFILE, RAM);
    end
  end

  assign rdata = RAM[addr];

  always_ff @(posedge clk) begin
    if (we) RAM[addr] <= wdata;
  end
endmodule
