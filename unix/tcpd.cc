#include <iostream>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace {
    const std::string SERVER_PATH = "/tmp/stream_server.soc";
}

class LocalServerSocket
{
public:
    LocalServerSocket(const std::string& path);
    int Initialize(int backlog = 16);
    void Run();
    static void Server(int fd);
private:
    int sockfd;
    struct sockaddr_un addr;
    std::vector<std::thread> workers;
};

LocalServerSocket::LocalServerSocket(const std::string& path)
{
    addr.sun_family = AF_LOCAL;
    strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1);
    unlink(path.c_str());
}

int
LocalServerSocket::Initialize(int backlog)
{
    sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;
    }

    int rc;
    rc = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if (rc < 0) {
        return -1;
    }

    rc = listen(sockfd, backlog);
    if (rc < 0) {
        return -1;
    }

    return 0;
}

void
LocalServerSocket::Run()
{
    int fd;
    socklen_t len;

    for (;;) {
        len = sizeof(addr);
        fd = accept(sockfd, (struct sockaddr*)&addr, &len);
        if (fd < 0) {
            if (errno == EINTR) {
                continue;
            }
            else {
                return;
            }
        }

        try {
            std::thread* th = new std::thread(Server, fd);
            workers.push_back(std::move(*th));
        }
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

void
LocalServerSocket::Server(int fd)
{
    std::cout << "Server" << std::endl;

    uint8_t* buf = reinterpret_cast<uint8_t*>(malloc(BUFSIZ));
    for (;;) {
        ssize_t sz = recv(fd, buf, BUFSIZ, 0);
        if (sz < 0) {
            std::cerr << "recv failed." << std::endl;
            goto exit;
        }
        if (sz == 0) {
            goto exit;
        }

        std::cout << sz << " bytes received." << std::endl;

        ssize_t len = sz;
        switch (buf[0]) {
        case 'a':
            sz = send(fd, buf, len, 0);
            if (sz < 0) {
                std::cerr << "send failed." << std::endl;
                goto exit;
            }
            break;
        case 'z':
            goto exit;
        default:
            break;
        }
    }
exit:
    free(buf);
    printf("Server exit\n");
}


int
main(int argc, char* argv[])
{
    LocalServerSocket server(SERVER_PATH);
    if (server.Initialize() < 0) {
        std::cerr << "Initialization failed." << std::endl;
        return EXIT_FAILURE;
    }

    server.Run();

    return EXIT_SUCCESS;
}
