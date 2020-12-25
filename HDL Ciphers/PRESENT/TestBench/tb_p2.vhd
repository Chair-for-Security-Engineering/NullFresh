--
-----------------------------------------------------------------
-- COMPANY : Ruhr University Bochum
-- AUTHOR  : Amir Moradi (amir.moradi@rub.de) Aein Rezaei Shahmirzadi (aein.rezaeishahmirzadi@rub.de)
-- DOCUMENT: https://doi.org/10.46586/tches.v2021.i1.305-342
-- -----------------------------------------------------------------
--
-- Copyright (c) 2020, Amir Moradi, Aein Rezaei Shahmirzadi
--
-- All rights reserved.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
-- ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
-- WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
-- DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTERS BE LIABLE FOR ANY
-- DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
-- (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
-- LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
-- ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
-- (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
-- SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--
-- Please see LICENSE and README for license and further instructions.
--

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;

-- file functions
USE ieee.std_logic_textio.all;
LIBRARY STD;
USE STD.TEXTIO.ALL;
 
ENTITY tb_p2 IS
END tb_p2;
 
ARCHITECTURE bench OF tb_p2 IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT Cipher
    Port ( clk      : in  STD_LOGIC;
           nReset   : in  STD_LOGIC;
		     dataIn1  : in  STD_LOGIC_VECTOR (3 downto 0);
           dataIn2  : in  STD_LOGIC_VECTOR (3 downto 0);

			  key      : in  STD_LOGIC_VECTOR (3 downto 0);
           dataOut1 : out STD_LOGIC_VECTOR (3 downto 0);
           dataOut2 : out STD_LOGIC_VECTOR (3 downto 0);

           done     : out STD_LOGIC);
    END COMPONENT;
 

   --Inputs
   signal clk    : std_logic := '0';
   signal nReset : std_logic := '0';
   signal dataIn1 : std_logic_vector(3 downto 0) := (others => '0');
   signal dataIn2 : std_logic_vector(3 downto 0) := (others => '0');

   signal key     : std_logic_vector(3 downto 0) := (others => '0');
   
	--Outputs
   signal dataOut1 : std_logic_vector(3 downto 0);
   signal dataOut2 : std_logic_vector(3 downto 0);

   signal done : std_logic;

   signal plaintext : std_logic_vector(3 downto 0)  := (others => '0');
   signal md1 : std_logic_vector(3 downto 0)  := (others => '0');
   signal md2 : std_logic_vector(3 downto 0)  := (others => '0');
   
   -- Clock period definitions
   constant clk_period : time := 10 ns;
 
 
BEGIN
 
  dataIn1 <= plaintext XOR md1;
  dataIn2 <= md1;

  
  -- Instantiate the Unit Under Test (UUT)
   uut: Cipher
	PORT MAP (
          	clk => clk,
          	nReset => nReset,
          	dataIn1 => dataIn1,
          	dataIn2 => dataIn2,

          	key => key,
				dataOut1 => dataOut1,
				dataOut2 => dataOut2,

          	done => done
        );

   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
	
	-- mixed (event-driven/cycle-based) result check
	check_results : process
	
		variable lineIn			: line;
		variable good_number		: boolean;
		variable space				: character;
		variable tv_key			: std_logic_vector( 79 downto 0);
		variable tv_plaintext	: std_logic_vector( 63 downto 0);
		variable tv_md1			: std_logic_vector( 63 downto 0);
		variable tv_md2			: std_logic_vector( 63 downto 0);
		variable tv_mk1			: std_logic_vector( 79 downto 0);--for dummy purposes due to tv-file format
		variable tv_mk2			: std_logic_vector( 79 downto 0);--for dummy purposes due to tv-file format
		variable tv_ciphertext	: std_logic_vector( 63 downto 0);
		variable out_temp		   : std_logic_vector( 63 downto 0);
		
		variable tv_plaintext_chunk  : std_logic_vector(3 downto 0);
		variable tv_md1_chunk		  : std_logic_vector(3 downto 0);
		variable tv_md2_chunk		  : std_logic_vector(3 downto 0);
		variable tv_key_chunk		  : std_logic_vector(3 downto 0);
		variable tv_ciphertext_chunk : std_logic_vector(3 downto 0);
		

-- these files only have 10 random test vectors
		file vector_file_SPS: text open read_mode is "..\tv_sPS_10.txt"; 
---------------------------------------------------------------------------------------------
--		reference to these files from the root directory of your modelsim library, 
--		e.g. c:\Modeltech_pe_edu_10.1\examples (just copy it there)
---------------------------------------------------------------------------------------------
		
		procedure assertMatch(testResult, correctResult : in std_logic_vector) is
			variable msg : line;
			variable msg2 : line;
		begin

			if (testResult'length mod 8 = 0) then
				hwrite(msg, testResult);
				write (msg, string'(" should be "));
				hwrite(msg, correctResult);
			else
				write(msg, testResult);
				write(msg, string'(" should be "));
				write(msg, correctResult);
			end if;

			write (msg2, string'("correct result"));

			assert (testResult = correctResult)  report msg.all severity error;
			assert (testResult /= correctResult) report msg2.all severity warning;

		end procedure assertMatch;

		

---------------------------------------------------------------------------------------------
-- XXXXXXXXXXXXXXXXX Start of CORE 2 XXXXXXXXXXXXXXXXX 
---------------------------------------------------------------------------------------------

		procedure testCipherCore2 (tv_key, tv_plain, tv_out : std_logic_vector) is

		begin
			
			report "---------- Test core 2: shared ----------";
			-- start cipher with new input
			nReset <= '0';
		   wait for 1*clk_period;
			
-- load 16 MSB of key
			FOR i in 0 to 3 loop
				tv_key_chunk := tv_key(79-i*4 downto 76-i*4);
				
				key <= tv_key_chunk;
				
				-- wait one cycle to load data
				wait for clk_period;
			end loop;

-- load plaintext, expected ciphertext, and remaining 63 bits of key
			FOR i in 0 to 15 loop
				tv_key_chunk := tv_key(63-i*4 downto 60-i*4);
				tv_plaintext_chunk := tv_plaintext(63-i*4 downto 60-i*4);
				tv_md1_chunk := tv_md1(63-i*4 downto 60-i*4);
				tv_md2_chunk := tv_md2(63-i*4 downto 60-i*4);
				tv_ciphertext_chunk := tv_ciphertext(63-i*4 downto 60-i*4);
				
				plaintext <= tv_plaintext_chunk;
				key <= tv_key_chunk;
				md1 <= tv_md1_chunk;
				md2 <= tv_md2_chunk;
			  
				-- wait one cycle to load data
				wait for clk_period;
			end loop;
				
			nReset <= '1';


			-- wait for cipher to finish
			wait until done = '1';
			wait for clk_period;
			
			FOR i in 0 to 15 loop
			out_temp(63-i*4 downto 60-i*4) := dataOut1 XOR dataOut2;
			-- wait one cycle to load data
			wait for clk_period;
			end loop;
			
			-- test result
			assertMatch(out_temp,tv_out);
			-- wait one cycle before next (possible) reset
			wait for clk_period;
			
			report "---------- core 2: shared successfully tested! ----------";
			
	
		end procedure testCipherCore2;
---------------------------------------------------------------------------------------------
-- XXXXXXXXXXXXXXXXX End of CORE 2 XXXXXXXXXXXXXXXXX 
---------------------------------------------------------------------------------------------


---------------------------------------------------------------------------------------------
-- XXXXXXXXXXXXXXXXX TESTING STARTS  XXXXXXXXXXXXXXXXX 
---------------------------------------------------------------------------------------------

	begin
		-- system init
		wait for 1.5*clk_period;

---------------------------------------------------------------------------------------------
-- XXXXXXXXXXXXXXXXX READ IN FROM FILE tv_sPS_10.txt XXXXXXXXXXXXXXXXX 
---------------------------------------------------------------------------------------------

report "---------- Test using testvector file tv_sPS_10.txt----------";
		read_loop_SPS: while not endfile(vector_file_SPS) loop
			readline(vector_file_SPS, lineIn);
			next when (lineIn(lineIn'left) = '-'); -- comment line
			
			-- read key
			hread(lineIn, tv_key, good => good_number);
			assert (good_number) report "Incorrect test vector format (key)" severity error;
			-- read space character
			read(lineIn, space);
						
			--for dummy purposes due to tv-file format
			-- read key mask mk1, 
			hread(lineIn, tv_mk1, good => good_number);
			assert (good_number) report "Incorrect test vector format (mk1)" severity error;
			-- read space character
			read(lineIn, space);
			--for dummy purposes due to tv-file format
			-- read key mask mk2
			hread(lineIn, tv_mk2, good => good_number);
			assert (good_number) report "Incorrect test vector format (mk2)" severity error;
			-- read space character
			read(lineIn, space);
			
			-- read plaintext
			hread(lineIn, tv_plaintext, good => good_number);
			assert (good_number) report "Incorrect test vector format (plaintext)" severity error;
			-- read space character
			read(lineIn, space);
			
			-- read plaintext mask md1
			hread(lineIn, tv_md1, good => good_number);
			assert (good_number) report "Incorrect test vector format (md1)" severity error;
			-- read space character
			read(lineIn, space);
			
			-- read plaintext mask md2
			hread(lineIn, tv_md2, good => good_number);
			assert (good_number) report "Incorrect test vector format (md2)" severity error;
			-- read space character
			read(lineIn, space);
			
			-- read ciphertext
			hread(lineIn, tv_ciphertext, good => good_number);
			assert (good_number) report "Incorrect test vector format (ciphertext)" severity error;
			-- read space character
		
---------------------------------------------------------------------------------------------
-- XXXXXXXXXXXXXXXXX TEST core 2 SHARED PRESENT XXXXXXXXXXXXXXXXX 
---------------------------------------------------------------------------------------------
			testCipherCore2(tv_key, tv_plaintext, tv_ciphertext);

		end loop read_loop_SPS;
		report "---------- End of testvector file tv_SPS_10.txt----------";


    report "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Ende!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
		wait;
	end process check_results;	
	
	

END;
