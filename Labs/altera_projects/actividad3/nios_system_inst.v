  //Example instantiation for system 'nios_system'
  nios_system nios_system_inst
    (
      .clk_0                   (clk_0),
      .in_port_to_the_switches (in_port_to_the_switches),
      .out_port_from_the_leds  (out_port_from_the_leds),
      .reset_n                 (reset_n)
    );

