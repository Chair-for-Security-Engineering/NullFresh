--
-----------------------------------------------------------------
-- COMPANY : Ruhr University Bochum
-- AUTHOR  : Amir Moradi (amir.moradi@rub.de) Aein Rezaei Shahmirzadi (aein.rezaeishahmirzadi@rub.de)
-- DOCUMENT: https://eprint.iacr.org/2020/
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

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity GF256_INV is
    Port ( clk : in  STD_LOGIC;
			  dcba1 : in  STD_LOGIC_VECTOR (3 downto 0);
           dcba2 : in  STD_LOGIC_VECTOR (3 downto 0);
           hgfe1 : in  STD_LOGIC_VECTOR (3 downto 0);
           hgfe2 : in  STD_LOGIC_VECTOR (3 downto 0);
           FreshRandom : in  STD_LOGIC;
           dcba1_out : out  STD_LOGIC_VECTOR (3 downto 0); --4bit LSB
           dcba2_out : out  STD_LOGIC_VECTOR (3 downto 0);
			  hgfe1_out : out  STD_LOGIC_VECTOR (3 downto 0); --4bit MSB
           hgfe2_out : out  STD_LOGIC_VECTOR (3 downto 0));
end GF256_INV;

architecture Behavioral of GF256_INV is

	signal sq_scl_mul_out1							: STD_LOGIC_VECTOR (4 DOWNTO 1);
	signal sq_scl_mul_out2							: STD_LOGIC_VECTOR (4 DOWNTO 1);
	signal sq_scl_mul_out1_reg						: STD_LOGIC_VECTOR (4 DOWNTO 1);
	signal sq_scl_mul_out2_reg						: STD_LOGIC_VECTOR (4 DOWNTO 1);
	
	signal dcba1_reg1									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal dcba2_reg1									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal hgfe1_reg1									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal hgfe2_reg1									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	
	signal dcba1_reg2									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal dcba2_reg2									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal hgfe1_reg2									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal hgfe2_reg2									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	
	signal GF16_inv_out1								: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal GF16_inv_out2								: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal GF16_inv_out1_reg						: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal GF16_inv_out2_reg						: STD_LOGIC_VECTOR (3 DOWNTO 0);
	
	signal dcba1_reg3									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal dcba2_reg3									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal hgfe1_reg3									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal hgfe2_reg3									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	
	signal dcba1_reg4									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal dcba2_reg4									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal hgfe1_reg4									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	signal hgfe2_reg4									: STD_LOGIC_VECTOR (3 DOWNTO 0);
	
	

