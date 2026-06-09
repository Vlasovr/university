library ieee;
use ieee.std_logic_1164.all;

entity mod11_counter_rtl_view_top is
  port (
    CLOCK : in  std_logic;
    RST   : in  std_logic;
    EN    : in  std_logic;
    LOAD  : in  std_logic;
    Q0    : out std_logic;
    Q1    : out std_logic;
    Q2    : out std_logic;
    Q3    : out std_logic;
    TC    : out std_logic
  );
end entity;

architecture structural of mod11_counter_rtl_view_top is
  signal q_s : std_logic_vector(3 downto 0);
begin
  U_COUNTER : entity work.mod11_counter
    port map (
      clk_i  => CLOCK,
      rst_i  => RST,
      en_i   => EN,
      load_i => LOAD,
      q_o    => q_s,
      tc_o   => TC
    );

  Q0 <= q_s(0);
  Q1 <= q_s(1);
  Q2 <= q_s(2);
  Q3 <= q_s(3);
end architecture;
