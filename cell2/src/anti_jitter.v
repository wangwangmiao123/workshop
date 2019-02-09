`timescale 1ns / 1ps

module anti_jitter(
		wire clk ,  
		input wire [3:0] BTN_X , 
		input wire [7:0] SW, 
		output reg [3:0] BTN_X_OK , 
		output reg [7:0] SW_OK 
    );


	reg [31:0] counter = 0 ;
	always @(posedge clk)begin 
		if(counter <100000)
		begin counter<= counter + 1 ;
		end 
		else begin  
			SW_OK <= SW ; 
			rst <= SW[0] ; ////bang
		   button_out <= button ; 
			counter <= 0 ;
		end
	end

endmodule
