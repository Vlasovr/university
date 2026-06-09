library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity lpm_rom_model is
  port (
    clk_i     : in  std_logic;
    address_i : in  std_logic_vector(3 downto 0);
    memenab_i : in  std_logic;
    q_o       : out std_logic_vector(7 downto 0)
  );
end entity;

architecture rtl of lpm_rom_model is
  type memory_t is array (0 to 15) of std_logic_vector(7 downto 0);
  signal q_reg : std_logic_vector(7 downto 0) := (others => 'Z');

  constant ROM_C : memory_t := (
    0  => x"11",
    1  => x"27",
    2  => x"3C",
    3  => x"45",
    4  => x"5A",
    5  => x"6E",
    6  => x"73",
    7  => x"8F",
    8  => x"91",
    9  => x"A4",
    10 => x"B8",
    11 => x"C2",
    12 => x"D5",
    13 => x"E9",
    14 => x"F0",
    15 => x"0D"
  );
begin
  process(clk_i)
  begin
    if rising_edge(clk_i) then
      if memenab_i = '1' then
        q_reg <= ROM_C(to_integer(unsigned(address_i)));
      else
        q_reg <= (others => 'Z');
      end if;
    end if;
  end process;

  q_o <= q_reg;
end architecture;
