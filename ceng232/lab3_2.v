`timescale 1ns / 1ps 
module lab3_2(
			input[4:0] smartCode,
			input CLK, 
			input lab, //0:Digital, 1:Mera
			input [1:0] mode, //00:exit, 01:enter, 1x: idle 
			output reg [5:0] numOfStuInMera,
			output reg [5:0] numOfStuInDigital,
			output reg restrictionWarnMera,//1:show warning, 0:do not show warning
			output reg isFullMera, //1:full, 0:not full
			output reg isEmptyMera, //1: empty, 0:not empty
			output reg unlockMera,	//1:door is open, 0:closed
			output reg restrictionWarnDigital,//1:show warning, 0:do not show warning
			output reg isFullDigital, //1:full, 0:not full
			output reg isEmptyDigital, //1: empty, 0:not empty
			output reg unlockDigital //1:door is open, 0:closed
	);
	 
	// You may declare your variables below	
	
	initial begin
			numOfStuInMera=0;
			numOfStuInDigital=0;
			restrictionWarnMera=0;
			isFullMera=0;
			isEmptyMera=1'b1;
			unlockMera=0;		
			restrictionWarnDigital=0;
			isFullDigital=0;
			isEmptyDigital=1'b1;
			unlockDigital=0;
	end
	assign sum = smartCode[0]+smartCode[1]+smartCode[2]+smartCode[3]+smartCode[4];
	assign isOdd = sum%2;
	always @(posedge CLK)
	begin
		
		restrictionWarnDigital=0;
		restrictionWarnMera=0;
		unlockMera=0;
		unlockDigital=0;
			if(mode == 2'b10 ||mode == 2'b11)
			begin
				
			end
			else if(mode == 2'b00)
			begin
			if(lab == 0 && isEmptyDigital==0)
				begin
				numOfStuInDigital=numOfStuInDigital-1;
				unlockDigital=1;
				end
			else if(lab == 1 && isEmptyMera==0)
				begin
				numOfStuInMera=numOfStuInMera-1;
				unlockMera=1;
				
				end
			end
			else 
			begin
			if(lab == 0 && isFullDigital==0)
				begin
				if(numOfStuInDigital<15)
					begin
					unlockDigital=1;
					numOfStuInDigital=numOfStuInDigital+1;
					
					end	
				else 
					begin
					if(isOdd==1)
						begin
						unlockDigital=1;
						numOfStuInDigital=numOfStuInDigital+1;
						end
					else
						begin
						restrictionWarnDigital=1;
						end
					end
					

				
				end
			if(lab==1 && isFullMera==0)
				begin
				if(numOfStuInMera<15)
					begin
					unlockMera=1;
					numOfStuInMera=numOfStuInMera+1;
					end
				else
					begin
					if(isOdd==0)
						begin
							unlockMera=1;
							numOfStuInMera=numOfStuInMera+1;
						end
					else
						begin
						restrictionWarnMera=1;
						end
					end
					


				
				
				end

			end

	if(numOfStuInMera==30) isFullMera=1;
	else isFullMera=0;
	if(numOfStuInDigital==30) isFullDigital=1;
	else isFullDigital = 0;
	if(numOfStuInDigital==0) isEmptyDigital = 1;
	else isEmptyDigital=0;
	if(numOfStuInMera==0) isEmptyMera = 1;
	else isEmptyMera = 0;
			



	end 
	
	
endmodule


