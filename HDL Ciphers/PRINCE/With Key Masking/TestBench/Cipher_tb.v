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

module Cipher_tb;

	// Inputs
	reg clk;
	reg rst;
	reg [63:0] input_s1;
	reg [63:0] input_s2;
	reg [127:0] Rand;
	reg [127:0] Key;
	reg [127:0] Key1;
	reg [127:0] Key2;
	reg enc_dec;

	// Outputs
	wire [63:0] output_s1;
	wire [63:0] output_s2;
	wire [63:0] out;
	wire done;
	integer i;

	// Instantiate the Unit Under Test (UUT)
	Cipher uut (
		.clk(clk), 
		.rst(rst), 
		.input_s1(input_s1), 
		.input_s2(input_s2), 
		.output_s1(output_s1), 
		.output_s2(output_s2), 
		.Key1(Key1), 
		.Key2(Key2), 
		.enc_dec(enc_dec), 
		.done(done)
	);
assign out = output_s1 ^ output_s2;
	initial begin
		// Initialize Inputs
		clk = 0;
		rst = 1;
		input_s1 = 64'hABCDEF0123456789;
		input_s2 = 0;
		Rand = {$random,$random,$random,$random};
		Key = 128'h2B7E151628AED2A6ABF7158809CF4F3C;
		Key1 = Key ^ Rand;
		Key2 = Rand;
		enc_dec = 0;
		
		#21;

        rst = 0;
		// Add stimulus here
		 
		 #400
		 
		rst = 1;
		input_s1 = 64'h7ad2824faf91a5b3;
		input_s2 = 0;
		enc_dec = 1;
		
		#21;
		
		rst = 0;
		
		
	end
      always #5 clk = ~clk;
endmodule

