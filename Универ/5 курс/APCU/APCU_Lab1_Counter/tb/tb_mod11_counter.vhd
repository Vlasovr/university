library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_mod11_counter is
end entity;

architecture sim of tb_mod11_counter is
  signal clk  : std_logic := '0';
  signal rst  : std_logic := '1';
  signal en   : std_logic := '0';
  signal load : std_logic := '0';
  signal q    : std_logic_vector(3 downto 0);
  signal tc   : std_logic;

  procedure tick(signal clk_s : in std_logic) is
  begin
    wait until rising_edge(clk_s);
    wait for 1 ns;
  end procedure;

  procedure expect_q(
    constant expected : in natural;
    constant message  : in string
  ) is
  begin
    assert unsigned(q) = to_unsigned(expected, 4)
      report message severity failure;
  end procedure;
begin
  clk <= not clk after 5 ns;

  DUT : entity work.mod11_counter
    port map (
      clk_i  => clk,
      rst_i  => rst,
      en_i   => en,
      load_i => load,
      q_o    => q,
      tc_o   => tc
    );

  stimulus : process
  begin
    tick(clk);
    expect_q(5, "Reset must force initial state 5");

    rst <= '0';
    en <= '1';

    for expected in 6 to 15 loop
      tick(clk);
      expect_q(expected, "Counter sequence from 5 to 15 is broken");
    end loop;

    assert tc = '1'
      report "Terminal-count flag must be active at value 15 while enable is high"
      severity failure;

    tick(clk);
    expect_q(5, "Counter must wrap from 15 to 5");

    en <= '0';
    tick(clk);
    tick(clk);
    expect_q(5, "Counter must hold its state when enable is low");

    load <= '1';
    en <= '1';
    tick(clk);
    expect_q(5, "Load must force state 5");

    load <= '0';
    tick(clk);
    expect_q(6, "Counter must continue after load is released");

    assert false report "tb_mod11_counter: TEST PASSED" severity note;
    wait;
  end process;
end architecture;
