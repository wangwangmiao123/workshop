`timescale 1ns / 1ps

module led_dev_io(
		input  clk,	
		input  rst,
		input  GPIOf0000000_we,
		input  [31:0] peripheral_in,
		output reg [1:0] counter_set,	
		output wire[7:0] led_out, 
		output reg [21:0] GPIOf0  //no use 
	);
	
		reg [7:0] led = 0; 	
		assign led_out = led;
		always @(negedge clk or posedge rst) begin
				  if (rst) begin
							 led <= 8'h00;
							 counter_set <= 2'b00;
				  end 
				  else begin
							 if (GPIOf0000000_we)
										//{GPIOf0[21:0], led, counter_set} <= peripheral_in;
										{led, counter_set, GPIOf0[21:0]} <= peripheral_in;
							 else begin
										led <= led;
										counter_set <= counter_set;
							 end
				  end
		end
endmodule
