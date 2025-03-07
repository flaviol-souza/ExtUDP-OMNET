import socket

def udp_client():
    server_address = ('127.0.0.1', 4560)  # Conectando diretamente à simulação
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    try:
        while True:
            message = input("Digite a mensagem (ou 'bye' para sair): ")
            if message.lower() == 'bye':
                break

            print(f"Enviando: {message}")
            client_socket.sendto(message.encode(), server_address)

            data, _ = client_socket.recvfrom(1024)  # Espera resposta do OMNeT++
            print(f"Recebido do servidor: {data.decode()}")

    except KeyboardInterrupt:
        print("\nEncerrando cliente.")

    finally:
        client_socket.close()

if __name__ == '__main__':
    udp_client()
