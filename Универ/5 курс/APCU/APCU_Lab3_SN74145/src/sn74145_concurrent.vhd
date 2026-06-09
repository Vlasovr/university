library ieee;
use ieee.std_logic_1164.all;

entity sn74145_concurrent is
  port (
    a_i : in  std_logic;
    b_i : in  std_logic;
    c_i : in  std_logic;
    d_i : in  std_logic;
    y_o : out std_logic_vector(9 downto 0)
  );
end entity;

architecture rtl of sn74145_concurrent is
begin
  y_o(0) <= a_i or b_i or c_i or d_i;
  y_o(1) <= (not a_i) or b_i or c_i or d_i;
  y_o(2) <= a_i or (not b_i) or c_i or d_i;
  y_o(3) <= (not a_i) or (not b_i) or c_i or d_i;
  y_o(4) <= a_i or b_i or (not c_i) or d_i;
  y_o(5) <= (not a_i) or b_i or (not c_i) or d_i;
  y_o(6) <= a_i or (not b_i) or (not c_i) or d_i;
  y_o(7) <= (not a_i) or (not b_i) or (not c_i) or d_i;
  y_o(8) <= a_i or b_i or c_i or (not d_i);
  y_o(9) <= (not a_i) or b_i or c_i or (not d_i);
end architecture;
