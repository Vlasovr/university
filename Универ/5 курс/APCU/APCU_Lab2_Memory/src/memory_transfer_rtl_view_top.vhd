library ieee;
use ieee.std_logic_1164.all;

entity memory_transfer_rtl_view_top is
  port (
    CLOCK     : in  std_logic;
    RST       : in  std_logic;
    START     : in  std_logic;
    DONE      : out std_logic;
    BUSY      : out std_logic;
    OK        : out std_logic;
    BUS_D0    : out std_logic;
    BUS_D1    : out std_logic;
    BUS_D2    : out std_logic;
    BUS_D3    : out std_logic;
    BUS_D4    : out std_logic;
    BUS_D5    : out std_logic;
    BUS_D6    : out std_logic;
    BUS_D7    : out std_logic
  );
end entity;

architecture structural of memory_transfer_rtl_view_top is
  signal data_bus_s : std_logic_vector(7 downto 0);
  signal unused_state_s : std_logic_vector(2 downto 0);
  signal unused_rom_q_s : std_logic_vector(7 downto 0);
  signal unused_reg_q_s : std_logic_vector(7 downto 0);
  signal unused_ram_q_s : std_logic_vector(7 downto 0);
  signal unused_rom_addr_s : std_logic_vector(3 downto 0);
  signal unused_ram_addr_s : std_logic_vector(3 downto 0);
begin
  U_MEMORY_TRANSFER : entity work.memory_transfer
    port map (
      clk_i       => CLOCK,
      rst_i       => RST,
      start_i     => START,
      done_o      => DONE,
      busy_o      => BUSY,
      verify_ok_o => OK,
      state_o     => unused_state_s,
      data_bus_o  => data_bus_s,
      rom_q_o     => unused_rom_q_s,
      reg_q_o     => unused_reg_q_s,
      ram_q_o     => unused_ram_q_s,
      rom_addr_o  => unused_rom_addr_s,
      ram_addr_o  => unused_ram_addr_s
    );

  BUS_D0 <= data_bus_s(0);
  BUS_D1 <= data_bus_s(1);
  BUS_D2 <= data_bus_s(2);
  BUS_D3 <= data_bus_s(3);
  BUS_D4 <= data_bus_s(4);
  BUS_D5 <= data_bus_s(5);
  BUS_D6 <= data_bus_s(6);
  BUS_D7 <= data_bus_s(7);
end architecture;
