valgrind --trace-children=yes --track-fds=yes --log-fd=2 --error-limit=no \
         --leak-check=full --show-possibly-lost=yes --track-origins=yes \
         --show-reachable=yes --log-file=valgrind-out.txt ./webserv config_file.conf 
		
