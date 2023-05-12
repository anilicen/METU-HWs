`timescale 1ns / 1ps
module lab4ROM (input [3:0] romAddr, output reg[4:0] romOutput);

reg [4:0] memory [0:15];


initial begin
	memory[4'b1000]= 5'b11110;
    memory[4'b1001]= 5'b11111;
    memory[4'b1011]= 5'b10000;
    memory[4'b1100]= 5'b10111;
    memory[4'b1101]= 5'b11000;
    memory[4'b1110]= 5'b11001;
    memory[4'b1111]= 5'b11010;
  	memory[4'b0000]= 5'b00000;
  	memory[4'b0010]= 5'b00110;
    memory[4'b0011]= 5'b00111;
    memory[4'b0001]= 5'b01011;
    memory[4'b0101]= 5'b00001;
    memory[4'b0110]= 5'b01101;
    memory[4'b0111]= 5'b11101;

end
always@(*)begin
romOutput = memory[romAddr];
end


endmodule
																							
module lab4RAM (
	input ramMode, //0:read, 1:write
	input [3:0] ramAddr, 
	input [4:0] ramInput,
	input  ramOp, //0:polynomial, 1:derivative
	input [1:0] ramArg,  //00:+1, 01:+2, 10:-1, 11:-2
	input CLK, 
	output reg [8:0] ramOutput);

	integer number = 0;
  reg [7:0] bits = 0;
	integer i =0;
		reg [8:0]storage [0:15];
	always@(*)

	begin
	if(ramMode==1'b0) ramOutput= storage[ramAddr];
	end
	
	always@(posedge CLK)
	begin
		if(ramMode == 1'b1)
		begin
			if(ramArg == 2'b00) number =1;
			else if(ramArg== 2'b01) number = 2;
			else if(ramArg== 2'b10) number = -1;
			else if(ramArg== 2'b11) number = -2;


			if(ramOp==0)
			begin
			for(i = 0; i<5;i=i+1)
				begin
				if(ramInput[i]==0) bits = bits+number**i;
				else bits = bits-number **i;
				end

			end
			else if (ramOp==1)
			begin
              for(i=3;i>-1;i=i-1)
				begin
                  if(ramInput[i+1]==0) bits = bits+((i+1)*number**i);
					else  bits = bits-((i+1)*number**i);

				end


			end	
          if(bits[7]==1'b0) storage[ramAddr]={1'b0,bits};
          else storage[ramAddr]={1'b1,-bits};
		end
	end



endmodule


module polMEM(input mode, input [3:0] memAddr, input op, input [1:0] arg, input CLK, output wire [8:0] memOutput);

	/*Don't edit this module*/
	wire [4:0]  romOutput;

	lab4ROM RO(memAddr, romOutput);
	lab4RAM RA(mode, memAddr, romOutput, op, arg, CLK, memOutput);



endmodule
