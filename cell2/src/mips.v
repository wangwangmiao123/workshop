`include "define.vh"

module mips (
	`ifdef DEBUG
	input wire  debug_en,  // debug enable
	input wire  debug_step,  // debug step clock
	input wire  [6:0] debug_addr,  // debug address
	output wire [31:0] debug_data,  // debug data
	`endif
	output cpu_data_out, //data write to peripheral 
	input  peripheral_out, //data read from peripheral 
	output peripheral_in ,
	output addr, 
	output peripheral_we, 
	input wire clk,  // main clock
	input wire rst,  // synchronous reset
	input wire interrupter  // interrupt source, for future use
	);
	
	// instruction signals
	wire inst_ren;
	wire [31:0] inst_addr;
	wire [31:0] inst_data;
	
	// memory signals
	wire mem_ren, mem_wen;
	wire mem_re, mem_we;  //memory signals after filtering
	wire [31:0] mem_addr;
	wire [31:0] mem_data_r;
	wire [31:0] mem_data_w;

	assign addr = mem_addr; 
	assign peripheral_in = mem_data_w; 
	//
	wire [31:0] data2cpu = inst_addr[31:28]==4'hf || inst_addr[31:28]==4'he ? 
									peripheral_out : mem_data_r;
	
	// mips core
	mips_core MIPS_CORE (
		.clk(clk),
		.rst(rst),
		`ifdef DEBUG
		.debug_en(debug_en),
		.debug_step(debug_step),
		.debug_addr(debug_addr),
		.debug_data(debug_data),
		`endif
		.inst_ren(inst_ren),
		.inst_addr(inst_addr),
		.inst_data(inst_data),
		.mem_ren(mem_ren),
		.mem_wen(mem_wen),
		.mem_addr(mem_addr), // memeory target address 
		.mem_dout(mem_data_w), // data to memory 
		.mem_din(data2cpu) // data from memory or paripherial  
	);
	
	inst_rom INST_ROM (
		.clk(clk),
		.addr({2'b0, inst_addr[31:2]}),
		//.addr(inst_addr),
		.dout(inst_data)
		);
	
	data_ram DATA_RAM (
		.clk(clk),
		.we(mem_wen && inst_addr[31:28]!=4'hf),
		.addr({2'b0, mem_addr[31:2]}),
		//.addr(mem_addr),
		.din(mem_data_w),
		.dout(mem_data_r) // 
		);
	
endmodule
 