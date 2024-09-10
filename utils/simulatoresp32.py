import socket
import threading

# Variables internes
frequency="sidereal"
speed="x1"
# Mise à jour des variables
def manageCommand(command):
    global frequency
    global speed
    response = "UNKNOWN"
    if command in ['x1','x2','x4','x16']:
        speed=command
        response="OK"
    elif command in ['sidereal','lunar','solar']:
        frequency=command
        response="OK"
    elif command=="getParams":
        response="OK_"+frequency+"_"+speed
    elif command in ['AD','AD+','AD-','DEC+','DEC-','DEC']:
        response="OK"
    print(command)
    return response

# Fonction pour gérer les connexions TCP
def handle_tcp_client(client_socket):
    with client_socket:
        while True:
            data = client_socket.recv(1024).decode('utf-8')
            if not data:
                break
            response = manageCommand(data)
            client_socket.send(response.encode('utf-8'))

# Fonction pour écouter en TCP
def tcp_server(host, port):
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((host, port))
    server.listen(5)
    print(f"[*] TCP listening on {host}:{port}")
    while True:
        client_socket, addr = server.accept()
        print(f"[*] Accepted TCP connection from {addr}")
        client_handler = threading.Thread(target=handle_tcp_client, args=(client_socket,))
        client_handler.start()

# Fonction pour écouter en UDP
def udp_server(host, port):
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server.bind((host, port))
    print(f"[*] UDP listening on {host}:{port}")
    while True:
        data, addr = server.recvfrom(1024)
        print(f"[*] Received UDP data from {addr}")
        command = data.decode('utf-8')
        response = manageCommand(command)
        server.sendto(response.encode('utf-8'), addr)

# Lancement des serveurs TCP et UDP
def start_servers():
    host = "0.0.0.0"
    port = 4000

    # Créer deux threads pour écouter en TCP et en UDP simultanément
    tcp_thread = threading.Thread(target=tcp_server, args=(host, port))
    udp_thread = threading.Thread(target=udp_server, args=(host, port))

    tcp_thread.start()
    udp_thread.start()

    tcp_thread.join()
    udp_thread.join()

if __name__ == "__main__":
    start_servers()
