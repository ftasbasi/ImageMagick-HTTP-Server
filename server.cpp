// Server side C program to demonstrate HTTP Server programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fstream>
#include <Magick++.h>
#include <iostream>
#include <sstream>
#include <exception>

using namespace std;
using namespace Magick;
#define PORT 8080
int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char *error_msg = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 59\n\nSomething went wrong, please check your image or parameters!";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    int enable = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);


    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    Geometry geo;
    geo.aspect(true);

    while (1) {
        //printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        try {

            if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
                perror("In accept");
                exit(EXIT_FAILURE);
            }


            if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
                perror("setsockopt(SO_REUSEADDR) failed");

            char buffer[1024] = {0};
            valread = read(new_socket, buffer, 1024);
            if(valread <1) {
                throw new std::runtime_error("couldn't read the request");
            }
            string basicString(buffer, 1024); // length optional, but needed if there may be zero's in your data
            istringstream is(basicString);

            string line;
            getline(is, line);
            line = line.substr(4);
            int resize_start;
            int url_start;
            int url_end;
            url_end = line.find(" ");
            url_start = line.find("/url:") + 5;
            string url = line.substr(url_start, url_end - url_start); // find the middle word


            string operation = line.substr(4, line.find(":") - 4);

            resize_start = line.find(operation + ":") + operation.length() + 1;
            url_start = line.find("/url:");
            string sizeofImg = line.substr(resize_start, url_start - resize_start); // find the middle word

            int width, height;
            width = atoi(sizeofImg.substr(0, sizeofImg.find("x")).c_str());
            height = atoi(
                    sizeofImg.substr(sizeofImg.find("x") + 1, sizeofImg.length() - sizeofImg.find("x") - 1).c_str());


            InitializeMagick(*argv);
            if (url.find("http") != 0) {
                url = "https://" + url;
            }
            // string extension=url.rfind(".")+1,url.length()-url.rfind("."));
            Image urlImage(url);
            if (operation == "resize") {
                geo.height(height);
                geo.width(width);
                urlImage.resize(geo);

            } else if (operation == "crop") {
                urlImage.crop(Geometry(width, height, 100, 100));
            } else if (operation == "grayscale") {
                urlImage.type(GrayscaleType);
            } else if (operation == "rotate") {
                urlImage.rotate(stod(line.substr(line.find(operation + ":") + operation.length() + 1,
                                                 line.find("/url:") - line.find(":") - 1)));
            } else {
                throw new std::runtime_error("wrong parameter");
            }

            urlImage.write("test.jpeg");
            string headers = "HTTP/1.0 200 OK\r\nContent-type: image/JPEG\r\n\r\n";
            send(new_socket, headers.data(), headers.length(), 0);
            ifstream f("test.jpeg", ios::in | ios::binary | ios::ate);
            streampos size = f.tellg();
            char *image = new char[size];
            f.seekg(0, ios::beg);
            f.read(image, size);
            f.close();

            send(new_socket, image, size, 0);
            remove("test.jpeg");
            free(image);
            }
        catch (std::runtime_error* rerror)
        {
            write(new_socket, error_msg, strlen(error_msg));
            cout << rerror->what()<< '\n';
            free(rerror);
        }
        catch(...)
        {
            write(new_socket, error_msg, strlen(error_msg));
            std::exception_ptr p = std::current_exception();
            std::clog <<(p ? p.__cxa_exception_type()->name() : "null") << std::endl;

        }
        close(new_socket);

    }

    return 0;
}