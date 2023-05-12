`timescale 1ns / 1ps


module ab(input A, input B, input clk, output reg Q);
   initial Q = 0;
   always @ (posedge clk)  
      case ({B,A})  
         2'b00 :  Q <= Q;  
         2'b01 :  Q <= 0;  
         2'b10 :  Q <= 1;  
         2'b11 :  Q <= ~Q;  
      endcase
endmodule

module nor_gate_2(
    output x,
    input a,
    input b
    );

	assign x = ~(a|b);

endmodule

module nxor_gate_2(
    output x,
    input a,
    input b
    );

	assign x = ~(a^b);

endmodule

module xor_gate_2(
    output x,
    input a,
    input b
    );

	assign x = (a^b);

endmodule

module and_gate_2(
    output x,
    input a,
    input b
    );

	assign x = (a&b);

endmodule


module ic1337(// Inputs
              input I0,
              input I1,
              input I2,
              input clk,
              // Outputs
              output Q0,
              output Q1,
              output Z);
	 wire nor1_out;
	 wire and1_out;
	 wire nor2_out;
	 wire xnor2_out;
	 
   	 nor_gate_2 g1(nor1_out,I0,~I1);
	 and_gate_2 g2(and1_out,~I2,nor1_out);
	 nor_gate_2 g3(nor2_out,I2,~I1);
	 nxor_gate_2 g4(xnor2_out,I0,nor2_out);
	 ab g5(and1_out,I2,clk,Q0);
	 ab g6(~I2,xnor2_out,clk,Q1);
	 xor_gate_2 g7(Z,Q0,Q1);
endmodule
