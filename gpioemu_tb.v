`timescale 1ns/10ps

module gpioemu_tb;

  // Inputs
  reg clk;
  reg n_reset;
  reg [15:0] saddress;
  reg srd;
  reg swr;
  reg [31:0] sdata_in;
  reg [31:0] gpio_in;
  reg gpio_latch;

  // Outputs
  wire [31:0] sdata_out;
  wire [31:0] gpio_out;
  wire [31:0] gpio_in_s_insp;


  // Instancja testowa
  gpioemu gpioemu (
    n_reset,
    saddress,
    srd,
    swr,
    sdata_in,
    sdata_out,
    gpio_in,
    gpio_latch,
    gpio_out,
    clk,
    gpio_in_s_insp
  );

  initial begin
	$dumpfile("gpioemu.vcd");
	$dumpvars(0,gpioemu_tb);
	end

  // Generacja zegara
  initial begin
    clk = 0;
    forever #5 clk = ~clk;
  end

  initial begin
    // Inicjalizacja inputów
    n_reset = 0;
    saddress = 16'h0;
    srd = 0;
    swr = 0;
    sdata_in = 32'h0;
    gpio_in = 32'h0;
    gpio_latch = 0;
    #10;

    #5 n_reset = 1;

    // ustawienie wartosci
	  #5 sdata_in = 24'hface;
	  #5 gpio_latch = 1;
    #5
	  #5 gpio_latch = 0;
    #10

    // Zapis do rejestru A1
    #5 saddress = 16'h100;
    #5 swr = 1;
    #5
    #5 swr = 0;
    #10;

    // Odczyt z rejestru A1
    #5 saddress = 16'h100;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;

    // zmiana wartosci
	  #5 sdata_in = 24'hff;
	  #5 gpio_latch = 1;
    #5
	  #5 gpio_latch = 0;
    #10

    // Zapis do rejestru A2 --- pierwsze mnozenie
    #5 saddress = 16'h108;
    #5 swr = 1;
    #5
    #5 swr = 0;
    #10;
    
    // Odczyt z rejestru A2
    #5 saddress = 16'h108;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;

    // Odczyt z rejestru B
    #5 saddress = 16'h120;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;

    #100;

    // Odczyt z rejestru W
    #5 saddress = 16'h110;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;

    // Odczyt z rejestru licznika operacji
    #5 saddress = 16'h130;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;
	
	// ustawienie wartosci
	  #5 sdata_in = 24'hba5;
	  #5 gpio_latch = 1;
    #5
	  #5 gpio_latch = 0;
    #10

    // Zapis do rejestru A1
    #5 saddress = 16'h100;
    #5 swr = 1;
    #5
    #5 swr = 0;
    #10;

    // Odczyt z rejestru A1
    #5 saddress = 16'h100;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;

    // zmiana wartosci
	  #5 sdata_in = 24'h0;
	  #5 gpio_latch = 1;
    #5
	  #5 gpio_latch = 0;
    #10

    // Zapis do rejestru A2 --- drugie mnozenie
    #5 saddress = 16'h108;
    #5 swr = 1;
    #5
    #5 swr = 0;
    #10;
    
    // Odczyt z rejestru A2
    #5 saddress = 16'h108;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;
	 
	 // Odczyt z rejestru B
    #5 saddress = 16'h120;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;

    #100;

    // Odczyt z rejestru W
    #5 saddress = 16'h110;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;

    // Odczyt z rejestru licznika operacji
    #5 saddress = 16'h130;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;
	
	// ustawienie wartosci
	  #5 sdata_in = 24'hface;
	  #5 gpio_latch = 1;
    #5
	  #5 gpio_latch = 0;
    #10

    // Zapis do rejestru A1
    #5 saddress = 16'h100;
    #5 swr = 1;
    #5
    #5 swr = 0;
    #10;

    // Odczyt z rejestru A1
    #5 saddress = 16'h100;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;

    // zmiana wartosci
	  #5 sdata_in = 24'h1;
	  #5 gpio_latch = 1;
    #5
	  #5 gpio_latch = 0;
    #10

    // Zapis do rejestru A2 --- trzecie mnozenie
    #5 saddress = 16'h108;
    #5 swr = 1;
    #5
    #5 swr = 0;
    #10;
    
    // Odczyt z rejestru A2
    #5 saddress = 16'h108;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;
	 
	 // Odczyt z rejestru B
    #5 saddress = 16'h120;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;

    #100;

    // Odczyt z rejestru W
    #5 saddress = 16'h110;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;

    // Odczyt z rejestru licznika operacji
    #5 saddress = 16'h130;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;

    // ustawienie wartosci
	  #5 sdata_in = 24'hf9f8f7;
	  #5 gpio_latch = 1;
    #5
	  #5 gpio_latch = 0;
    #10

    // Zapis do rejestru A1
    #5 saddress = 16'h100;
    #5 swr = 1;
    #5
    #5 swr = 0;
    #10;

    // Odczyt z rejestru A1
    #5 saddress = 16'h100;
    #5 srd = 1;
    #5
    #5 srd = 0;
    #10;

    // zmiana wartosci
	  #5 sdata_in = 24'hf1f2f3;
	  #5 gpio_latch = 1;
    #5
	  #5 gpio_latch = 0;
    #10

    // Zapis do rejestru A2 --- czwarte mnozenie
    #5 saddress = 16'h108;
    #5 swr = 1;
    #5
    #5 swr = 0;
    #100;

    $finish;
  end

endmodule
