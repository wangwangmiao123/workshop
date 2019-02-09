`include "define.vh"

module mips_top (
	input  wire 	CLK_200M_P,CLK_200M_N,
	input  wire 	[7:0] SW,
	input  wire 	RSTN,
	output wire 	[4:0] BTN_X,	//matrix button for output
	input  wire 	[3:0] BTN_Y,	//matrix button for input
	output wire SEGLED_CLK,
	output wire SEGLED_DO, 
	output wire SEGLED_PEN,
	output wire LED_PEN, 
	output wire LED_DO, 
	output wire LED_CLK,
	output wire [3:0]VGA_B,
	output wire [3:0]VGA_G,
	output wire [3:0]VGA_R,
	output wire HS, VS
	);
	
	// clock generator
	wire clk_cpu, clk_disp;
	wire locked;
	wire clk200MHz;
	wire clk0, clk1 ;
	
	//
	wire [31:0] peripheral_in ;
	reg [31:0] peripheral_out; 

	// anti jitter 
	wire [7:0] SW_OK ;
	wire [3:0] BTN_X_OK ; 

	//button control  
	wire [19:0] btn;
	wire [3:0] btn_out ; 
	wire btn_reset, btn_step;
	wire btn_interrupt;
	wire disp_prev, disp_next;
	
	//LED control signal 
	wire [7:0] led_out ; //led_dev_io to display 	
	
	//Counter_x 
	wire [1:0] counter_set ; //set counter channel 
	reg counter_we ;
	wire counter0_OUT,counter1OUT,counter2_OUT;  
	wire [31:0] counter_out; 

	//bus control begin 
	wire [31:0] addr ; 	 
	reg  GPIOf0000000_we ;  
	reg  GPIOe0000000_we ;  
	reg  GPIOe0000000_re ; 
	
	//seven segment data io 
	wire [63:0] seg_data ;
	wire [31:0] seg_data_store ; 

	//
	clk_diff CLK_DIFF (
		.clk200P(CLK_200M_P),
		.clk200N(CLK_200M_N),
		.clk200MHz(clk200MHz)
	);
	my_clk_gen CLK_GEN (
		.clkin1(clk200MHz),
		.CLK_OUT1(clk0), //100MHz
		.CLK_OUT2(clk1), //50MHz
		.CLK_OUT3(clk_disp), //25MHz
		.CLK_OUT4(clk_cpu) //10MHz
		);
	


	// button scan 
	btn_scan #(
		.CLK_FREQ(25)
		) BTN_SCAN (
		.clk(clk_disp), //25MHz
		.rst(1'b0),	// never reset 
		.btn_x(BTN_X_OK),
		.btn_y(BTN_Y),
		.result(btn),
		.btn_out(btn_out)
	);
	
	assign
		btn_step = btn[16],
		btn_interrupt = btn[19];
	
	`ifndef SIMULATING
	`else
	assign
	`endif
	
	// reset
	reg rst_all;
	reg [15:0] rst_count = 16'hFFFF;
	always @(posedge clk_cpu) begin
		rst_all <= (rst_count != 0);
		rst_count <= {rst_count[14:0], (!RSTN)};
	end

	wire [31:0] disp_data;
	display DISPLAY (
		.clk(clk_disp), //2.5 MHz 
		.rst(rst_all), 
		.en(8'hff),
		.data(seg_data),
		.dot(8'b00000000), 
		.led( {8'h00, led_out} ), 
		.led_clk(LED_CLK), // output 
		.led_en(LED_PEN),
		.led_do(LED_DO),
		.seg_clk(SEGLED_CLK),
		.seg_en(SEGLED_PEN),
		.seg_do(SEGLED_DO)
	);
	
	//counter 
	Counter_x U10 (
		.clk(clk_IO),
		.rst(rst_all),
		.clk0(clk0),
		.clk1(clk1),
		.clk2(clk1),
		.counter_we(counter_we),
		.counter_val(peripheral_in[31:0]),
		.counter_ch(counter_set[1:0]),
		.counter0_OUT(counter0_OUT),
		.counter1_OUT(counter1_OUT),
		.counter2_OUT(counter2_OUT),
		.counter_out(counter_out[31:0])
	);

	//
	seven_seg_dev_io SEVEN_SEG_DEV_IO(
		.clk(clk_CPU),
		.rst(rst_all), 
		.data_input(peripheral_in), 
		.sw(SW_OK[4:3]), 
		.data(seg_data), 
		.data_store(seg_data_store),//ouptut 
		.GPIOe0000000_re( GPIOeO0000000_re), 
		.GPIOe0000000_we( GPIOe0000000_we)
	); 	
	
	led_dev_io LED_DEV_IO (
		.clk(clk_IO),	
		.rst(rst_all),
		.GPIOf0000000_we(GPIOf0000000_we),	
		.peripheral_in(peripheral_in),
		//output 
		.counter_set(counter_set),	
		.led_out(led_out), 
		.GPIOf0()  
	); 	

	always @(*) begin	
		GPIOe0000000_we  =0 ;
		GPIOf0000000_we  =0 ;
		GPIOe0000000_re  =0 ;
			
		case (addr[31:28])
			4'he: begin	// 7 segments LEDs
				GPIOe0000000_we = peripheral_we;
				peripheral_out = seg_data_store; 
			end
			4'hf: begin 
				if (addr[2]) begin // counter port 
					counter_we = peripheral_we; 
					peripheral_out = counter_out; 
					 
				end else begin //GPIO 
					GPIOf0000000_we = peripheral_we;
					peripheral_out = {counter0_OUT, counter1_OUT, counter2_OUT, 9'h00, led_out, btn_out[3:0], SW_OK[7:0]};
				end
			end
			default : begin 
				// error 
			end 
		endcase
	end
	
	// BUS CONTROL end 
	wire [6:0]  debug_addr;
	wire [31:0] debug_data;
	
	mips MIPS (
		`ifdef DEBUG
		.debug_en(SW_OK[0]),
		.debug_step(btn_step),
		.debug_addr(debug_addr),
		.debug_data(debug_data),
		`endif
	
		.cpu_data_out(peripheral_in), 
		.peripheral_out(peripheral_out), //data read from peripheral 
		.addr(addr),  
		.peripheral_we(peripheral_we), 	
		.clk(clk_cpu),
		.rst(rst_all),
		.interrupter(btn_interrupt)
		);

	wire [9:0] vga_h_count;
	wire [9:0] vga_v_count;
	wire vga_rdn;
	wire [11:0] vga_data;
		
	vga VGA(
		.clk(clk_disp),					// vga clk = 25MHz
		.rst(0),  // never reset
		.Din(vga_data),	
		.h_count(vga_h_count),
		.v_count(vga_v_count),
		.rdn(vga_rdn),
		.R(VGA_R),
		.G(VGA_G),
		.B(VGA_B),
		.HS(HS),
		.VS(VS)
		); 
	
	vga_debug VGA_DEBUG(
	.clk(clk_disp), //25MHz 
	.debug_data(debug_data),
	.h_count(vga_h_count),
	.v_count(vga_v_count),
	.debug_addr(debug_addr), //output 
	.dout(vga_data)
	);

endmodule
