--
-----------------------------------------------------------------
-- COMPANY : Ruhr University Bochum
-- AUTHOR  : Amir Moradi (amir.moradi@rub.de) Aein Rezaei Shahmirzadi (aein.rezaeishahmirzadi@rub.de)
-- DOCUMENT: https://eprint.iacr.org/2020/890.pdf
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

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY substitutionCombined IS
		PORT ( state_s1  : IN  STD_LOGIC_VECTOR (63 DOWNTO 0);
				 state_s2  : IN  STD_LOGIC_VECTOR (63 DOWNTO 0);

				 sel			: IN STD_LOGIC;
				 clk			: IN STD_LOGIC;
				 
				 result_s1 : OUT  STD_LOGIC_VECTOR (63 DOWNTO 0);
				 result_s2 : OUT  STD_LOGIC_VECTOR (63 DOWNTO 0));
END substitutionCombined;

ARCHITECTURE behavioral OF substitutionCombined IS
	
BEGIN

	substition_PRINCE:
		FOR i IN 0 TO 15 GENERATE
			sBoxCombined_PRINCE: ENTITY work.MaskedSbox
				PORT MAP ( input1  => state_s1(((i+1) * 4 - 1) DOWNTO i*4),
							  input2  => state_s2(((i+1) * 4 - 1) DOWNTO i*4),

							  
							  sel		 	 => sel,
							  clk		 	 => clk,
							  
							  output1 => result_s1(((i+1) * 4 - 1) DOWNTO i*4),
							  output2 => result_s2(((i+1) * 4 - 1) DOWNTO i*4));
		END GENERATE;
		
END behavioral;

