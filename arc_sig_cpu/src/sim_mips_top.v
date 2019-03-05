`timescale 1ns / 1ps

module sim_mips_top;

	// Inputs
	reg CLK_200M_P;
	reg CLK_200M_N;
	reg [7:0] SW;
	reg RSTN;
	reg [3:0] BTN_Y;

	// Outputs
	wire [4:0] BTN_X;
	wire SEGLED_CLK;
	wire SEGLED_DO;
	wire SEGLED_PEN;
	wire LED_PEN;
	wire LED_DO;
	wire LED_CLK;
	wire [3:0] VGA_B;
	wire [3:0] VGA_G;
	wire [3:0] VGA_R;
	wire HS;
	wire VS;

	// Instantiate the Unit Under Test (UUT)
	mips_top uut (
		.CLK_200M_P(CLK_200M_P), 
		.CLK_200M_N(CLK_200M_N), 
		.SW(SW), 
		.RSTN(RSTN), 
		.BTN_X(BTN_X), 
		.BTN_Y(BTN_Y), 
		.SEGLED_CLK(SEGLED_CLK), 
		.SEGLED_DO(SEGLED_DO), 
		.SEGLED_PEN(SEGLED_PEN), 
		.LED_PEN(LED_PEN), 
		.LED_DO(LED_DO), 
		.LED_CLK(LED_CLK), 
		.VGA_B(VGA_B), 
		.VGA_G(VGA_G), 
		.VGA_R(VGA_R), 
		.HS(HS), 
		.VS(VS)
	);

	initial begin
		// Initialize Inputs
		CLK_200M_P = 0;
		CLK_200M_N = 0;
		SW = 0;
		RSTN = 0;
		BTN_Y = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here

	end
      
endmodule

