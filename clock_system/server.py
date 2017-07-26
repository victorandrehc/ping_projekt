#!/usr/bin/env python
import urllib2
import socket
import threading
import sys

BASE_URL="https://script.google.com/macros/s/AKfycbyhGjBXVw6pPQhnUutMd0s-GjPRjVhjZuZX0paQNCrfJ2j93pF-/exec?"
OK="Ok"
FINISH="FINISH"

class Client(threading.Thread):
	"""docstring for Client"""
	def __init__(self,clientsocket):
		self.clientsocket=clientsocket
		super(Client, self).__init__(target=self.function)

	def function(self):
		stop=False
		last_received=""
		while stop==False:

			received=last_received+self.clientsocket.recv(29)
			print received
			received_list=received.split('\n')
			last_received=received_list.pop()
			for r in received_list:
				if r!=FINISH:
					r=r.split(';')
					print r
					url=BASE_URL
					url+='name='+r[0]+'&'
					url+='in='+r[1]+'&'
					url+='out='+r[2]
					returned=urllib2.urlopen(url).read() 
					print returned
				else:
					stop=True
					break
		print "CLOSING CONNECTION"
		self.clientsocket.close()





def main():
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	# Bind the socket to the port
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	s.connect(("8.8.8.8", 80))
	host =s.getsockname()[0]
	s.close()
	print host
	server_address = (host, 5555)
	print >>sys.stderr, 'starting up on %s port %s' % server_address
	sock.bind(server_address)
	sock.listen(1)
	while(1):
		#accept connections from outside
		(clientsocket, address) = sock.accept()
		#now do something with the clientsocket
		#in this case, we'll pretend this is a threaded server
		
		ct = Client(clientsocket)
		ct.start()


if __name__=="__main__":
	main()



