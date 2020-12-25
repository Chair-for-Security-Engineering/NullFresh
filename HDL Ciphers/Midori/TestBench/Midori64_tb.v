`timescale 1ns / 1ps

/*
* -----------------------------------------------------------------
* COMPANY : Ruhr University Bochum
* AUTHOR  : Amir Moradi (amir.moradi@rub.de) Aein Rezaei Shahmirzadi (aein.rezaeishahmirzadi@rub.de)
* DOCUMENT: https://doi.org/10.46586/tches.v2021.i1.305-342
* -----------------------------------------------------------------
*
* Copyright (c) 2020, Amir Moradi, Aein Rezaei Shahmirzadi
*
* All rights reserved.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTERS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Please see LICENSE and README for license and further instructions.
*/

module Midori64_tb;

	// Inputs
	reg clk;
	reg reset;
	reg [63:0] input1;
	reg [63:0] input2;
	reg [127:0] key;
	reg enc_dec;

	// Outputs
	wire [63:0] output1;
	wire [63:0] output2;
	wire [63:0] Out;
	wire [63:0] In;
	wire done;

	assign In = input1 ^ input2 ;
	assign Out = output1 ^ output2 ;

	// Instantiate the Unit Under Test (UUT)
	Midori64 uut (
		.clk(clk), 
		.reset(reset), 
		.input1(input1), 
		.input2(input2),  
		.output1(output1), 
		.output2(output2), 
		.key(key), 
		.enc_dec(enc_dec), 
		.done(done)
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		reset = 1;
		input1 = 0;
		input2 = 0;
		key = 0;
		enc_dec = 0;

		// Wait 100 ns for global reset to finish
		#20;
		reset = 0;
		
		#500
		
		reset = 1;
		input1 = 64'h3c9cceda2bbd449a;
		input2 = 0;
		key = 0;
		enc_dec = 1;

		// Wait 100 ns for global reset to finish
		#20;
		reset = 0;
		
        
		// Add stimulus here

	end
      always #5 clk=~clk;
endmodule

