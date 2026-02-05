import socket
import threading
import os

def handle_client(conn, addr, base_path):
    # TODO: Implement FETCH command by looking at HELLO command
    # you will load files in the base_path directory
    # if a file is not found, return "ERROR::FILE_NOT_FOUND"
    # You also need to handle invalid commands with "ERROR::INVALID_COMMAND"

    with conn:
        data = conn.recv(1024) # whimsi protocol has a maximum request size of 1024 bytes
        request = data.decode().strip()
        print(f"{addr} - {request}")

        command = request.split(" ")[0]

        if (command != "HELLO" and command != "FETCH"):
            raise Exception("ERROR::INVALID_COMMAND")

        if command == "HELLO":
            response = "WORLD"
            conn.sendall(response.encode())

        if command == "FETCH":
            try:
                file = request.split(" ", 1)[1]
                path = os.path.join(base_path, file)
             
                if not os.path.isfile(path):
                    conn.sendall("ERROR::FILE_NOT_FOUND".encode())
                    return
                with open(path, 'r') as f:
                    conn.sendall(f.read().encode())
            except Exception as e: 
                conn.sendall("ERROR::INVALID_COMMAND".encode())
                

def start_server(host='localhost', port=53009, base_path='.'):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen()
        print(f"Server listening on {host}:{port}")

        while True:
            conn, addr = s.accept()
            thread = threading.Thread(target=handle_client, args=(conn, addr, base_path))
            thread.start()

if __name__ == "__main__":
    start_server(base_path='../whimsi/')
