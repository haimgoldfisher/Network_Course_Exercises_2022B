# import socket module
from socket import *
import sys  # In order to terminate the program

serverSocket = socket(AF_INET, SOCK_STREAM)
# Prepare a sever socket
port = 6789
try:
    serverSocket.bind(('0.0.0.0', port))  # '0.0.0.0' listen to every possible IPv4,
    # this method connects the socket to ip address & the port
except error:
    sys.exit()
serverSocket.listen(1)  # waiting for a respond from the CLIENT
while True:
    # Establish the connection
    print('Ready to serve...')
    connectionSocket, addr = serverSocket.accept() # it checks if the entire input had received
    try:
        message = connectionSocket.recv(1024) # it holds the data of the input
        print(message)
        filename = message.split()[1]
        f = open(filename[1:])
        outputdata = f.read()
        # Send one HTTP header line into socket
        connectionSocket.send('HTTP/1.1 200 OK\n\n'.encode())  # the connection had succeeded
        print("OK")  # for self debug
        # Send the content of the requested file to the client
        for i in range(0, len(outputdata)):
            connectionSocket.send(outputdata[i].encode())
        connectionSocket.send("\r\n".encode())
        connectionSocket.close()
    except IOError:
        # Send response message for file not found
        connectionSocket.send('HTTP/1.1 404 Not Found\n\n'.encode())  # the connection had failed
        print("Not Found")  # for self debug
        connectionSocket.close()
    # Close client socket
    serverSocket.close()
    sys.exit()  # Terminate the program after sending the corresponding data
