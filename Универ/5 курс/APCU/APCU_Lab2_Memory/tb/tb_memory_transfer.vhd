library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_memory_transfer is
end entity;

architecture sim of tb_memory_transfer is
  signal clk       : std_logic := '0';
  signal rst       : std_logic := '1';
  signal start     : std_logic := '0';
  signal done      : std_logic;
  signal busy      : std_logic;
  signal verify_ok : std_logic;
  signal state     : std_logic_vector(2 downto 0);
  signal data_bus  : std_logic_vector(7 downto 0);
  signal rom_q     : std_logic_vector(7 downto 0);
  signal reg_q     : std_logic_vector(7 downto 0);
  signal ram_q     : std_logic_vector(7 downto 0);
  signal rom_addr  : std_logic_vector(3 downto 0);
  signal ram_addr  : std_logic_vector(3 downto 0);

  procedure tick(signal clk_s : in std_logic) is
  begin
    wait until rising_edge(clk_s);
    wait for 1 ns;
  end procedure;
begin
  clk <= not clk after 5 ns;

  DUT : entity work.memory_transfer
    port map (
      clk_i       => clk,
      rst_i       => rst,
      start_i     => start,
      done_o      => done,
      busy_o      => busy,
      verify_ok_o => verify_ok,
      state_o     => state,
      data_bus_o  => data_bus,
      rom_q_o     => rom_q,
      reg_q_o     => reg_q,
      ram_q_o     => ram_q,
      rom_addr_o  => rom_addr,
      ram_addr_o  => ram_addr
    );

  stimulus : process
  begin
    tick(clk);
    rst <= '0';
    start <= '1';

    tick(clk);
    assert state = "001" report "FSM must enter ROM read state" severity failure;
    assert rom_addr = x"4" report "ROM source address must be 4" severity failure;

    tick(clk);
    assert state = "010" report "FSM must enter register load state" severity failure;
    assert data_bus = x"5A" report "Register must see ROM[4] value 0x5A on shared bus" severity failure;

    tick(clk);
    assert state = "011" report "FSM must enter RAM write state" severity failure;
    assert reg_q = x"5A" report "Register must capture value 0x5A" severity failure;
    assert ram_addr = x"5" report "RAM destination address must be 5" severity failure;
    assert data_bus = x"5A" report "Register must drive value 0x5A during RAM write" severity failure;

    tick(clk);
    assert state = "100" report "FSM must enter RAM readback state" severity failure;

    tick(clk);
    assert done = '1' report "Transfer must finish with done=1" severity failure;
    assert verify_ok = '1' report "Verification flag must be active after successful transfer" severity failure;
    assert ram_q = x"5A" report "RAM[5] must contain transferred value 0x5A" severity failure;
    assert data_bus = x"5A" report "RAM readback must drive 0x5A onto the bus" severity failure;

    start <= '0';
    tick(clk);
    assert done = '0' report "FSM must return to idle after start is released" severity failure;

    assert false report "tb_memory_transfer: TEST PASSED" severity note;
    wait;
  end process;
end architecture;
