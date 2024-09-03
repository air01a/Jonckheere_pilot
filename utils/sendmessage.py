import sys
import socket

def send_udp_message(message, ip, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(message.encode('utf-8'), (ip, port))
    print(f"Message '{message}' envoyé à {ip}:{port}")
    sock.close()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python udp_sender.py <IP_ADDRESS> <MESSAGE>")
        sys.exit(1)
    
    #ip_address = sys.argv[1]
    message = sys.argv[1]
    
    udp_port = 4000  # Port UDP 4000
    
    send_udp_message(message, "192.168.1.1", udp_port)