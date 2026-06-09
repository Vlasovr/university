library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity mod11_counter is
  port (
    clk_i  : in  std_logic;
    rst_i  : in  std_logic;
    en_i   : in  std_logic;
    load_i : in  std_logic;
    q_o    : out std_logic_vector(3 downto 0);
    tc_o   : out std_logic
  );
end entity;

architecture rtl of mod11_counter is
  constant COUNT_MIN_C : unsigned(3 downto 0) := to_unsigned(5, 4);
  constant COUNT_MAX_C : unsigned(3 downto 0) := to_unsigned(15, 4);

  signal count_reg : unsigned(3 downto 0) := COUNT_MIN_C;
begin
  process(clk_i)
  begin
    if rising_edge(clk_i) then
      if rst_i = '1' or load_i = '1' then
        count_reg <= COUNT_MIN_C;
      elsif en_i = '1' then
        if count_reg = COUNT_MAX_C then
          count_reg <= COUNT_MIN_C;
        else
          count_reg <= count_reg + 1;
        end if;
      end if;
    end if;
  end process;

  q_o  <= std_logic_vector(count_reg);
  tc_o <= '1' when count_reg = COUNT_MAX_C and en_i = '1' else '0';
end architecture;
