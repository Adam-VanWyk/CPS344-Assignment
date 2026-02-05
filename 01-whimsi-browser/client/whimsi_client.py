import socket
import sys

from whimsi_renderer import WhimsiRenderer

# YOU DO NOT NEED TO MODIFY THIS FUNCTION!
def send_request_to_server(request, host="localhost", port=53009):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((host, port))
            s.sendall(request.encode())
            response = s.recv(1024).decode()
            return response
    except Exception as e:
        print(f"Error communicating with server: {e}")
        return None

# Below is an example of how to use the renderer
# TODO: Replace this with your own code
# (1) Parse a .whimsi file and support all the commands
# (1a) Handling IMPORT requires recursive parsing
# (1b) Handling CLICK requires binding the on_click function below to load the file
# (2) Instead of loading files from the local file system, load them from the server

def renderer(content):
    lines = content.strip().slpit('\n')
    
    for line in lines:
        item = line.split()
        type = item[0]

        if type == "TEXT":
            x = int(item[1])
            y = int(item[2])
            text = item[3]
            size = int(item[4])
            color = item[5]
            whimsi.draw_text(x, y, text, size, color)

        elif type == "RECT":
            x = int(item[1])
            y = int(item[2])
            wid = int(item[3])
            hig = int(item[4])
            color = item[5]
            whimsi.draw_rectangle(x, y, wid, hig, color)

        elif type == "CIRCLE":
            x = int(item[1])
            y = int(item[2])
            rad = int(item[3])
            color = item[4]
            whimsi.draw_circle(x, y, rad, color)

        elif type == "ALIEN":
            x = int(item[1])
            y = int(item[2])
            size = int(item[3])
            whimsi.draw_alien(x, y, size)

        elif type == "RAINBOW":
            x = int(item[1])
            y = int(item[2])
            size = int(item[3])
            whimsi.draw_rainbow(x, y, size)

        elif type == "CAT":
            x = int(item[1])
            y = int(item[2])
            size = int(item[3])
            whimsi.draw_cat(x, y, size)

        elif type == "IMPORT":
            file = item[1]
            imported_file = send_request_to_server(file)
            if imported_file:
                renderer(imported_file)

        elif type == "CLICK":
            file = item[1]
            x = int(item[2])
            y = int(item[3])
            text = item[4]
            size = int(item[5])
            color = item[6]
            whimsi.draw_clickable_text(file, x, y, text, size, color)

if __name__ == "__main__":
    whimsi = WhimsiRenderer()
    if len(sys.argv) < 2:
        print("Usage: python3 whimsi_client.py <route (e.g, localhost/hello.whimsi)>")
        sys.exit(1)
    
    route = sys.argv[1]
    host, file_path = route.split("/")

    def on_click(file_path):
        # Example: Print the clicked file path
        print("Clicked file:", file_path)
        # TODO: Fetch file from server and render it
        fetch_response = send_request_to_server("FETCH")
        renderer(fetch_response)
        # Here is an example of loading cats which you should replace
        # whimsi.clear()
        # for i in range(10):
        #     whimsi.draw_cat(10 + i * 100, 20 + i * 50, 200)

    for i in range(10):
        whimsi.draw_text(10 + i * 10, 10 + i * 20, "Hello World!", 20, "black")
    whimsi.draw_alien(200, 200, 200)

    whimsi.draw_clickable_text("../examples/cats.whimsi", 450, 450, "Load cats", 30, "blue", on_click)

    # Example of how to fetch from server
    # Note - make sure to run server first otherwise you will get None
    hello_response = send_request_to_server("HELLO")
    print("SERVER SAYS:", hello_response)

    # needed at very end, otherwise nothing will be shown
    whimsi.exitonclick()
