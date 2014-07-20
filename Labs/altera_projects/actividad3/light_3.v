module light_3(SW, KEY, CLOCK_50, LEDG);
	input [7:0] SW;
	input [0:0]	KEY;
	input CLOCK_50;
	input [7:0] LEDG;
	
	nios_system NiosII (CLOCK_50, KEY[0], LEDG, SW);
endmodule
