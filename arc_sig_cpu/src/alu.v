`include "define.vh"

module alu (
	input wire [31:0] a, b,  // two operands
	input wire [3:0]  oper,   // operation type
	output reg [31:0] result,  // calculation result
	output reg overflow //overflow 
	); 
	`include "mips_define.vh"	
	always @(*) begin
		result = 0;
		case (oper)
			EXE_ALU_ADD: begin
				result = a + b;
			end
			EXE_ALU_SUB: begin
				result = a - b;
			end
			EXE_ALU_SLT: begin
				result = $signed(a) < $signed(b);
			end
			EXE_ALU_AND: begin
				result = a & b;
			end
			EXE_ALU_OR: begin
				result = a | b; 
			end
			EXE_ALU_XOR: begin
				result = (~a&b) | (a& ~b) ; 
			end
			EXE_ALU_SR: begin
				result = a >> b; 
			end
			EXE_ALU_SL: begin
				result = a << b; 
			end
			EXE_ALU_NOR: begin
				result = ~a & ~b ; 
			end
		endcase
		case (oper)
			EXE_ALU_ADD :
			begin
				if(a>0 && b>0 && result < a ) overflow  = 1; 
				if(a<0 && b<0 && result > a ) overflow  = 1; 
				else result = 0;
			end
			EXE_ALU_SUB : 
			begin 
				if(a>0 && b<0 && result < a ) overflow  = 1; 
				if(a<0 && b>0 && result > a ) overflow  = 1; 
				else result = 0;
			end
			default :
			begin
				result = 0; 
			end
		endcase 

	end
endmodule
