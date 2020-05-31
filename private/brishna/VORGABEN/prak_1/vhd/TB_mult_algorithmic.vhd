library IEEE;
use IEEE.std_logic_1164.all; -- import std_logic types
--use IEEE.std_logic_arith.all; -- import add/sub of std_logic_vector
--use IEEE.std_logic_unsigned.all;
--use IEEE.std_logic_signed.all;
use IEEE.math_real.all; 
use IEEE.numeric_std.all; -- for type conversion to_unsigned

--library STD;
--use STD.textio.all;

--------------------------------------------------------------------------------
--!@file: TB_mult_algorithmic.vhd
--!@brief: testbench for the algorithmic multiplier description
--!...
--
--!@author: Tobias Koal(TK)
--!@revision info :
-- last modification by tkoal(TK)
-- Mon Apr 13 14:27:02 CEST 2015
--------------------------------------------------------------------------------

-- entity description

entity TB_mult_algorithmic is
end entity;

-- architecture description

architecture testbench of TB_mult_algorithmic is


	-- CONSTANTS (upper case only!)


	-- SIGNALS (lower case only!)
	
	component mult_algorithmic
  port(
  		a   : in   unsigned(19 downto 0);
  		b   : in   unsigned(19 downto 0);
  		z   : out  unsigned(39 downto 0)
  );
  end component;
--vhdl
  signal a: unsigned(19 downto 0);
  signal b: unsigned(19 downto 0);
  signal z: unsigned(39 downto 0) ;
  
  --Golden device
  signal goldenDevice_a: unsigned(19 downto 0);
	signal goldenDevice_b: unsigned(19 downto 0);
	signal goldenDevice_z: unsigned(39 downto 0) ;
	
	-- Constants to check for the Multilications
    constant number :  unsigned(19 downto 0) :=  "00000000000000111111" ;
    -- the int range that i want to check the multiplication with it
    constant rangeofLoop: integer := 50;   

begin

	-- this is your algorithmic multiplier description
	tb_component : entity work. mult_algorithmic(algorithmic_description)
	port map (
	  a => a,
	  b => b,
	  z => z
	);

	-- create a stimulus process here!
 stimulus: process
 begin
	for input_a in 0 to rangeofLoop loop
	  a <= number+input_a;
	for input_b in 0 to rangeofLoop loop
	b <= number+input_b; 
	wait for 10 ns;	
	end loop ; 
end loop ; 


wait;
end process;

	-- create a golden device here!
 goldendevice: process
 begin
	for input_a1 in 0 to rangeofLoop loop
	  goldenDevice_a <= number+input_a1;
	for input_b1 in 0 to rangeofLoop loop
	
	goldenDevice_b <=number+input_b1; 
	
	goldenDevice_z<=goldenDevice_a*goldenDevice_b;	
	wait for 10 ns;
	end loop ; 
end loop ; 
end process;

	-- create a compare process here!
compare: process
 begin
	for input_a2 in 0 to rangeofLoop loop
	for input_b2 in 0 to rangeofLoop loop
	
	if (z /= goldenDevice_z) then report "error" ;
	end if;
	
	wait for 10 ns;
	end loop ; 
end loop ; 
end process;

end testbench;
