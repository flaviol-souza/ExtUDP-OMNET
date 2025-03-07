import socket

def client_program():
    host = '127.0.0.1'  # IP address of the OMNeT++ UDP server
    port = 4560         # Port number where the OMNeT++ server listens

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP socket

    message = input(" -> ")  # User input

    while message.lower().strip() != 'bye':
        try:
            print('Sending msg to the server:', message)
            client_socket.sendto(message.encode(), (host, port))  # Send UDP message
        except Exception as e:
            print(e)
        try:
            print('waiting answer from server')
            data, server_address = client_socket.recvfrom(1024)  # Receive UDP response
            print('Received from server:', data.decode())  # Display server response
        except socket.timeout:
            print("No response from server")

        message = input(" -> ")  # Get next user input

    client_socket.close()  # Close the socket

if __name__ == '__main__':
    client_program()