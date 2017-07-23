#!/usr/bin/env python
import urllib2
import socket
import threading
import sys

BASE_URL="https://script.google.com/macros/s/AKfycbyhGjBXVw6pPQhnUutMd0s-GjPRjVhjZuZX0paQNCrfJ2j93pF-/exec?"
OK="Ok"

class Client(threading.Thread):
	"""docstring for Client"""
	def __init__(self,clientsocket):
		self.clientsocket=clientsocket
		super(Client, self).__init__(target=self.function)

	def function(self):
		received= self.clientsocket.recv(100)
		received=received.split(';')
		url=BASE_URL
		url+='name='+received[0]+'&'
		url+='in='+received[1]+'&'
		url+='out='+received[2]
		
		returned=urllib2.urlopen(url).read() 
		print returned
		self.clientsocket.close()





def main():
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	# Bind the socket to the port
	server_address = ('localhost', 5555)
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



