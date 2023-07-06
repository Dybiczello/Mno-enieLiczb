/* verilator lint_off UNUSED */
/* verilator lint_off MULTIDRIVEN */
/* verilator lint_off UNDRIVEN */
/* verilator lint_off COMBDLY */
/* verilator lint_off WIDTH */
/* verilator lint_off BLKSEQ */
/* verilator lint_off BLKANDNBLK */

module gpioemu(                             //magistrala z CPU
    n_reset,
    saddress[15:0], srd, swr,                           
    sdata_in[31:0], sdata_out[31:0], 
    gpio_in[31:0], gpio_latch,          	//styk z GPIO - in                          
    gpio_out[31:0],                     	//styk z GPIO - out
    clk,                                	//sygnal zegara
    gpio_in_s_insp[31:0]);                  //sygnał testowy

    input 			clk;
	input 			n_reset;
	input 	[15:0] 	saddress;               //magistrala adresów
	input 			srd;                    //odczyt przez CPU z mag. danych
	input 			swr;                    //zapis przez CPU do mag. danych
	input	[31:0] 	sdata_in;               //magistrala wejściowa CPU
	input 	[31:0] 	gpio_in;                //dane z peryferii wejście do modulu
	input			gpio_latch;             //zapis danych na gpio_in

	output 	[31:0] 	sdata_out;              //magistrala wyjściowa z CPU
	output	[31:0] 	gpio_out;               //dane wyjsciowe do peryferii
    output  [31:0]  gpio_in_s_insp;         //debuging

	reg 	[31:0] 	gpio_in_s;           	//stan peryferii wyjsciowych
	reg 	[31:0] 	gpio_out_s;          	//stan peryferii wejsciowych    
	
	reg		[31:0] 	sdata_out_s;      	    //stan magistrali danych - wyjscie
	reg     [15:0]  counter;             	//licznik wykonanych operacji  
	reg     [23:0]  A1;                  	//rejestr zawierajacy pierwsza liczbe
    reg     [23:0]  A2;                  	//rejestr zawierajacy druga liczbe
    reg     [31:0]  W;                   	//wynik operacji mnozenia
    reg     [5:0]   L;              	    //liczba jedynek w wyniku (max 32 jedynki = wystarczy 6 bitow)
    reg     [31:0]  B;                   	//stan zleconej operacji(
    
    reg     [47:0]  result;                  //rejestr pomocniczy 

    
    
	integer i = 0;
	integer j = 0;

	assign sdata_out = sdata_out_s;
	assign gpio_out = gpio_out_s;
    assign gpio_in_s_insp = gpio_in_s;


	always @(negedge n_reset) 
	begin
		counter <= 16'h0;
		gpio_out_s <= 0;
        A1 <= 0;
        A2 <= 0;
        W <= 0;
        B <= 0;
        L <= 0;
        result <= 0;
		i <= 0;
		j <= 0;
        
        
	end
	always @(posedge clk) begin
            if (B == 1) // jelsi B ==1 następuje rozpoaczęcie operacji możenia
		begin
			result <= 0;
			for (i = 0; i < 23; i = i + 1) begin   
                if (A2[i] == 1) begin
                    result = result + (A1 << i);
                end
			end
			for (j = 0; j < 47; j = j + 1) begin
                     if (result[j] == 1) begin
                         L = L + 1;
					 end

			if (result > 2**32 - 1) 
			begin
				B <= 2; // B = 2 oznacza to błąd
				i <= 0;
				j <= 0;
				L <= 6'h0;
				
			end 
			else begin
				B <= 0; 
				i <= 0;
				j <= 0;
				
				W <= result;
				counter <= counter + 1;
			end
            end
		end
    end

	// odczyt z szyny gpio_in po sygnale gpio_latch
		
	always @(posedge gpio_latch)
	begin
		gpio_in_s <= gpio_in;
	end
			
	// odczyt danych z magistrali	
    always @(posedge srd) begin
        case (saddress)
            16'h100: sdata_out_s[31:0] <= {8'h0, A1};
            16'h108: sdata_out_s[31:0] <= {8'h0, A2};
            16'h110: sdata_out_s <= W;
            16'h120: sdata_out_s <= B;
            16'h118: sdata_out_s <= {26'h0, L};
			16'h130: sdata_out_s <= {16'h0, counter [15:0]};

            default: sdata_out_s <= 32'h0;
        endcase
    end
		
	// zapis danych do magistrali
    always @(posedge swr) begin
        if (saddress == 16'h100) begin
				A1 <= sdata_in[23:0];
		end
		else if (saddress == 16'h108) begin
				A2 <= sdata_in[23:0];
				L <= 6'h0;
				if (B == 0) begin 
					B <= 1;
				end
		end
    end
		
		
endmodule