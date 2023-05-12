`timescale 1ns / 1ps

module lab4_2(// INPUTS
              input wire      mode,
              input wire[2:0] opCode,
              input wire[3:0] value,
              input clk,
              input reset,
              // OUTPUTS
              output reg[9:0] result,
              output reg cacheFull,
              output reg invalidOp,
              output reg overflow);

    //================//
    // INITIAL BLOCK  //
    //================//
    //Modify the lines below to implement your design
    reg [2:0] opCodeList [0:31];
    reg [3:0] valueList [0:31];
    integer count;
    integer index;
    integer countCheck;
    reg [9:0] temp;
    reg [9:0] prev;


    initial begin
        cacheFull=0;
        invalidOp=0;
        overflow=0;

        count = 0;
        index = 0;
        countCheck=0;
        
    end

    //================//
    //      LOGIC     //
    //================//
    //Modify the lines below to implement your design
    always @(posedge clk or posedge reset)
    begin
        if(reset)
        begin
            cacheFull=0;
            invalidOp=0;
            overflow=0;
            count = 0;
            index = 0;
            countCheck=0;
            result=0;
            prev=0;
            
        end
        else if(mode == 1'b0)
        begin
            if(count == 32) cacheFull = 1;    

            else if(opCode== 3'b111 || opCode == 3'b011) invalidOp=1;
                 
            else 
            begin
                invalidOp=0;
                opCodeList[count]=opCode;
                valueList[count] = value;
                count=count+1;
            end
                

            
        end

        else if(mode == 1'b1 && count!=0)
        begin
                invalidOp=1;
				overflow=0;
            temp = result;
            if(opCodeList[countCheck]==3'b000)
            begin
                if(result+valueList[countCheck]>1023) overflow=1;
                result = result+valueList[countCheck];
            end
            else if(opCodeList[countCheck]==3'b001)
            begin
                if(result+prev+valueList[countCheck]>1023) overflow=1;
                result = result+prev+valueList[countCheck];
            end
            else if(opCodeList[countCheck]==3'b010)
            begin
                if(result+valueList[countCheck]>1023) overflow=1;
                result = result+valueList[countCheck];
            end
            else if(opCodeList[countCheck]==3'b100)
            begin
                result = result[0]+result[1]+result[2]+result[3]+result[4]+result[5]+result[6]+result[7]+result[8]+result[9];
            end
            else if(opCodeList[countCheck]==3'b101)
            begin
                result = {result[0],result[1],result[2],result[3],result[4],result[5],result[6],result[7],result[8],result[9]};
            end
            else if(opCodeList[countCheck]==3'b110)
            begin
                index = valueList[countCheck];
                temp=prev;
            end


            countCheck=countCheck+1;
            prev = temp;
            if(count == countCheck) countCheck=index;

        end

        

    end
endmodule