begin

	GEN_reg1: ENTITY work.reg
	GENERIC map ( size => 8)
	Port Map (
		clk						=> clk,
		D(3 downto 0)			=> dcba1,
		D(7 downto 4)			=> dcba2,
		Q(3 downto 0)			=> dcba1_reg1,
		Q(7 downto 4)			=> dcba2_reg1 );
		
		
	Inst_Gf6_sq_scl_mul_Canright: entity work.Gf6_sq_scl_mul_Canright 
		PORT MAP(
		clk => clk,
		dcba1 => dcba1,
		dcba2 => dcba2,
		hgfe1 => hgfe1,
		hgfe2 => hgfe2,
		FreshRadnom => FreshRandom,
		tzyx1 => sq_scl_mul_out1,
		tzyx2 => sq_scl_mul_out2 );
		
	GEN_reg2: ENTITY work.reg
	GENERIC map ( size => 8)
	Port Map (
		clk						=> clk,
		D(3 downto 0)			=> hgfe1,
		D(7 downto 4)			=> hgfe2,
		Q(3 downto 0)			=> hgfe1_reg1,
		Q(7 downto 4)			=> hgfe2_reg1 );
		
	-----------------------------------------
	GEN_reg3: ENTITY work.reg
	GENERIC map ( size => 8)
	Port Map (
		clk						=> clk,
		D(3 downto 0)			=> dcba1_reg1,
		D(7 downto 4)			=> dcba2_reg1,
		Q(3 downto 0)			=> dcba1_reg2,
		Q(7 downto 4)			=> dcba2_reg2 );
	
	GEN_reg4: ENTITY work.reg
	GENERIC map ( size => 8)
	Port Map (
		clk						=> clk,
		D(3 downto 0)			=> sq_scl_mul_out1,
		D(7 downto 4)			=> sq_scl_mul_out2,
		Q(3 downto 0)			=> sq_scl_mul_out1_reg,
		Q(7 downto 4)			=> sq_scl_mul_out2_reg );
		
	GEN_reg5: ENTITY work.reg
	GENERIC map ( size => 8)
	Port Map (
		clk						=> clk,
		D(3 downto 0)			=> hgfe1_reg1,
		D(7 downto 4)			=> hgfe2_reg1,
		Q(3 downto 0)			=> hgfe1_reg2,
		Q(7 downto 4)			=> hgfe2_reg2 );
		
	-----------------------------------------
	GEN_reg6: ENTITY work.reg
	GENERIC map ( size => 8)
	Port Map (
		clk						=> clk,
		D(3 downto 0)			=> dcba1_reg2,
		D(7 downto 4)			=> dcba2_reg2,
		Q(3 downto 0)			=> dcba1_reg3,
		Q(7 downto 4)			=> dcba2_reg3 );
		
	Inst_GF16_inv_282: ENTITY work.GF16_inv_282 
	PORT MAP(
		clk => clk,
		dcba1 => sq_scl_mul_out1_reg,
		dcba2 => sq_scl_mul_out2_reg,
		tzyx1 => GF16_inv_out1,
		tzyx2 => GF16_inv_out2 );
		
	GEN_reg7: ENTITY work.reg
	GENERIC map ( size => 8)
	Port Map (
		clk						=> clk,
		D(3 downto 0)			=> hgfe1_reg2,
		D(7 downto 4)			=> hgfe2_reg2,
		Q(3 downto 0)			=> hgfe1_reg3,
		Q(7 downto 4)			=> hgfe2_reg3 );
	-----------------------------------------
	
	GEN_reg8: ENTITY work.reg
	GENERIC map ( size => 8)
	Port Map (
		clk						=> clk,
		D(3 downto 0)			=> dcba1_reg3,
		D(7 downto 4)			=> dcba2_reg3,
		Q(3 downto 0)			=> dcba1_reg4,
		Q(7 downto 4)			=> dcba2_reg4 );
	
	GEN_reg9: ENTITY work.reg
	GENERIC map ( size => 8)
	Port Map (
		clk						=> clk,
		D(3 downto 0)			=> GF16_inv_out1,
		D(7 downto 4)			=> GF16_inv_out2,
		Q(3 downto 0)			=> GF16_inv_out1_reg,
		Q(7 downto 4)			=> GF16_inv_out2_reg );
		
	GEN_reg10: ENTITY work.reg
	GENERIC map ( size => 8)
	Port Map (
		clk						=> clk,
		D(3 downto 0)			=> hgfe1_reg3,
		D(7 downto 4)			=> hgfe2_reg3,
		Q(3 downto 0)			=> hgfe1_reg4,
		Q(7 downto 4)			=> hgfe2_reg4 );
	-----------------------------------------
	
	Inst_Gf16_mul2_Canright1: ENTITY work.Gf16_mul2_Canright 
	PORT MAP(
		clk => clk,
		dcba1 => dcba1_reg4,
		dcba2 => dcba2_reg4,
		hgfe1 => GF16_inv_out1_reg,
		hgfe2 => GF16_inv_out2_reg,
		tzyx1 => hgfe1_out,
		tzyx2 => hgfe2_out);
		
	Inst_Gf16_mul2_Canright2: ENTITY work.Gf16_mul2_Canright 
	PORT MAP(
		clk => clk,
		dcba1 => hgfe1_reg4,
		dcba2 => hgfe2_reg4,
		hgfe1 => GF16_inv_out1_reg,
		hgfe2 => GF16_inv_out2_reg,
		tzyx1 => dcba1_out,
		tzyx2 => dcba2_out);
		
end Behavioral;

