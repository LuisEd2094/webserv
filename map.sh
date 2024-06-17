c++ map.cpp -g -o maptest && valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --log-file=valgrind-out.txt \
         -s \
         ./maptest /home/luis/proyects/webserv/
