library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity memory_transfer is
  port (
    clk_i       : in  std_logic;
    rst_i       : in  std_logic;
    start_i     : in  std_logic;
    done_o      : out std_logic;
    busy_o      : out std_logic;
    verify_ok_o : out std_logic;
    state_o     : out std_logic_vector(2 downto 0);
    data_bus_o  : out std_logic_vector(7 downto 0);
    rom_q_o     : out std_logic_vector(7 downto 0);
    reg_q_o     : out std_logic_vector(7 downto 0);
    ram_q_o     : out std_logic_vector(7 downto 0);
    rom_addr_o  : out std_logic_vector(3 downto 0);
    ram_addr_o  : out std_logic_vector(3 downto 0)
  );
end entity;

architecture rtl of memory_transfer is
  constant SRC_ADDR_C  : std_logic_vector(3 downto 0) := x"4";
  constant DST_ADDR_C  : std_logic_vector(3 downto 0) := x"5";
  constant EXPECTED_C  : std_logic_vector(7 downto 0) := x"5A";

  type state_t is (S_IDLE, S_READ_ROM, S_LOAD_REG, S_WRITE_RAM, S_READ_RAM, S_DONE);

  signal state_reg : state_t := S_IDLE;
  signal state_next : state_t;
  signal data_bus : std_logic_vector(7 downto 0);
  signal rom_q : std_logic_vector(7 downto 0);
  signal ram_q : std_logic_vector(7 downto 0);
  signal reg_q : std_logic_vector(7 downto 0) := (others => '0');

  signal rom_en : std_logic;
  signal reg_oe : std_logic;
  signal ram_we : std_logic;
  signal ram_oe : std_logic;
begin
  U_ROM : entity work.lpm_rom_model
    port map (
      clk_i     => clk_i,
      address_i => SRC_ADDR_C,
      memenab_i => rom_en,
      q_o       => rom_q
    );

  U_RAM : entity work.lpm_ram_io_model
    port map (
      clk_i     => clk_i,
      address_i => DST_ADDR_C,
      we_i      => ram_we,
      out_en_i  => ram_oe,
      data_io   => data_bus,
      q_dbg_o   => ram_q
    );

  data_bus <= rom_q when rom_en = '1' else (others => 'Z');
  data_bus <= reg_q when reg_oe = '1' else (others => 'Z');

  process(state_reg, start_i)
  begin
    state_next <= state_reg;
    case state_reg is
      when S_IDLE =>
        if start_i = '1' then
          state_next <= S_READ_ROM;
        end if;
      when S_READ_ROM =>
        state_next <= S_LOAD_REG;
      when S_LOAD_REG =>
        state_next <= S_WRITE_RAM;
      when S_WRITE_RAM =>
        state_next <= S_READ_RAM;
      when S_READ_RAM =>
        state_next <= S_DONE;
      when S_DONE =>
        if start_i = '0' then
          state_next <= S_IDLE;
        end if;
    end case;
  end process;

  process(clk_i)
  begin
    if rising_edge(clk_i) then
      if rst_i = '1' then
        state_reg <= S_IDLE;
        reg_q <= (others => '0');
      else
        if state_reg = S_LOAD_REG then
          reg_q <= data_bus;
        end if;
        state_reg <= state_next;
      end if;
    end if;
  end process;

  rom_en <= '1' when state_reg = S_READ_ROM or state_reg = S_LOAD_REG else '0';
  reg_oe <= '1' when state_reg = S_WRITE_RAM else '0';
  ram_we <= '1' when state_reg = S_WRITE_RAM else '0';
  ram_oe <= '1' when state_reg = S_READ_RAM or state_reg = S_DONE else '0';

  done_o      <= '1' when state_reg = S_DONE else '0';
  busy_o      <= '0' when state_reg = S_IDLE or state_reg = S_DONE else '1';
  verify_ok_o <= '1' when state_reg = S_DONE and ram_q = EXPECTED_C else '0';

  with state_reg select state_o <=
    "000" when S_IDLE,
    "001" when S_READ_ROM,
    "010" when S_LOAD_REG,
    "011" when S_WRITE_RAM,
    "100" when S_READ_RAM,
    "101" when others;

  data_bus_o <= data_bus;
  rom_q_o    <= rom_q;
  reg_q_o    <= reg_q;
  ram_q_o    <= ram_q;
  rom_addr_o <= SRC_ADDR_C;
  ram_addr_o <= DST_ADDR_C;
end architecture;
