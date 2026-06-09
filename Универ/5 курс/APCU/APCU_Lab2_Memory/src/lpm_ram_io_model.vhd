library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity lpm_ram_io_model is
  port (
    clk_i     : in    std_logic;
    address_i : in    std_logic_vector(3 downto 0);
    we_i      : in    std_logic;
    out_en_i  : in    std_logic;
    data_io   : inout std_logic_vector(7 downto 0);
    q_dbg_o   : out   std_logic_vector(7 downto 0)
  );
end entity;

architecture rtl of lpm_ram_io_model is
  type memory_t is array (0 to 15) of std_logic_vector(7 downto 0);
  signal ram : memory_t := (others => (others => '0'));
  signal q_reg : std_logic_vector(7 downto 0) := (others => 'Z');
begin
  process(clk_i)
  begin
    if rising_edge(clk_i) then
      if we_i = '1' then
        ram(to_integer(unsigned(address_i))) <= data_io;
      end if;

      if out_en_i = '1' and we_i = '0' then
        q_reg <= ram(to_integer(unsigned(address_i)));
      else
        q_reg <= (others => 'Z');
      end if;
    end if;
  end process;

  data_io <= q_reg when out_en_i = '1' and we_i = '0' else (others => 'Z');
  q_dbg_o <= q_reg;
end architecture;
