import socket
#create an INET, STREAMing socket
s = socket.socket(
    socket.AF_INET, socket.SOCK_STREAM)
#now connect to the web server on port 80
# - the normal http port
s.connect(('192.168.2.102', 5555))
s.send("VICTOR;08:00:00;12:00:00\n")
s.send("IGOR;09:00:00;13:00:00\n")
s.send("ANA;10:00:00;14:00:00\n")
s.send("AABBCCDDEE;00:00:00;00:00:00\n")
s.send("FINISH\n");
s.close()