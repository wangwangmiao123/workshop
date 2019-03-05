module seven_seg_dev_io(
	input clk, 
	input rst, 
	input [1:0] sw , 
	input [31 : 0 ] data_input, 
	input GPIOe0000000_we , 
	input GPIOe0000000_re , 
	output reg [63:0] data,
	output reg [31:0] data_store 
); 

function [6:0] digit2seg;
	input [3:0] number;
	begin
		case (number)
			4'h0: digit2seg = 7'b0111111;
			4'h1: digit2seg = 7'b0000110;
			4'h2: digit2seg = 7'b1011011;
			4'h3: digit2seg = 7'b1001111;
			4'h4: digit2seg = 7'b1100110;
			4'h5: digit2seg = 7'b1101101;
			4'h6: digit2seg = 7'b1111101;
			4'h7: digit2seg = 7'b0000111;
			4'h8: digit2seg = 7'b1111111;
			4'h9: digit2seg = 7'b1101111;
			4'hA: digit2seg = 7'b1110111;
			4'hB: digit2seg = 7'b1111100;
			4'hC: digit2seg = 7'b0111001;
			4'hD: digit2seg = 7'b1011110;
			4'hE: digit2seg = 7'b1111001;
			4'hF: digit2seg = 7'b1110001;
		endcase
	end
endfunction

wire [63:0] temp ;
assign temp = data ;     

always @(*) begin
	if(rst) begin 
		data_store = 0;
	end 
	else begin 
		if( GPIOe0000000_we )
		begin 
			data_store =  data_input;
		end 
		else data_store = data_store ;
		
		case (sw[1:0]) 
		2'b01 : //digit mode 
			begin 
					data = { 1'b0, digit2seg(data_store[3:0]),
								1'b0, digit2seg(data_store[7:4]),
								1'b0, digit2seg(data_store[11:8]),
								1'b0, digit2seg(data_store[15:12]),
								1'b0, digit2seg(data_store[19:16]),
								1'b0, digit2seg(data_store[23:20]),
								1'b0, digit2seg(data_store[27:24]),
								1'b0, digit2seg(data_store[31:28])
							};
			end
		default : // image mode 
			begin 
				data = {data_store, 32'h00000000};
			end
		endcase
		
		if(sw == 2'b00 && clk == 1'b1)
		begin 
			data = {temp[55:0] , temp[63:56]} ;
			data_store = data ; 
		end

	end 
end

			
endmodule 