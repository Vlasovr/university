library ieee;
use ieee.std_logic_1164.all;

entity sn74145_sequential is
  port (
    a_i : in  std_logic;
    b_i : in  std_logic;
    c_i : in  std_logic;
    d_i : in  std_logic;
    y_o : out std_logic_vector(9 downto 0)
  );
end entity;

architecture rtl of sn74145_sequential is
begin
  process(a_i, b_i, c_i, d_i)
    variable sel_v : std_logic_vector(3 downto 0);
    variable y_v   : std_logic_vector(9 downto 0);
  begin
    sel_v := d_i & c_i & b_i & a_i;
    y_v := (others => '1');

    case sel_v is
      when "0000" => y_v(0) := '0';
      when "0001" => y_v(1) := '0';
      when "0010" => y_v(2) := '0';
      when "0011" => y_v(3) := '0';
      when "0100" => y_v(4) := '0';
      when "0101" => y_v(5) := '0';
      when "0110" => y_v(6) := '0';
      when "0111" => y_v(7) := '0';
      when "1000" => y_v(8) := '0';
      when "1001" => y_v(9) := '0';
      when others => null;
    end case;

    y_o <= y_v;
  end process;
end architecture;
