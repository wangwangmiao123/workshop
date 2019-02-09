`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:01:12 10/15/2017 
// Design Name: 
// Module Name:    clk_div 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module clk_div(
		input wire clk,
		input wire rst,
		output reg [31:0] clkdiv
	); 
	
	always @(posedge clk or posedge rst ) begin 
			if(rst)begin
				clkdiv <= 0; 
			end
			else begin 
				clkdiv <= clkdiv+1; 
			end
	end
endmodule 