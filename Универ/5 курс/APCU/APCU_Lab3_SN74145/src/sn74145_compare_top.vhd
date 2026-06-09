library ieee;
use ieee.std_logic_1164.all;

entity sn74145_compare_top is
  port (
    A        : in  std_logic;
    B        : in  std_logic;
    C        : in  std_logic;
    D        : in  std_logic;
    YL0      : out std_logic;
    YL1      : out std_logic;
    YL2      : out std_logic;
    YL3      : out std_logic;
    YL4      : out std_logic;
    YL5      : out std_logic;
    YL6      : out std_logic;
    YL7      : out std_logic;
    YL8      : out std_logic;
    YL9      : out std_logic;
    YS0      : out std_logic;
    YS1      : out std_logic;
    YS2      : out std_logic;
    YS3      : out std_logic;
    YS4      : out std_logic;
    YS5      : out std_logic;
    YS6      : out std_logic;
    YS7      : out std_logic;
    YS8      : out std_logic;
    YS9      : out std_logic;
    MATCH    : out std_logic
  );
end entity;

architecture structural of sn74145_compare_top is
  signal y_logic_s : std_logic_vector(9 downto 0);
  signal y_seq_s   : std_logic_vector(9 downto 0);
begin
  U_LOGIC : entity work.sn74145_concurrent
    port map (
      a_i => A,
      b_i => B,
      c_i => C,
      d_i => D,
      y_o => y_logic_s
    );

  U_SEQ : entity work.sn74145_sequential
    port map (
      a_i => A,
      b_i => B,
      c_i => C,
      d_i => D,
      y_o => y_seq_s
    );

  YL0 <= y_logic_s(0);
  YL1 <= y_logic_s(1);
  YL2 <= y_logic_s(2);
  YL3 <= y_logic_s(3);
  YL4 <= y_logic_s(4);
  YL5 <= y_logic_s(5);
  YL6 <= y_logic_s(6);
  YL7 <= y_logic_s(7);
  YL8 <= y_logic_s(8);
  YL9 <= y_logic_s(9);

  YS0 <= y_seq_s(0);
  YS1 <= y_seq_s(1);
  YS2 <= y_seq_s(2);
  YS3 <= y_seq_s(3);
  YS4 <= y_seq_s(4);
  YS5 <= y_seq_s(5);
  YS6 <= y_seq_s(6);
  YS7 <= y_seq_s(7);
  YS8 <= y_seq_s(8);
  YS9 <= y_seq_s(9);

  MATCH <= '1' when y_logic_s = y_seq_s else '0';
end architecture;
