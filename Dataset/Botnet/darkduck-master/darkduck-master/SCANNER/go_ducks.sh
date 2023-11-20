#!/bin/bash

echo -ne "     ___  ____ ___  __ __  ___  __  _________ __\n"
echo -ne "    / _ \/ / // _ \/ //_/ / _ \/ / / / ___/ //_/\n"
echo -ne "   / // /_  _/ , _/ ,<   / // / /_/ / /__/ ,<   \n"
echo -ne "  /____/_/_//_/|_/_/|_|_/____/\____/\___/_/|_|__\n"
echo -ne "    / __/ ___/ / // |/ / |/ /_  // _ \  _  _<  /\n"
echo -ne "   _\ \/ /__/_  _/    /    //_ </ , _/ | |/ / / \n"
echo -ne "  /___/\___/ /_//_/|_/_/|_/____/_/|_|  |___/_/  \n"
echo -ne "                   __  \n "
echo -ne "              ___( o)>\n "
echo -ne "              \ <_. ) \n "
echo -ne "-------------------------------------------------  \n "


rm ips*
python3 ./get_target.py
while IFS='' read -r line || [[ -n "$line" ]]; do
    python3 ./scanner.py $line &
done < ips_local
exit
