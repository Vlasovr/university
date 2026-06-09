library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_sn74145 is
end entity;

architecture sim of tb_sn74145 is
  signal a       : std_logic := '0';
  signal b       : std_logic := '0';
  signal c       : std_logic := '0';
  signal d       : std_logic := '0';
  signal y_logic : std_logic_vector(9 downto 0);
  signal y_seq   : std_logic_vector(9 downto 0);

  function bit_from_int(value_i : integer) return std_logic is
  begin
    if value_i = 0 then
      return '0';
    end if;
    return '1';
  end function;
begin
  U_LOGIC : entity work.sn74145_concurrent
    port map (
      a_i => a,
      b_i => b,
      c_i => c,
      d_i => d,
      y_o => y_logic
    );

  U_SEQ : entity work.sn74145_sequential
    port map (
      a_i => a,
      b_i => b,
      c_i => c,
      d_i => d,
      y_o => y_seq
    );

  stimulus : process
    variable expected_v : std_logic_vector(9 downto 0);
  begin
    for code_i in 0 to 15 loop
      a <= bit_from_int(code_i mod 2);
      b <= bit_from_int((code_i / 2) mod 2);
      c <= bit_from_int((code_i / 4) mod 2);
      d <= bit_from_int((code_i / 8) mod 2);
      wait for 10 ns;

      expected_v := (others => '1');
      if code_i <= 9 then
        expected_v(code_i) := '0';
      end if;

      assert y_logic = expected_v
        report "Concurrent decoder output mismatch" severity failure;
      assert y_seq = expected_v
        report "Sequential decoder output mismatch" severity failure;
      assert y_logic = y_seq
        report "Concurrent and sequential implementations are not equivalent" severity failure;
    end loop;

    assert false report "tb_sn74145: TEST PASSED; checked 16 BCD/input combinations" severity note;
    wait;
  end process;
end architecture;
